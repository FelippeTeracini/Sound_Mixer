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
#define AFEC_CHANNEL_PIN_SATURATION 5
//afec 1 | channel 3 = PC12
#define AFEC_CHANNEL_PIN_LOWPASS 3

//! DAC channel used for test
#define DACC_CHANNEL        0 // (PB13)
//! DAC register base for test
#define DACC_BASE           DACC
//! DAC ID for test
#define DACC_ID             ID_DACC

/** RTOS  */
#define TASK_TRIGGER_STACK_SIZE            (1024/sizeof(portSTACK_TYPE))
#define TASK_TRIGGER_STACK_PRIORITY        (tskIDLE_PRIORITY)
#define TASK_UARTTX_STACK_SIZE             (2048/sizeof(portSTACK_TYPE))
#define TASK_UARTTX_STACK_PRIORITY         (tskIDLE_PRIORITY)
#define TASK_UARTRX_STACK_SIZE             (2048/sizeof(portSTACK_TYPE))
#define TASK_UARTRX_STACK_PRIORITY         (1)
#define TASK_PROCESS_STACK_SIZE            (2048/sizeof(portSTACK_TYPE))
#define TASK_PROCESS_STACK_PRIORITY        (2)
#define TASK_EFFECTS_STACK_SIZE            (2*1024/sizeof(portSTACK_TYPE))
#define TASK_EFFECTS_STACK_PRIORITY        (tskIDLE_PRIORITY)

#define MAX_DIGITAL     (4095UL)

QueueHandle_t xQueueGain;
QueueHandle_t xQueueSaturation;
QueueHandle_t xQueueLowpass;

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
/**
 * \brief AFEC interrupt callback function.
 */

// BUFFER SIZE
PPBUF_DECLARE(buffer,11000);
volatile uint32_t buf = 0;

void TC0_Handler(void){
	volatile uint32_t ul_dummy;

	/****************************************************************
	* Devemos indicar ao TC que a interrup??o foi satisfeita.
	******************************************************************/
	ul_dummy = tc_get_status(TC0, 0);

	/* Avoid compiler warning */
	UNUSED(ul_dummy);

}

// QueueHandle_t xQueue1;

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
	int percent100 = 50;
	
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

// static int32_t convert_adc_to_gain(int32_t ADC_value){
// 	
// 	uint32_t max_gain = 100;
// 	return (ADC_value*(max_gain) / MAX_DIGITAL);
// }
// 
// static int32_t convert_adc_to_saturation(int32_t ADC_value){
// 	
// 	uint32_t max_saturation = 100;
// 	return (ADC_value*(max_saturation) / MAX_DIGITAL);
// }
// 
// static int32_t convert_adc_to_lowpass(int32_t ADC_value){
// 	
// 	uint32_t max_lowpass = 100;
// 	return (ADC_value*(max_lowpass) / MAX_DIGITAL);
// }

static void AFEC_Audio_callback(void){
	/** The conversion data value */

	// check swap
	if(ppbuf_get_full_signal(&buffer,false) == true) {
		ppbuf_get_full_signal(&buffer,true); // swap
		printf("Signal: %d\n", g_ul_value);
		printf("Average: %d\n", count/11000);
		count = 0;
	}
	
	g_ul_value = afec_channel_get_value(AFEC0, AFEC_CHANNEL_PIN);
	
	/*        EFFECTS        */
	Saturation(saturation_value);
	Gain(gain_value);
	lowPassFrequency(lowpass_value);
	
	
	count += g_ul_value;
	g_ul_value_old = g_ul_value;
	ppbuf_insert_active(&buffer, &g_ul_value, 1);
	ppbuf_remove_inactive(&buffer, &buf, 1);	
	
	dacc_get_interrupt_status(DACC_BASE);
	dacc_write_conversion_data(DACC_BASE, buf, DACC_CHANNEL);

}

// static void AFEC_Gain_callback(void){
// 	uint32_t gain;
// 	gain = afec_channel_get_value(AFEC1, AFEC_CHANNEL_PIN_GAIN);
// 	gain = convert_adc_to_gain(gain);
// 	xQueueSendFromISR( xQueueGain, &gain, NULL);
// }
// 
// static void AFEC_Saturation_callback(void){
// 	uint32_t saturation;
// 	saturation = afec_channel_get_value(AFEC1, AFEC_CHANNEL_PIN_SATURATION);
// 	saturation = convert_adc_to_saturation(saturation);
// 	xQueueSendFromISR( xQueueSaturation, &saturation, NULL);
// }
// 
// static void AFEC_Lowpass_callback(void){
// 	uint32_t lowpass;
// 	lowpass = afec_channel_get_value(AFEC1, AFEC_CHANNEL_PIN_LOWPASS);
// 	lowpass = convert_adc_to_lowpass(lowpass);
// 	xQueueSendFromISR( xQueueLowpass, &lowpass, NULL);
// }

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
   
	/*** Configuracao espec?fica do canal AFEC ***/
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

	/* Selecina canal e inicializa convers?o */  
	afec_channel_enable(AFEC0, AFEC_CHANNEL_PIN);
}

