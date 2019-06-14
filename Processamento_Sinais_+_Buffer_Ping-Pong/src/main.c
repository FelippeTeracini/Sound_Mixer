/** 
 * 12- ADC
 * Rafael Corsi @ insper.edu.br
 * Abril 2017
 * 
 * Configura o ADC do SAME70 para fazer leitura
 * do sensor de temperatura interno
 * 
 * Alterado por Sabrina SS @ al.insper.edu.br
 * 2018.1
 * 
 * Configura ADC e AFEC para fazer leitura
 * de um sinal externo e retornar como sinal
 * analógico
 */

/************************************************************************/
/* Includes                                                             */
/************************************************************************/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "asf.h"
#include "PingPong.h"
#include "queue.h"
#include <math.h>

/************************************************************************/
/* Defines                                                              */
/************************************************************************/

/** Header printf */
#define STRING_EOL    "\r"
#define STRING_HEADER "-- AFEC TO DAC AUDIO PROCESSING --\r\n" \
"-- "BOARD_NAME" --\r\n" \
"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

//channel 0 = PD30
#define AFEC_CHANNEL_PIN 0
//afec 1 | channel 1 = PC13
#define AFEC_CHANNEL_PIN_GAIN 1
//afec 1 | channel 5 = PC30
#define AFEC_CHANNEL_PIN_SATURATION 3
//afec 1 | channel 3 = PC12
#define AFEC_CHANNEL_PIN_LOWPASS 5

//! DAC channel used for test
#define DACC_CHANNEL        0 // (PB13)
//! DAC register base for test
#define DACC_BASE           DACC
//! DAC ID for test
#define DACC_ID             ID_DACC

// Botao Reverb
#define BUTREVERB_PIO      PIOD
#define BUTREVERB_PIO_ID   16
#define BUTREVERB_IDX  28
#define BUTREVERB_IDX_MASK (1 << BUTREVERB_IDX)

// Botao Echo
#define BUTECHO_PIO      PIOC
#define BUTECHO_PIO_ID   12
#define BUTECHO_IDX  31
#define BUTECHO_IDX_MASK (1 << BUTECHO_IDX)

// Botao3
#define BUT3_PIO      PIOA
#define BUT3_PIO_ID   10
#define BUT3_IDX  19
#define BUT3_IDX_MASK (1 << BUT3_IDX)

// LED Reverb
#define LEDREVERB_PIO      PIOA
#define LEDREVERB_PIO_ID   10
#define LEDREVERB_IDX  0
#define LEDREVERB_IDX_MASK (1 << LEDREVERB_IDX)

// LED Echo
#define LEDECHO_PIO      PIOB
#define LEDECHO_PIO_ID   11
#define LEDECHO_IDX  2
#define LEDECHO_IDX_MASK (1 << LEDECHO_IDX)


/** RTOS  */
#define TASK_TRIGGER_STACK_SIZE            (1024/sizeof(portSTACK_TYPE))
#define TASK_TRIGGER_STACK_PRIORITY        (tskIDLE_PRIORITY)
#define TASK_UARTTX_STACK_SIZE             (2048/sizeof(portSTACK_TYPE))
#define TASK_UARTTX_STACK_PRIORITY         (tskIDLE_PRIORITY)
#define TASK_UARTRX_STACK_SIZE             (2048/sizeof(portSTACK_TYPE))
#define TASK_UARTRX_STACK_PRIORITY         (1)
#define TASK_PROCESS_STACK_SIZE            (2048/sizeof(portSTACK_TYPE))
#define TASK_PROCESS_STACK_PRIORITY        (2)

#define MAX_DIGITAL     (4095UL)

xQueueHandle xQueueGain;
xQueueHandle xQueueSaturation;
xQueueHandle xQueueLowpass;

xQueueHandle xQueueEffects;

xSemaphoreHandle xSemaphoreReverb;
xSemaphoreHandle xSemaphoreEcho;

typedef struct {
	int gain;
	int saturation;
	int lowpass;
} effects_t;

typedef struct {
	int values[22000];
	int p;
	int maxp;
} old_buffer_t;

/************************************************************************/
/*        definir funcs                                                 */
/************************************************************************/
void TC_init(Tc * TC, int ID_TC, int TC_CHANNEL, int freq);

extern void vApplicationStackOverflowHook(xTaskHandle *pxTask,
signed char *pcTaskName);
extern void vApplicationIdleHook(void);
extern void vApplicationTickHook(void);
extern void vApplicationMallocFailedHook(void);
extern void xPortSysTickHandler(void);

static void Softning();
static void Hard_clipping();
static void Volume();


/************************************************************************/
/* RTOS application funcs                                               */
/************************************************************************/

/**
 * \brief Called if stack overflow during execution
 */
extern void vApplicationStackOverflowHook(xTaskHandle *pxTask,
		signed char *pcTaskName)
{
	printf("stack overflow %x %s\r\n", pxTask, (portCHAR *)pcTaskName);
	/* If the parameters have been corrupted then inspect pxCurrentTCB to
	 * identify which task has overflowed its stack.
	 */
	for (;;) {
	}
}

/**
 * \brief This function is called by FreeRTOS idle task
 */
extern void vApplicationIdleHook(void)
{
	pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);
}

/**
 * \brief This function is called by FreeRTOS each tick
 */
extern void vApplicationTickHook(void)
{
}

extern void vApplicationMallocFailedHook(void)
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */

	/* Force an assert. */
	configASSERT( ( volatile void * ) NULL );
}


/************************************************************************/
/* Callbacks / Handler                                                 */
/************************************************************************/
/**
*  Interrupt handler for TC1 interrupt.
*/
void TC0_Handler(void){
	volatile uint32_t ul_dummy;

	/****************************************************************
	* Devemos indicar ao TC que a interrup��o foi satisfeita.
	******************************************************************/
	ul_dummy = tc_get_status(TC0, 0);

	/* Avoid compiler warning */
	UNUSED(ul_dummy);

}

/**
 * \brief AFEC interrupt callback function.
 */

// BUFFER SIZE
PPBUF_DECLARE(buffer,1000);
volatile uint32_t buf = 0;

uint32_t corte_filtro = 4000;
float volume = 0.5;
int g_ul_value_old = 0;
uint32_t temp;
int g_ul_value = 0;
uint32_t high_cut = 3000;
uint32_t low_cut = 300;

volatile int saturation_value = 100;
volatile int gain_value = 50;
volatile int lowpass_value = 10000;
volatile int count = 0;
volatile int ground = 400;

volatile char reverb_on = 0;
volatile char echo_on = 0;

old_buffer_t old_buffer;

static void Softning(){
	temp = g_ul_value;
	g_ul_value = (int) ((float) g_ul_value * (float) g_ul_value_old* 0.01) + g_ul_value ;
	g_ul_value_old = temp;

}

static void Hard_clipping(){
	g_ul_value = g_ul_value *4;
	
	if (g_ul_value > high_cut){
		g_ul_value = high_cut;
	}
	if (g_ul_value < low_cut){
		g_ul_value = low_cut;
	}
	g_ul_value = g_ul_value / 4;

}

static void Volume(){
	g_ul_value  = (int) ((float) g_ul_value * volume);

}

static void Saturation(int value) {
	int sat_value = (int) ((100 - value) * ground / 100) + 20;
	
	if (g_ul_value > ground + sat_value) g_ul_value = ground + sat_value;
	if (g_ul_value < ground - sat_value) g_ul_value = ground - sat_value;
}

static void Gain(int value) {
	int percent100 = 90;
	
	g_ul_value = (int) (((g_ul_value - ground) * value / percent100) + ground);
}

static void lowPassFrequency(int CUTOFF)
{
	double RC = 1.0/(CUTOFF*2*3.14);
	double dt = 1.0/22000;
	double alpha = dt/(RC+dt);
	
	int current = g_ul_value - ground;
	int past = g_ul_value_old - ground;
	
	g_ul_value = (int) ((double) past + (alpha*((double) current - (double) past)) + ground);
}

static void reverb(void) {
	double normal_value = (g_ul_value - ground);

	g_ul_value = (int) (normal_value/3 +
		/*old_buffer.values[(old_buffer.p + (old_buffer.maxp/2))%old_buffer.maxp]*(((double) value)/ (max_value*3)) + */
		(old_buffer.values[(old_buffer.p + (old_buffer.maxp/2))%old_buffer.maxp] + 
		old_buffer.values[(old_buffer.p + (old_buffer.maxp/4))%old_buffer.maxp])*0.1666 +
		(old_buffer.values[(old_buffer.p + (old_buffer.maxp/8))%old_buffer.maxp] +
		old_buffer.values[(old_buffer.p + (old_buffer.maxp/16))%old_buffer.maxp])*0.0833 +
		(old_buffer.values[(old_buffer.p + (old_buffer.maxp/32))%old_buffer.maxp] +
		old_buffer.values[(old_buffer.p + (old_buffer.maxp/64))%old_buffer.maxp] +
		old_buffer.values[(old_buffer.p + (old_buffer.maxp/128))%old_buffer.maxp] +
		old_buffer.values[(old_buffer.p + (old_buffer.maxp/256))%old_buffer.maxp])*0.0416 +
		ground);

}

static void echo(void) {
	double normal_value = (g_ul_value - ground);
	
	g_ul_value = (int) (normal_value*2/3 +
		old_buffer.values[old_buffer.p%old_buffer.maxp]*0.333 +
		ground);

}

static void AFEC_Audio_callback(void){
	/** The conversion data value */

	// check swap
	if(ppbuf_get_full_signal(&buffer,false) == true) {
		ppbuf_get_full_signal(&buffer,true); // swap
// 		printf("Signal: %d\n", g_ul_value);
// 		printf("Average: %d\n", count/11000);
		count = 0;
	}
	
	g_ul_value = afec_channel_get_value(AFEC0, AFEC_CHANNEL_PIN);
	
	/*        EFFECTS        */
	if (echo_on)
		echo();
	if (reverb_on)
		reverb();
	Saturation(saturation_value);
	Gain(gain_value);
	lowPassFrequency(lowpass_value);
	
	
	count += g_ul_value;
	g_ul_value_old = g_ul_value;
	if (old_buffer.p < old_buffer.maxp) {
		old_buffer.values[old_buffer.p] = g_ul_value - ground;
		old_buffer.p += 1;
	} else {
		old_buffer.values[old_buffer.p] = g_ul_value - ground;
		old_buffer.p = 0;
	}
	
	ppbuf_insert_active(&buffer, &g_ul_value, 1);
	ppbuf_remove_inactive(&buffer, &buf, 1);	
	
	dacc_get_interrupt_status(DACC_BASE);
	dacc_write_conversion_data(DACC_BASE, buf, DACC_CHANNEL);

}

static int32_t convert_adc_to_gain(int32_t ADC_value){

	uint32_t max_gain = 100;
	return (ADC_value*(max_gain) / MAX_DIGITAL);
}

static int32_t convert_adc_to_saturation(int32_t ADC_value){

	uint32_t max_saturation = 100;
	return (ADC_value*(max_saturation) / MAX_DIGITAL);
}

static int32_t convert_adc_to_lowpass(int32_t ADC_value){

	uint32_t max_lowpass = 10000;
	double x = (((double) ADC_value) / MAX_DIGITAL) * (((double) ADC_value) / MAX_DIGITAL);
	return x * max_lowpass + 20;
}

static void AFEC_Gain_callback(void){
	uint32_t gain;
	gain = afec_channel_get_value(AFEC1, AFEC_CHANNEL_PIN_GAIN);
	gain = convert_adc_to_gain(gain);
/*	printf("CALLBACK: %d\n", gain);*/
	xQueueSendFromISR( xQueueGain, &gain, NULL);
}

