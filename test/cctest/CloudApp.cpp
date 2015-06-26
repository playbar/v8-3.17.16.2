#include "CloudApp.h"


char *jsscritp =
"\
var app = new CloudApp( 4 ); \
app.start();\
app = null;"\
;


//char *jsscritp =  "\
//var skey = new SkeyApp();\
//var iid = CloudApp.TestId;\
//var app = new CloudApp( iid ); \
//var skeyid = skey.skeyid();\
//var appid = skey.appid();\
//var ca = CloudApp.CreateApp();\
//var caid = ca.appid();\
//var app2 = new CloudApp( 20); \
//var app3 = new CloudApp( 30);\
//app3.add(app2);\
//var id1 = app2.appid();\
//var apparray = [app2, app3];\
//var arrint = [1, 2 ];\
//var bo = app.IsEqual(apparray);\
//{\
//	var te = new CloudApp(9); \
//	te = null; \
//}\
//var id = app.appid(); \
//app.start();\
//app = null"	\
//;
//  