// static void config_AFEC_EFFECTS(void){
// /************************************* 
//    * Ativa e configura AFEC
//    *************************************/  
//   /* Ativa AFEC - 0 */
// 	afec_enable(AFEC1);
// 
// 	/* struct de configuracao do AFEC */
// 	struct afec_config afec_cfg;
// 
// 	/* Carrega parametros padrao */
// 	afec_get_config_defaults(&afec_cfg);
// 
// 	/* Configura AFEC */
// 	afec_init(AFEC1, &afec_cfg);
//   
// 	/* Configura trigger por software */
// 	afec_set_trigger(AFEC1, AFEC_TRIG_TIO_CH_0);
// 		
// 	AFEC1->AFEC_MR |= 3;
//   
// 	/* configura call back */
// 	afec_set_callback(AFEC1, AFEC_INTERRUPT_EOC_1,	AFEC_Gain_callback, 1);
// 	afec_set_callback(AFEC1, AFEC_INTERRUPT_EOC_3,	AFEC_Saturation_callback, 1);
// 	afec_set_callback(AFEC1, AFEC_INTERRUPT_EOC_5,	AFEC_Lowpass_callback, 1);
//    
// 	/*** Configuracao espec?fica do canal AFEC ***/
// 	struct afec_ch_config afec_ch_cfg;
// 	afec_ch_get_config_defaults(&afec_ch_cfg);
// 	afec_ch_cfg.gain = AFEC_GAINVALUE_0;
// 	afec_ch_set_config(AFEC1, AFEC_CHANNEL_PIN_GAIN, &afec_ch_cfg);
// 	afec_ch_set_config(AFEC1, AFEC_CHANNEL_PIN_SATURATION, &afec_ch_cfg);
// 	afec_ch_set_config(AFEC1, AFEC_CHANNEL_PIN_LOWPASS, &afec_ch_cfg);
//   
// 	/*
// 	* Calibracao:
// 	* Because the internal ADC offset is 0x200, it should cancel it and shift
// 	 down to 0.
// 	 */
// 	//afec_channel_set_analog_offset(AFEC0, AFEC_CHANNEL_PIN, 0x1B0);
// 	afec_channel_set_analog_offset(AFEC1, AFEC_CHANNEL_PIN_GAIN, 0x200);
// 	afec_channel_set_analog_offset(AFEC1, AFEC_CHANNEL_PIN_SATURATION, 0x200);
// 	afec_channel_set_analog_offset(AFEC1, AFEC_CHANNEL_PIN_LOWPASS, 0x200);
// 
// 	/***  Configura sensor de temperatura ***/
// 	struct afec_temp_sensor_config afec_temp_sensor_cfg;
// 
// 	afec_temp_sensor_get_config_defaults(&afec_temp_sensor_cfg);
// 	afec_temp_sensor_set_config(AFEC1, &afec_temp_sensor_cfg);
// 
// 	/* Selecina canal e inicializa convers?o */  
// }

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

	/** Configura o TC para operar em  4Mhz e interrup?c?o no RC compare */
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

// void BUT_init(void){
// 	/* config. pino botao em modo de entrada */
// 	pmc_enable_periph_clk(BUT_PIO_ID);
// 	pio_set_input(BUT_PIO, BUT_PIN_MASK, PIO_PULLUP | PIO_DEBOUNCE);
// 
// };

void task_adc_to_dac(void) {
	
	config_ADC_AUDIO();
	
	TC_init(TC0, ID_TC0, 0, 22000);

	config_DAC();
	
	saturation_value = 0;
	gain_value = 50;
	lowpass_value = 10000;
	
	while(1) {
		/*vTaskDelay( 10 / portTICK_PERIOD_MS);*/
	}
}

// void task_effects_controller(void) {
// 	
// 	xQueueGain = xQueueCreate( 100, sizeof( uint32_t ) );
// 	xQueueSaturation = xQueueCreate( 100, sizeof( uint32_t ) );
// 	xQueueLowpass = xQueueCreate( 100, sizeof( uint32_t ) );
// 	
// 	uint32_t gain = 0;
// 	uint32_t saturation = 0;
// 	uint32_t lowpass = 0;
// 	config_AFEC_EFFECTS();
// 	printf("Teste\n");
// 	afec_channel_enable(AFEC1, AFEC_CHANNEL_PIN_GAIN);
// 	afec_start_software_conversion(AFEC1);
// 	printf("Teste: %d\n", gain);
// 	
// 	while(1) {
// 		/*vTaskDelay( 10 / portTICK_PERIOD_MS);*/
// 		if (xQueueReceive( xQueueGain, &(gain), ( TickType_t )  10 / portTICK_PERIOD_MS)) {
// 			printf("Gain: %d\n", gain);
// 			afec_channel_enable(AFEC1, AFEC_CHANNEL_PIN_SATURATION);
// 			afec_start_software_conversion(AFEC1);
		/*}*/
// 		if (xQueueReceive( xQueueSaturation, &(saturation), ( TickType_t )  10 / portTICK_PERIOD_MS)) {
// 			printf("Saturation: %d\n", saturation);
// 			afec_channel_enable(AFEC1, AFEC_CHANNEL_PIN_LOWPASS);
// 			afec_start_software_conversion(AFEC1);
		/*}*/
// 		if (xQueueReceive( xQueueLowpass, &(lowpass), ( TickType_t )  10 / portTICK_PERIOD_MS)) {
// 			printf("Lowpass: %d\n", lowpass);
// 			afec_channel_enable(AFEC1, AFEC_CHANNEL_PIN_GAIN);
// 			afec_start_software_conversion(AFEC1);
		/*}*/
// 	}
// }


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
		printf("Failed to create test led task\r\n");
	}
	
// 	if (xTaskCreate(task_effects_controller, "effects", TASK_EFFECTS_STACK_SIZE, NULL, TASK_EFFECTS_STACK_PRIORITY, NULL) != pdPASS) {
// 		printf("Failed to create task_effects_controller task\r\n");
// 	}
	
	vTaskStartScheduler();
	

	while (1) {
		
	}
}