static void AFEC_Saturation_callback(void){
	uint32_t saturation;
	saturation = afec_channel_get_value(AFEC1, AFEC_CHANNEL_PIN_SATURATION);
	saturation = convert_adc_to_saturation(saturation);
	xQueueSendFromISR( xQueueSaturation, &saturation, NULL);
}

static void AFEC_Lowpass_callback(void){
	uint32_t lowpass;
	lowpass = afec_channel_get_value(AFEC1, AFEC_CHANNEL_PIN_LOWPASS);
	lowpass = convert_adc_to_lowpass(lowpass);
	xQueueSendFromISR( xQueueLowpass, &lowpass, NULL);
}

void but1_callback(void)
{
	xSemaphoreGiveFromISR(xSemaphoreReverb, 4);
}

void but2_callback(void)
{
	xSemaphoreGiveFromISR(xSemaphoreEcho, 4);
}

void but3_callback(void)
{
	
}
/************************************************************************/
/* Funcoes                                                              */
/************************************************************************/

/**
 * \brief Configure UART console.
 * BaudRate : 115200
 * 8 bits
 * 1 stop bit
 * sem paridade
 */
static void configure_console(void)
{
 
	const usart_serial_options_t uart_serial_options = {
		.baudrate   = CONF_UART_BAUDRATE,
		.charlength = CONF_UART_CHAR_LENGTH,
		.paritytype = CONF_UART_PARITY,
		.stopbits   = CONF_UART_STOP_BITS,
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

static void config_ADC_AUDIO(void){
/************************************* 
   * Ativa e configura AFEC
   *************************************/  
  /* Ativa AFEC - 0 */
	afec_enable(AFEC0);

	/* struct de configuracao do AFEC */
	struct afec_config afec_cfg;

	/* Carrega parametros padrao */
	afec_get_config_defaults(&afec_cfg);

	/* Configura AFEC */
	afec_init(AFEC0, &afec_cfg);
  
	/* Configura trigger por software */
	afec_set_trigger(AFEC0, AFEC_TRIG_TIO_CH_0);
		
	AFEC0->AFEC_MR |= 3;
  
	/* configura call back */
	afec_set_callback(AFEC0, AFEC_INTERRUPT_EOC_0,	AFEC_Audio_callback, 1); 
   
	/*** Configuracao espec�fica do canal AFEC ***/
	struct afec_ch_config afec_ch_cfg;
	afec_ch_get_config_defaults(&afec_ch_cfg);
	afec_ch_cfg.gain = AFEC_GAINVALUE_0;
	afec_ch_set_config(AFEC0, AFEC_CHANNEL_PIN, &afec_ch_cfg);
  
	/*
	* Calibracao:
	* Because the internal ADC offset is 0x200, it should cancel it and shift
	 down to 0.
	 */
	//afec_channel_set_analog_offset(AFEC0, AFEC_CHANNEL_PIN, 0x1B0);
	afec_channel_set_analog_offset(AFEC0, AFEC_CHANNEL_PIN, 0x1A0);

	/***  Configura sensor de temperatura ***/
	struct afec_temp_sensor_config afec_temp_sensor_cfg;

	afec_temp_sensor_get_config_defaults(&afec_temp_sensor_cfg);
	afec_temp_sensor_set_config(AFEC0, &afec_temp_sensor_cfg);

	/* Selecina canal e inicializa convers�o */  
	afec_channel_enable(AFEC0, AFEC_CHANNEL_PIN);
}

static void config_AFEC_EFFECTS(void){
/*************************************
   * Ativa e configura AFEC
   *************************************/
  /* Ativa AFEC - 0 */
	afec_enable(AFEC1);

	/* struct de configuracao do AFEC */
	struct afec_config afec_cfg;

	/* Carrega parametros padrao */
	afec_get_config_defaults(&afec_cfg);

	/* Configura AFEC */
	afec_init(AFEC1, &afec_cfg);

	/* Configura trigger por software */
	afec_set_trigger(AFEC1, AFEC_TRIG_SW);

/*	AFEC1->AFEC_MR |= 3;*/

	/* configura call back */
	afec_set_callback(AFEC1, AFEC_INTERRUPT_EOC_3,	AFEC_Saturation_callback, 1);
	afec_set_callback(AFEC1, AFEC_INTERRUPT_EOC_1,	AFEC_Gain_callback, 1);
	afec_set_callback(AFEC1, AFEC_INTERRUPT_EOC_5,	AFEC_Lowpass_callback, 1);

	/*** Configuracao espec?fica do canal AFEC ***/
	struct afec_ch_config afec_ch_cfg;
	afec_ch_get_config_defaults(&afec_ch_cfg);
	afec_ch_cfg.gain = AFEC_GAINVALUE_0;
	afec_ch_set_config(AFEC1, AFEC_CHANNEL_PIN_GAIN, &afec_ch_cfg);
	afec_ch_set_config(AFEC1, AFEC_CHANNEL_PIN_SATURATION, &afec_ch_cfg);
	afec_ch_set_config(AFEC1, AFEC_CHANNEL_PIN_LOWPASS, &afec_ch_cfg);

	/*
	* Calibracao:
	* Because the internal ADC offset is 0x200, it should cancel it and shift
	 down to 0.
	 */
	afec_channel_set_analog_offset(AFEC1, AFEC_CHANNEL_PIN_GAIN, 0x200);
	afec_channel_set_analog_offset(AFEC1, AFEC_CHANNEL_PIN_SATURATION, 0x200);
	afec_channel_set_analog_offset(AFEC1, AFEC_CHANNEL_PIN_LOWPASS, 0x200);

	/***  Configura sensor de temperatura ***/
	struct afec_temp_sensor_config afec_temp_sensor_cfg;

	afec_temp_sensor_get_config_defaults(&afec_temp_sensor_cfg);
	afec_temp_sensor_set_config(AFEC1, &afec_temp_sensor_cfg);
}

static void config_DAC(void){
	/* Enable clock for DACC */
	sysclk_enable_peripheral_clock(DACC_ID);

	/* Reset DACC registers */
	dacc_reset(DACC_BASE);
	dacc_enable_channel(DACC_BASE, DACC_CHANNEL);
}

/**
* Configura TimerCounter (TC) para gerar uma interrupcao no canal (ID_TC e TC_CHANNEL)
* na taxa de especificada em freq.
*/
void TC_init(Tc * TC, int ID_TC, int TC_CHANNEL, int freq){
	uint32_t ul_div;
	uint32_t ul_tcclks;
	uint32_t ul_sysclk = sysclk_get_cpu_hz();

	pmc_enable_periph_clk(ID_TC);

	/** Configura o TC para operar em  4Mhz e interrup�c�o no RC compare */
	tc_find_mck_divisor(freq, ul_sysclk, &ul_div, &ul_tcclks, ul_sysclk);
	
	//PMC->PMC_SCER = 1 << 14;
	ul_tcclks = 1;
	
	tc_init(TC, TC_CHANNEL, ul_tcclks 
							| TC_CMR_WAVE /* Waveform mode is enabled */
							| TC_CMR_ACPA_SET /* RA Compare Effect: set */
							| TC_CMR_ACPC_CLEAR /* RC Compare Effect: clear */
							| TC_CMR_CPCTRG /* UP mode with automatic trigger on RC Compare */
	);
	
	tc_write_rc(TC, TC_CHANNEL, (ul_sysclk / ul_div) / freq /8 );
	tc_write_ra(TC, TC_CHANNEL, (ul_sysclk / ul_div) / freq / 8 / 2);

	tc_start(TC, TC_CHANNEL);
}

void button_init(void)
{

	// Inicializa clock do perif�rico PIO responsavel pelo botao
	pmc_enable_periph_clk(BUTREVERB_PIO_ID);
	pmc_enable_periph_clk(BUTECHO_PIO_ID);
	pmc_enable_periph_clk(BUT3_PIO_ID);

	// Configura PIO para lidar com o pino do bot�o como entrada
	// com pull-up
	pio_configure(BUTREVERB_PIO, PIO_INPUT, BUTREVERB_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_configure(BUTECHO_PIO, PIO_INPUT, BUTECHO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_configure(BUT3_PIO, PIO_INPUT, BUT3_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);

	// Configura interrup��o no pino referente ao botao e associa
	// fun��o de callback caso uma interrup��o for gerada
	// a fun��o de callback � a: but_callback()
	pio_handler_set(BUTREVERB_PIO, BUTREVERB_PIO_ID, BUTREVERB_IDX_MASK, PIO_IT_FALL_EDGE, but1_callback);
	pio_handler_set(BUTECHO_PIO, BUTECHO_PIO_ID, BUTECHO_IDX_MASK, PIO_IT_FALL_EDGE, but2_callback);
	pio_handler_set(BUT3_PIO, BUT3_PIO_ID, BUT3_IDX_MASK, PIO_IT_FALL_EDGE, but3_callback);


	// Ativa interrup��o
	pio_enable_interrupt(BUTREVERB_PIO, BUTREVERB_IDX_MASK);
	pio_enable_interrupt(BUTECHO_PIO, BUTECHO_IDX_MASK);
	pio_enable_interrupt(BUT3_PIO, BUT3_IDX_MASK);

	// Configura NVIC para receber interrupcoes do PIO do botao
	// com prioridade 4 (quanto mais pr�ximo de 0 maior)
	NVIC_EnableIRQ(BUTREVERB_PIO_ID);
	NVIC_SetPriority(BUTREVERB_PIO_ID, 4); // Prioridade 4
	NVIC_EnableIRQ(BUTECHO_PIO_ID);
	NVIC_SetPriority(BUTECHO_PIO_ID, 4); // Prioridade 4
	NVIC_EnableIRQ(BUT3_PIO_ID);
	NVIC_SetPriority(BUT3_PIO_ID, 4); // Prioridade 4
}

void led_init(void)
{
	// Configura led
	pmc_enable_periph_clk(LEDREVERB_PIO_ID);
	pmc_enable_periph_clk(LEDECHO_PIO_ID);

	pio_configure(LEDREVERB_PIO, PIO_OUTPUT_0, LEDREVERB_IDX_MASK, PIO_DEFAULT);
	pio_configure(LEDECHO_PIO, PIO_OUTPUT_0, LEDECHO_IDX_MASK, PIO_DEFAULT);
}

static void task_adc_to_dac(void *pvParameters) {

	xQueueEffects = xQueueCreate( 100, sizeof( effects_t ) );
	xSemaphoreReverb = xSemaphoreCreateCounting(1, 0);
	xSemaphoreEcho = xSemaphoreCreateCounting(1, 0);
	
	old_buffer.p = 0;
	old_buffer.maxp = 22000;
	for (int i = 0; i < old_buffer.maxp; i++) {
		old_buffer.values[i] = 0;
	}

	config_ADC_AUDIO();
	
	TC_init(TC0, ID_TC0, 0, 44000);

	config_DAC();

	led_init();

	pio_clear(LEDREVERB_PIO, LEDREVERB_IDX_MASK);
	pio_clear(LEDECHO_PIO, LEDECHO_IDX_MASK);
	
	saturation_value = 50;
	gain_value = 70;
	lowpass_value = 10000;

	effects_t effects;
	effects.gain = 50;
	effects.saturation = 0;
	effects.lowpass = 10000;
	
	while(1) {
		if (xQueueReceive( xQueueEffects, &(effects),  10 / portTICK_RATE_MS)) {
// 			printf("Gain_Value: %d\n", effects.gain);
			gain_value = effects.gain;
			saturation_value = effects.saturation;
			lowpass_value = effects.lowpass;
			vTaskDelay( 100 / portTICK_RATE_MS);
		}
		if (xSemaphoreTake( xSemaphoreReverb,  10 / portTICK_RATE_MS)) {
			printf("REVERB PRESSED\n");
			reverb_on = !reverb_on;
			if (reverb_on)
				pio_clear(LEDREVERB_PIO, LEDREVERB_IDX_MASK);
			else
				pio_set(LEDREVERB_PIO, LEDREVERB_IDX_MASK);
		}
		if (xSemaphoreTake( xSemaphoreEcho,  10 / portTICK_RATE_MS)) {
			printf("ECHO PRESSED\n");
			echo_on = !echo_on;
			if (echo_on)
				pio_clear(LEDECHO_PIO, LEDECHO_IDX_MASK);
			else
				pio_set(LEDECHO_PIO, LEDECHO_IDX_MASK);
		}
	}
}

void task_effects_controller(void) {

	xQueueGain = xQueueCreate( 100, sizeof( uint32_t ) );
	xQueueSaturation = xQueueCreate( 100, sizeof( uint32_t ) );
	xQueueLowpass = xQueueCreate( 100, sizeof( uint32_t ) );

	config_AFEC_EFFECTS();

	button_init();
	
	int saturation = 0;
	int gain = 50;
	int lowpass = 10000;

	effects_t effects;
	effects.gain = gain;
	effects.saturation = saturation;
	effects.lowpass = lowpass;

	afec_channel_enable(AFEC1, AFEC_CHANNEL_PIN_GAIN);

	afec_start_software_conversion(AFEC1);

// 	printf("Teste: %d\n", gain);
	
	while(1) {
		if (xQueueReceive( xQueueGain, &(gain),  10 / portTICK_RATE_MS)) {
			printf("Gain: %d\n", gain);
			effects.gain = gain;
			
			afec_channel_disable(AFEC1, AFEC_CHANNEL_PIN_GAIN);
			afec_channel_enable(AFEC1, AFEC_CHANNEL_PIN_SATURATION);
			afec_start_software_conversion(AFEC1);
		}
		if (xQueueReceive( xQueueSaturation, &(saturation), 10 / portTICK_RATE_MS)) {
			printf("Saturation: %d\n", saturation);
			effects.saturation = saturation;
			
			afec_channel_disable(AFEC1, AFEC_CHANNEL_PIN_SATURATION);
			afec_channel_enable(AFEC1, AFEC_CHANNEL_PIN_LOWPASS);
			afec_start_software_conversion(AFEC1);
		}
		if (xQueueReceive( xQueueLowpass, &(lowpass), 10 / portTICK_RATE_MS)) {
			printf("Lowpass: %d\n", lowpass);
			effects.lowpass = lowpass;

			afec_channel_disable(AFEC1, AFEC_CHANNEL_PIN_LOWPASS);
			afec_channel_enable(AFEC1, AFEC_CHANNEL_PIN_GAIN);
			afec_start_software_conversion(AFEC1);
		}
		xQueueSend( xQueueEffects, &effects, NULL);
		vTaskDelay(500 / portTICK_RATE_MS);
	}
}


/************************************************************************/
/* Main                                                                 */
/************************************************************************/
int main(void)
{

	/* Initialize the SAM system. */
	sysclk_init();
	ioport_init();
	board_init();
  
	/* inicializa delay */
	delay_init(sysclk_get_cpu_hz());

	/* inicializa console (printf) */
	configure_console();
  
	/* Output example information. */
	puts(STRING_HEADER);
	
	/*BUT_init();*/
	
	if (xTaskCreate(task_adc_to_dac, "AudioTask", TASK_TRIGGER_STACK_SIZE, NULL,
	TASK_TRIGGER_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to create AudioTask task\r\n");
	}

	if (xTaskCreate(task_effects_controller, "EffectsController", TASK_TRIGGER_STACK_SIZE, NULL,
	TASK_TRIGGER_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to create EffectsController task\r\n");
	}
	
	vTaskStartScheduler();

	while (1) {
		
	}
}
