team_drive_trash":"trash_query",a.Bq?[a.Bq]:[]).UC(1).Eda(1).Fq(tr(this,a)).J4(EFa(this,a)).Mn()};_.XIa=function(a){sr.call(this,a,_.jr)};_.t(_.XIa,sr);_.XIa.prototype.H=function(a){return this.W.ao("shared_with_me_query",[]).Mva().q3("me",0).Fq(tr(this,a)).J4(EFa(this,a)).Mn()};_.YIa=function(a){sr.call(this,a,_.UEa)};_.w(_.YIa,sr);_.YIa.prototype.H=function(a){return this.W.ao("starred_query",[]).isStarred(1).Fq(tr(this,a)).J4(EFa(this,a)).Mn()};_.Cs=new _.P(new _.Ml("Dialog"),"dialog-cancel",1190);_.Ds=function(a,b){_.dc.call(this);this.QA=a;this.message=(a=b||"")?a+" for operation "+this.QA.getType():"Error executing operation "+this.QA.getType()};_.w(_.Ds,_.dc);var ZIa=function(a){_.Ds.call(this,a,"Operation already executed")};_.w(ZIa,_.Ds);_.Es=function(){this.Ka=!1;this.W=null};_.Es.prototype.execute=function(a){if(this.Ka)return _.oi(new ZIa(this.Vb()));this.Ka=!0;this.W=a;return this.Id()};_.$Ia=function(a){_.y(this,a,0,-1,null,null)};_.w(_.$Ia,_.x);_.Fs=function(a){_.y(this,a,0,-1,null,null)};_.w(_.Fs,_.x);_.Gs=function(a){_.y(this,a,0,-1,null,null)};_.w(_.Gs,_.x);_.Fs.prototype.Zd=function(){return _.q(this,1)};_.Hs=function(a,b){_.D(a,1,b);return a};_.Fs.prototype.Od=function(){return _.q(this,2)};_.Is=function(a,b){_.D(a,2,b);return a};
var bJa;_.aJa=function(a){_.y(this,a,0,-1,null,_.Js)};_.w(_.aJa,_.x);var cJa=function(a){_.y(this,a,0,-1,bJa,null)};_.w(cJa,_.x);var dJa=function(a){_.y(this,a,0,-1,null,null)};_.w(dJa,_.x);_.Js=[[2,3,4,5,6,7,8,9,10]];bJa=[1];_.aJa.prototype.Qf=function(){return _.E(this,cJa,1)};
_.Ks=function(a){_.Es.call(this);this.Ha=a;this.xu=a.get(_.Dn)};_.w(_.Ks,_.Es);_.Ks.prototype.Na=function(){return this.Ha};_.eJa=function(a,b){return b.submit(a.Na(),_.Eq(_.eDa(a.W.H)))};var fJa=function(){return(0,_.S)('<span class="'+_.W("a-la-ua-x")+'"></span>')},gJa=function(a){return(0,_.S)("<span>"+_.T(a.errorMessage)+"</span>")};var hJa=function(a,b,c,d,e,f){this.W=a||"d";this.ma=b||{};this.JM=c||2;this.ra=d||21;this.ua=null!=e?e:!0;this.ta=f||"l"};_.g=hJa.prototype;_.g.getContent=function(){var a=_.Zl(fJa);a.appendChild(this.H());return a};_.g.isError=function(){return"e"==this.W};_.g.getType=function(){return this.W};_.g.getPosition=function(){return this.ta};_.g.bp=function(){return _.$b(this)};
_.Ls=function(a,b){this.template=a;this.data=b};_.Ms=function(a,b,c,d,e,f,h){hJa.call(this,b,c,d,e,f,h);this.ha=a};_.w(_.Ms,hJa);_.Ms.prototype.H=function(){return _.Yl(this.ha.template,this.ha.data)};_.jJa=function(a,b,c,d,e,f,h){_.Ms.call(this,c?c:new _.Ls(gJa,{errorMessage:b}),"e",_.iJa(a,d,e),f,e,h);this.Ha=a;this.wa=d};_.t(_.jJa,_.Ms);_.jJa.prototype.H=function(){return this.ka(_.Ms.prototype.H.call(this),this.wa)};_.jJa.prototype.ka=function(a){return a};_.iJa=function(){return{}};
_.Ja=function(a){this.Ha=a;this.ha="Ocorreu um erro";this.W=this.H=this.ka=this.ra=null;this.ma=!0};_.Ia=function(a,b){a.ha=b;return a};_.kJa=function(a,b){a.ra=b;return a};_.Ja.prototype.je=function(a){this.ka=a;return this};_.Ha=function(a,b){a.H=b;return a};_.Ga=function(a,b){a.W=b;return a};_.lJa=function(a){a.ma=!1;return a};_.Fa=function(a){return new _.jJa(a.Ha,a.ha,a.ra,a.ka,a.H,a.W,a.ma)};
_.mJa=function(){this.ha=!0;this.Ba=!1;this.ra=this.ua=this.ta=this.yz=!0;this.Sa=this.Oa=this.ma=this.ka=!1};_.Ns=new _.mJa;_.Ns.id="root";_.Ns.name="Meu Drive";_.Ns.W="owner";_.Ns.km=!1;_.Ns.Ia="https://drive.google.com/drive/my-drive";_.Ns.Ka="//ssl.gstatic.com/docs/doclist/images/icon_11_collection_list.png";_.Ns.wa="";
_.nJa=function(a){_.dc.call(this,a||"network unavailable")};_.w(_.nJa,_.dc);_.Os=new _.Ml("DriveDocs");_.oJa=new _.P(_.Os,"redo",83);_.pJa=new _.P(_.Os,"undo",113);_.qJa=new _.K("dZ0hH");_.rJa=new _.K("gKCm6e");_.sJa=new _.K("EcaKcf");_.tJa=function(){_.Gq.call(this,_.pJa,_.sJa,_.bo)};_.t(_.tJa,_.Gq);var uJa=function(){return(0,_.S)("<span>Em andamento\u2026</span>")};_.Ps=function(a,b,c,d){_.Ks.call(this,a);this.lc=b;this.Hc=c;this.ha=a.get(_.Ea);this.ta=a.get(_.qJa);this.Cb=d;this.ra=null;this.nb=this.ua=!1};_.w(_.Ps,_.Ks);_.g=_.Ps.prototype;_.g.Vb=function(){return this.lc};_.g.Oq=function(){return new _.Ls(uJa,{})};_.g.lk=function(){return null};_.g.vC=function(){return null};_.g.nF=function(){return this.ua?{undo:(0,_.u)(this.Pc,this)}:null};
_.g.execute=function(a){var b=this;this.Cb&&this.ho();a=_.Ps.Ea.execute.call(this,a);a.then(function(c){c=b.lk(c);b.Eu();var d=null!=b.nF()?2:1;c&&(b.ra=b.ha.H(new _.Ms(c,"d",b.nF(),d)))}).then(null,function(c){if(!(c instanceof _.ii||c instanceof _.$h||c instanceof _.nJa||"iid"==c)){var d=new _.Ja(b.Na()),e=b.vC(c);e&&_.kJa(d,e);d.je(c instanceof Error?c:Error(c));_.Ha(d,b.Hc);b.Eu();b.ra=b.ha.H(_.Fa(d))}});return this.lA(a)};_.g.lA=function(a){var b=new _.hi;_.ji(a,b.callback.bind(b));return b};
_.g.ho=function(){if(!this.nb){var a=this.Oq();a&&(this.nb=!0,this.ra=this.ha.H(new _.Ms(a,"p",null,3)))}};_.g.Eu=function(){null!=this.ra&&(this.ha.W(this.ra),this.ra=null)};_.Qs=function(a,b,c){a.ua=!0;var d=c();d.Va(function(){this.ta.oha({uza:this.Vb(),hka:b,Cha:c})}.bind(a));return d};_.Ps.prototype.G2=function(){return _.mi(!0)};_.vJa=function(a,b){b=b();b.Va(function(){this.ta.LBa()}.bind(a));return b};_.Ps.prototype.Pc=function(){this.Eu();_.eJa(this,new _.tJa)};
_.Rs=function(a,b,c,d){_.Ps.call(this,a,b,c,d);this.H=a.get(_.Oa);this.yh={UL:[],Be:0,rf:0,mP:0,gD:0,kx:0}};_.w(_.Rs,_.Ps);var wJa=new _.Ml("Offline");_.xJa=new _.P(wJa,"offline-enable",2763);_.yJa=new _.P(wJa,"offline-disable",2764);_.zJa=new _.P(wJa,"offline-docs-missing-extension-promo",91084);_.AJa=new _.P(wJa,"offline-extension",2710);_.BJa=new _.P(wJa,"offline-missing-extension-shown",91085);_.CJa=new _.P(_.Os,"offline-missing-extension",91016);_.DJa=new _.P(_.Os,"offline-pin-blob",91114);_.EJa=new _.P(_.Os,"offline-pin-gdoc",91115);_.FJa=new _.P(_.Os,"offline-pin-items",91065);_.GJa=new _.P(_.Os,"offline-preview",91068); _.HJa=new _.P(_.Os,"offline-preview-menu-",91069);
_.Ss=new _.K("an61Xb");_.IJa=new _.K("mrxZud");_.JJa=new _.K("YAg4U");_.KJa=new _.K("CIX3pe");_.LJa=new _.K("yZaXld");_.MJa=new _.K("i2Mnsf","Z8OyKc");_.NJa=new _.K("BNXlHd","Re1uYe",[_.Ss]);_.OJa=new _.K("Uolhrf","account",[_.fn]);_.PJa=new _.K("QxFe3b");_.QJa=new _.K("Yppsyd","Yppsyd",[_.Dn,_.Na]);_.RJa=new _.K("FMhWyb","settings",[_.po,_.qo,_.Zwa,_.Fo,_.PJa,_.Fq,_.qJa,_.Vr,_.Yr,_.Oa,_.eo,_.zj,_.IJa,_.Xn,_.QJa,_.Dn,_.Ea,_.Sp,_.Qp,_.hqa,_.Nm,_.Na,_.Rj]);var Ts,tKa;Ts=new _.Ml("DriveSettings");_.SJa=new _.P(Ts,"clear-local-state-dialog",1722);_.TJa=new _.P(Ts,"data-service-dialog",1246);_.UJa=new _.P(Ts,"data-service-shutdown-dialog",2821);_.VJa=new _.P(Ts,"dapper-dialog",1695);new _.P(Ts,"debug-dialog",1251);_.WJa=new _.P(Ts,"disable-photos-in-my-drive",1302);_.XJa=new _.P(Ts,"edit-prefs",2702);_.YJa=new _.P(Ts,"enable-photos-in-my-drive",1299);_.ZJa=new _.P(Ts,"experimentz-window",2822);new _.P(Ts,"feedback-dialog",1247);
_.$Ja=new _.P(Ts,"hangouts-off",91041);_.aKa=new _.P(Ts,"hangouts-on",91042);_.bKa=new _.P(Ts,"hangouts-chime-request",91043);_.cKa=new _.P(Ts,"hangouts-install-request",91028);new _.P(Ts,"help-dialog",1245);_.dKa=new _.P(Ts,"item-lookup-dialog",2059);_.eKa=new _.P(Ts,"manage-apps",1310);_.fKa=new _.P(Ts,"memory-info-dialog",1694);_.gKa=new _.P(Ts,"notification-settings",91033);_.hKa=new _.P(Ts,"offline-settings",1311);_.iKa=new _.P(Ts,"report-data-issue",2747);
_.jKa=new _.P(Ts,"request-history-dialog",2098);_.kKa=new _.P(Ts,"prefs-disable-convert",2620);_.lKa=new _.P(Ts,"prefs-disable-people-predict",91026);_.mKa=new _.P(Ts,"prefs-disable-priority-as-home-view",91066);_.nKa=new _.P(Ts,"prefs-disable-quick-access",91002);_.oKa=new _.P(Ts,"prefs-enable-convert",2619);_.pKa=new _.P(Ts,"prefs-enable-people-predict",91027);_.qKa=new _.P(Ts,"prefs-enable-priority-as-home-view",91067);_.rKa=new _.P(Ts,"prefs-enable-quick-access",91003); _.sKa=new _.P(Ts,"reset-promos-dialog",1248);tKa=new _.P(Ts,"settings-dialog",790);new _.P(Ts,"show-shortcuts",2485);_.uKa=new _.P(Ts,"startup-timing-dialog",1249);_.vKa=new _.P(Ts,"task-info-dialog",1250);_.wKa=new _.P(Ts,"upload-info-dialog",2415);
_.Us=function(a,b,c,d){_.Gq.call(this,a||tKa,c||_.RJa,d||_.bo);this.H=b||"fa"};_.t(_.Us,_.Gq);if(window.jstiming){window.jstiming.Wma={};window.jstiming.tib=1;var xKa=function(a,b,c){var d=a.t[b],e=a.t.start;if(d&&(e||c))return d=a.t[b][0],void 0!=c?e=c:e=e[0],Math.round(d-e)};window.jstiming.getTick=xKa;window.jstiming.getLabels=function(a){var b=[],c;for(c in a.t)b.push(c);return b};var yKa=function(a,b,c){var d="";window.jstiming.srt&&(d+="&srt="+window.jstiming.srt);window.jstiming.pt&&(d+="&tbsrt="+window.jstiming.pt);try{window.external&&window.external.tran?d+="&tran="+window.external.tran:
window.gtbExternal&&window.gtbExternal.tran?d+="&tran="+window.gtbExternal.tran():window.chrome&&window.chrome.csi&&(d+="&tran="+window.chrome.csi().tran)}catch(v){}var e=window.chrome;if(e&&(e=e.loadTimes)){e().wasFetchedViaSpdy&&(d+="&p=s");if(e().wasNpnNegotiated){d+="&npn=1";var f=e().npnNegotiatedProtocol;f&&(d+="&npnv="+(encodeURIComponent||escape)(f))}e().wasAlternateProtocolAvailable&&(d+="&apa=1")}var h=a.t,k=h.start;e=[];f=[];for(var m in h)if("start"!=m&&0!=m.indexOf("_")){var n=h[m][1];
n?h[n]&&f.push(m+"."+xKa(a,m,h[n][0])):k&&e.push(m+"."+xKa(a,m))}if(b)for(var r in b)d+="&"+r+"="+b[r];(b=c)||(b="https:"==document.location.protocol?"https://csi.gstatic.com/csi":"http://csi.gstatic.com/csi");return[b,"?v=3","&s="+(window.jstiming.sn||"")+"&action=",a.name,f.length?"&it="+f.join(","):"",d,"&rt=",e.join(",")].join("")},zKa=function(a,b,c){a=yKa(a,b,c);if(!a)return"";b=new Image;var d=window.jstiming.tib++;window.jstiming.Wma[d]=b;b.onload=b.onerror=function(){window.jstiming&&delete window.jstiming.Wma[d]};
b.src=a;b=null;return a};window.jstiming.report=function(a,b,c){var d=document.visibilityState,e="visibilitychange";d||(d=document.webkitVisibilityState,e="webkitvisibilitychange");if("prerender"==d){var f=!1,h=function(){if(!f){b?b.prerender="1":b={prerender:"1"};if("prerender"==(document.visibilityState||document.webkitVisibilityState))var k=!1;else zKa(a,b,c),k=!0;k&&(f=!0,document.removeEventListener(e,h,!1))}};document.addEventListener(e,h,!1);return""}return zKa(a,b,c)}}
;var Vs;Vs=new _.Ml("rkqxHb");_.AKa=new _.P(Vs,"ZMvdJc",91061);_.BKa=new _.P(Vs,"D46kPe",91105);_.CKa=new _.P(Vs,"h3N5pc",91093);_.DKa=new _.P(Vs,"V6dbSe",91094);_.EKa=new _.P(Vs,"rW3Qod",91095);_.FKa=new _.P(Vs,"tO6Oee",91096);_.GKa=new _.P(Vs,"afYede",91100);_.HKa=new _.P(Vs,"w5ntsf",91062);_.IKa=new _.P(Vs,"ZguaJf",91106);_.JKa=new _.P(Vs,"pzuEhf",91052);
_.KKa=new _.K("W0LoLb","VrIF5e");_.LKa=new _.K("IzyCSc","nDABpd");_.MKa=new _.K("c4ahyd","ykvETe");_.NKa=new _.K("BHkP0b","clNEWc");_.OKa=new _.K("ukNdCc","JwoHec");_.PKa=function(){this.H=new Map;this.W=new Set};_.Ws=function(a,b,c,d){a.H.set(b,{WTa:c,Dib:d||_.iwa})};_.Xs=new _.PKa;_.Ws(_.Xs,_.AKa,_.KKa);_.Ws(_.Xs,_.BKa,_.LKa);_.Ws(_.Xs,_.GKa,_.MKa);_.Ws(_.Xs,_.HKa,_.NKa);_.Ws(_.Xs,_.JKa,_.OKa);_.Ws(_.Xs,gDa,_.DDa);_.Ws(_.Xs,_.hDa,_.EDa);_.Ws(_.Xs,jDa,_.FDa);_.Ws(_.Xs,_.lDa,_.GDa);_.Ws(_.Xs,_.mDa,_.HDa);_.Ws(_.Xs,_.nDa,_.IDa);_.Ws(_.Xs,oDa,_.JDa);_.Ws(_.Xs,_.pDa,_.KDa);var QKa,RKa,SKa,TKa,UKa,WKa,XKa,eLa;QKa=new _.P($r,"browseApps",793);RKa=new _.P($r,"create",794);SKa=new _.P($r,"create-document",61);TKa=new _.P($r,"create-drawing",62);UKa=new _.P($r,"create-form",63);_.VKa=new _.P($r,"create-instant-site",2519);WKa=new _.P($r,"create-presentation",66);XKa=new _.P($r,"create-spreadsheet",68);_.YKa=new _.P($r,"delete-app-data",1111);_.ZKa=new _.P($r,"driveClientPromo",1894);_.$Ka=new _.P($r,"driveClientPromoDownload",1906);_.aLa=new _.P($r,"download-drive",1178); _.bLa=new _.P($r,"open",784);_.cLa=new _.P($r,"open-with-native-app",1290);_.dLa=new _.P($r,"open-with-menu-native-apps-loaded",1696);eLa=new _.P($r,"open-template-gallery",2836);_.fLa=new _.P($r,"updateUseByDefault",1063);
_.gLa=new _.K("Gh4Spc","apps",[_.Fo]);_.Ys=function(){this.tw=new _.Ep;this.Th=this.ma=!1};_.w(_.Ys,epa);_.Zs=function(a,b){b.listen(a.Sla,a)};_.hLa=function(a){a.ma=!1;a.Th&&(a.Th=!1,a.Sla())};_.Ys.prototype.Sla=function(){this.ma?this.Th=!0:this.tw.H(null)};_.Ys.prototype.listen=function(a,b){this.tw.listen(a,b)};_.Ys.prototype.kl=function(a){this.tw.kl(a)};_.Ys.prototype.zi=function(a){new qk(a,[this])};
_.iLa=function(a){return _.q(a,50).some(function(b){return"machineRoot"==b})};_.jLa=function(a){return _.q(a,50).some(function(b){return"externalMedia"==b})};_.$s=function(a){_.y(this,a,0,-1,null,null)};_.w(_.$s,_.x);_.$s.prototype.getType=function(){return _.q(this,1)};_.at=function(a,b){_.D(a,1,b)};_.g=_.$s.prototype;_.g.Vs=function(){return _.q(this,2)};_.g.getMessage=function(){return _.q(this,3)};_.g.Vh=function(a){_.D(this,3,a)};_.g.ky=function(){return _.q(this,5)};_.g.fsa=function(){return _.C(this,6,!1)};_.g.iA=function(){return _.q(this,7)};_.g.BW=function(a){_.D(this,7,a)};_.g.npa=function(){return _.C(this,8,!1)};
_.g.FZ=function(){return _.q(this,9)};_.g.Tl=function(){return _.q(this,10)};_.g.y$=function(){return _.q(this,11)};_.g.wDa=function(a){_.D(this,11,a)};_.g.RWa=function(){return _.q(this,12)};_.g.Haa=function(){return _.C(this,13,!1)};_.g.$z=function(){return _.q(this,14)};_.g.XH=function(){return _.q(this,15)};_.g.ZE=function(){return _.q(this,16)};_.g.rW=function(a){_.D(this,16,a)};_.g.VVa=function(){return _.C(this,17,!1)};_.g.tl=function(){return _.q(this,18)};
_.bt=function(a,b,c){if(b=b?_.bf(b):void 0){var d=_.q(b,4);d&&(d=d.replace(/\\n\\tat/g,"\n  at"),d=d.replace(/\\n/g,"\n"),_.D(b,4,d))}d="ReportableError"+(a?": "+a:"")+(b?"\n"+kLa(b).join("\n"):"")+(c?"\n"+("Original error: "+c+(c.stack?"\nOriginal error stack: "+c.stack.replace(/\n/g,"\n>"):"")):"");_.dc.call(this,d);this.Lm=b;this.ha=c;this.H=a||"";this.Lm&&this.Lm.npa()&&(this.stack="")};_.t(_.bt,_.dc);
var kLa=function(a){var b=[];_.Rc(lLa,function(c,d){c=c.call(a);if(_.Nb(c)){if(mLa[d]){var e=mLa[d][c];c=_.Nb(e)?e:"<unknown value: "+String(c)+">"}b.push(d+": "+String(c))}});_.q(a,4)&&b.push("\n"+_.q(a,4));return b},ct=_.$s.prototype,lLa={Type:ct.getType,Cause:ct.Vs,Message:ct.getMessage,DataType:ct.ky,HideFromUser:ct.fsa,UploadId:ct.iA,IsExternal:ct.npa,NumRetries:ct.FZ,ServerErrorReason:ct.Tl,UploadType:ct.y$,PrefetchedItemId:ct.RWa,ConflictRecoveryFailure:ct.Haa,ClientErrorReason:ct.$z,HttpResponseCode:ct.XH,
DapperTraceUrl:ct.ZE,HideFromServer:ct.VVa,Reason:ct.tl},mLa={Type:{1:"LOAD",2:"RUNTIME",3:"LOAD_FATAL"},Cause:{1:"UNAVAILABLE_OFFLINE",2:"AUTHORIZATION",3:"UNSUPPORTED",4:"PERMISSION",5:"NONEXISTENT",6:"UNKNOWN",7:"BAD_REQUEST",8:"SERVER_ERROR",9:"CONFLICT",10:"RESPONSE_TOO_LARGE",11:"RESPONSE_PARSE_ERROR",12:"REQUEST_BUILD_ERROR",13:"USER_QUOTA_EXCEEDED",14:"USER_RATE_LIMIT_EXCEEDED"},DataType:{1:"ACCOUNT",2:"APPS",6:"APPS_HAS_APP_DATA",3:"APPS_HAS_APP_DATA_AND_QUOTA",4:"USER_PREFS",5:"ITEMS"},
ServerErrorReason:{15:"SERVER_AUTH_ERROR",9:"BACKEND_ERROR",14:"BAD_CONTENT",11:"BAD_REQUEST",45:"CANNOT_MOVE_TRASHED_ITEM_INTO_TEAM_DRIVE",8:"CONFLICT",16:"CONVERSION_INPUT_CORRUPT",18:"CONVERSION_INPUT_TOO_LARGE",30:"CONVERSION_INPUT_TYPE_UNSUPPORTED",20:"CONVERSION_INVALID_LANGUAGE",19:"CONVERSION_INVALID_REQUEST",29:"CONVERSION_LANGUAGE_UNSUPPORTED",17:"CONVERSION_MIMETYPE_MISMATCH",22:"CONVERSION_OUTPUT_TOO_LARGE",28:"CONVERSION_OUTPUT_TYPE_UNSUPPORTED",21:"CONVERSION_PASSWORD_PROTECTED",24:"CONVERSION_TIMEOUT",
25:"CONVERSION_TOO_MANY_CHARTS",26:"CONVERSION_TOO_MANY_DRAWINGS",27:"CONVERSION_TOO_MANY_IMAGES",23:"CONVERSION_USER_RATE_QUOTA_EXCEEDED",31:"DEADLINE_EXCEEDED",0:"DEFAULT_REASON",42:"FILE_OWNER_NOT_MEMBER_OF_TEAM_DRIVE",43:"FILE_OWNER_NOT_MEMBER_OF_TEAM_DRIVE_DOMAIN",47:"FILE_OWNER_NOT_MEMBER_OF_WRITER_DOMAIN",46:"FILE_WRITER_TEAM_DRIVE_MOVE_IN_DISABLED",10:"FORBIDDEN",3:"INTERNAL_SERVER_ERROR",12:"INVALID",32:"LOGIN_REQUIRED",33:"MISSING_AUTH_COOKIE",1:"MISSING_REASON",4:"RESOURCE_NOT_FOUND",34:"RESPONSE_PREPARATION_FAILURE",
5:"RESPONSE_TOO_LARGE",35:"UPLOAD_BROKEN_CONNECTION",50:"SHARING_DLP_NO_SHARE_OUTSIDE_DOMAIN",52:"TEAM_DRIVE_CROSS_DOMAIN_MOVE_RESTRICTED",40:"TEAM_DRIVE_FILE_LIMIT_EXCEEDED",49:"TEAM_DRIVE_HIERARCHY_TOO_DEEP",48:"TEAM_DRIVE_MEMBERSHIP_REQUIRED",44:"TEAM_DRIVE_NAME_TOO_LONG",39:"TEAM_DRIVES_FILE_TYPE_NOT_SUPPORTED",36:"TEAM_DRIVES_FOLDER_MOVE_IN_NOT_SUPPORTED",37:"TEAM_DRIVES_FOLDER_MOVE_OUT_NOT_SUPPORTED",41:"TEAM_DRIVES_PARENT_LIMIT",38:"TEAM_DRIVES_SHORTCUT_FILE_NOT_SUPPORTED",53:"TOO_MANY_FILES",
54:"TOO_MANY_UNMOVABLE_FILES",13:"TRAN./src/ASF/sam/utils/cmsis/same70/include/instance/trng.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/xdmac.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/qspi.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/smc.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/sdramc.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/matrix.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/utmi.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/pmc.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/uart0.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/chipid.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/uart1.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/efc.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/pioa.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/piob.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/pioc.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/piod.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/pioe.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/rstc.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/supc.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/rtt.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/wdt.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/rtc.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/gpbr.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/rswdt.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/uart2.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/uart3.h:

../src/ASF/sam/utils/cmsis/same70/include/instance/uart4.h:

../src/ASF/sam/utils/cmsis/same70/include/pio/same70q21.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\stdio.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\_ansi.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\newlib.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\sys\config.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\machine\ieeefp.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\sys\cdefs.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\lib\gcc\arm-none-eabi\6.3.1\include\stdarg.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\sys\reent.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\_ansi.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\sys\_types.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\machine\_types.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\sys\lock.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\sys\types.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\machine\endian.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\machine\_endian.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\sys\select.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\sys\_sigset.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\sys\_timeval.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\sys\timespec.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\sys\_timespec.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\sys\_pthreadtypes.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\machine\types.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\sys\stdio.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\lib\gcc\arm-none-eabi\6.3.1\include\stdbool.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\stdlib.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\machine\stdlib.h:

c:\program\ files\ (x86)\atmel\studio\7.0\toolchain\arm\arm-gnu-toolchain\arm-none-eabi\include\alloca.h:

../src/ASF/common/utils/interrupt.h:

../src/ASF/common/utils/interrupt/interrupt_sam_nvic.h:
