


#include "v8.h"  
#include "utils.h"  
  
#include <iostream>  
#include <string>  
  
using namespace std;  
  
using namespace v8;  
  
enum AppState
{  
    IDEL = 0,  
    LOADED,  
    STOP  
};  
  
class CloudApp 
{  
public:  
	CloudApp* CreateApp();
	CloudApp()
	{
		state = IDEL;
		appId = 0;
	}
    CloudApp(int id)
	{   
        state = IDEL;  
        appId = id;  
    }  
	~CloudApp()
	{
		int i = 0;
	}

    void start()
	{  
        cout << "CloudApp been Loaded id = " << appId << endl;  
        state = LOADED;  
    };  
	bool IsEqual(CloudApp * app)
	{
		if ( app->appId == this->appId )
		{
			return true;
		}
		return false;
	}
  
    int getState() { return state;}  
    int getAppId() { return appId;}
      
private:  
    AppState state;  
    int appId;    
};  

class SkeyApp : public CloudApp
{
public:
	SkeyApp(){ skeyid = 234; };
	int getSkeyId() { return skeyid; }
private:
	int skeyid;
};
  
//向MakeWeak注册的callback.  
void CloudAppWeakReferenceCallback(Persistent<Value> object, void * param)
{  
    if (CloudApp* cloudapp = static_cast<CloudApp*>(param))
	{  
        delete cloudapp;  
    }  
}  
  
//将C++指针通过External保存为Persistent对象，避免的指针被析构  
Handle<External> MakeWeakCloudApp(void* parameter) 
{  
    Persistent<External> persistentCloudApp = Persistent<External>::New(External::New(parameter));  
          
//MakeWeak非常重要，当JS世界new一个CloudApp对象之后  
//C++也必须new一个对应的指针。  
//JS对象析构之后必须想办法去析构C++的指针，可以通过MakeWeak来实现，  
//MakeWeak的主要目的是为了检测Persistent Handle除了当前Persistent   
//的唯一引用外，没有其他的引用，就可以析构这个Persistent Handle了，  
//同时调用MakeWeak的callback。这是我们可以再这个callback中delete   
//C++指针  
    persistentCloudApp.MakeWeak(parameter, CloudAppWeakReferenceCallback);  
  
    return persistentCloudApp;  
}  
  
//将JS传进来的参数解析之后，创建C++对象  
CloudApp* NewCloudApp(const Arguments& args)
{  
    CloudApp* cloudApp = NULL;  
	int id = args[0]->Int32Value();
    if (args.Length() == 1) 
	{  
        cloudApp = new CloudApp(id);   
    } 
	else
	{  
        v8::ThrowException(String::New("Too many parameters for NewCloudApp"));  
    }  
  
    return cloudApp;  
}  
  
//相当于JS对应的构造函数，当JS中使用new CloudApp的时候，这个callback将自动被调用  
Handle<Value> CloudAppConstructCallback(const Arguments& args)
{  
    if (!args.IsConstructCall())  
        return Undefined();  
    CloudApp* cloudapp = NewCloudApp(args);  
    Handle<Object> object = args.This();  
    object->SetInternalField(0, MakeWeakCloudApp(cloudapp));  
    return Undefined();  
}  

Handle<Value> SkeyAppConstructCallback(const Arguments &args)
{
	if ( ! args.IsConstructCall( ))
	{
		return Undefined();
	}
	SkeyApp *skeyapp = new SkeyApp();
	Handle<Object> object = args.This();
	object->SetInternalField(0, MakeWeakCloudApp(skeyapp));
	return Undefined();
}
 
Handle<Value> JS_GetState(const Arguments& args) {  
    Handle<Object> self = args.Holder();  
  
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));  
    void* ptr = wrap->Value();  
    CloudApp* cloudapp = static_cast<CloudApp*>(ptr);  
 
    return Integer::New(cloudapp->getState());  
}  
  
Handle<Value> JS_GetAppId(const Arguments& args) {  
    Handle<Object> self = args.Holder();  
  
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));  
    void* ptr = wrap->Value();  
    CloudApp* cloudapp = static_cast<CloudApp*>(ptr);  
  
    return Integer::New(cloudapp->getAppId());  
}   

Handle<Value> JS_IsEqual(const Arguments &args)
{
	Handle<Object> self = args.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	CloudApp* cloudapp = static_cast<CloudApp*>(ptr);
	
	if ( args[0]->IsArray() )
	{
		int i = args.Length();
		i = 0;
	}
	Handle<Array> arr = Handle<Array>::Cast(args[0]);
	int size = arr->Length();
	CloudApp app[2];
	for (int i = 0; i < size; i++ )
	{
		Handle<Value> va = arr->Get(Integer::New(i));
		Handle<External> field = Handle<External>::Cast(va->ToObject()->GetInternalField(0));
		void *obj_ptr = field->Value();
		CloudApp *p = static_cast<CloudApp*>(obj_ptr);
		app[i] = *p;
	}

	cloudapp->IsEqual(app);

	//int ilen = va->InternalFieldCount();
	//Local<External> clwrap = Local<External>::Cast(va->GetInternalField(0));

	//CloudApp *p = (CloudApp*)args[0]->Int32Value();
	return Boolean::New(false);
}

Handle<Value> JS_Add(const Arguments &args)
{
	Handle<Object> self = args.Holder();
	return Undefined();
}
  
Handle<Value> JS_Start(const Arguments& args) {  
    Handle<Object> self = args.Holder();  
  
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));  
    void* ptr = wrap->Value();  
    CloudApp* cloudapp = static_cast<CloudApp*>(ptr);  
  
    cloudapp->start();  
  
    return Undefined();  
}  

Handle<Value>JS_SkeyID(const Arguments &args)
{
	Handle<Object> self = args.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	void *ptr = wrap->Value();
	SkeyApp * skeyApp = static_cast<SkeyApp*>(ptr);
	int id = skeyApp->getSkeyId();
	return Integer::New(id);
}

Handle<Value> JS_TestID(const Arguments &args)
{
	int iid = 23;
	return Integer::New(iid);
}


Handle<ObjectTemplate> ClouApp_Fun(Handle<FunctionTemplate> &cloudapp_class)
{
	Handle<ObjectTemplate> cloudapp_proto = cloudapp_class->PrototypeTemplate();
	cloudapp_proto->Set(String::New("start"), FunctionTemplate::New(JS_Start));
	cloudapp_proto->Set(String::New("state"), FunctionTemplate::New(JS_GetState));
	cloudapp_proto->Set(String::New("appid"), FunctionTemplate::New(JS_GetAppId));
	cloudapp_proto->Set(String::New("IsEqual"), FunctionTemplate::New(JS_IsEqual));
	cloudapp_proto->Set(String::New("add"), FunctionTemplate::New(JS_Add));	
	return cloudapp_proto;
}

Handle<FunctionTemplate> CloudApp_Class()
{
	Handle<FunctionTemplate> cloudapp_class = FunctionTemplate::New(CloudAppConstructCallback);
	cloudapp_class->SetClassName(String::New("CloudApp"));
	ClouApp_Fun(cloudapp_class);
	Handle<ObjectTemplate> cloudapp_inst = cloudapp_class->InstanceTemplate();
	cloudapp_inst->SetInternalFieldCount(1);
	return cloudapp_class;
}

Handle<ObjectTemplate> SkeyApp_Fun(Handle<FunctionTemplate> &skeyapp_class)
{
	Handle<ObjectTemplate> skeyapp_proto = skeyapp_class->PrototypeTemplate();
	skeyapp_proto->Set(String::New("skeyid"), FunctionTemplate::New(JS_SkeyID));
	return skeyapp_proto;
}
Handle<FunctionTemplate> SkeyApp_Class()
{
	Handle<FunctionTemplate> skeyapp_class = FunctionTemplate::New(SkeyAppConstructCallback);
	skeyapp_class->SetClassName(String::New("SkeyApp"));
	ClouApp_Fun(skeyapp_class);
	SkeyApp_Fun(skeyapp_class);
	Handle<ObjectTemplate> skeyapp_inst = skeyapp_class->InstanceTemplate();
	skeyapp_inst->SetInternalFieldCount(1);
	return skeyapp_class;
}
  
Handle<Value> JS_CreateApp(const Arguments &args)
{
	int argc = 1;
	Handle<Value> argv[] = { args[0] };
	return CloudApp_Class()->GetFunction()->NewInstance(argc, argv);
}



void SetupCloudAppInterface(Handle<ObjectTemplate> global) 
{  
	Handle<FunctionTemplate> app_class = CloudApp_Class();
	app_class->Set(String::New("CreateApp"), FunctionTemplate::New(JS_CreateApp));
	app_class->ReadOnlyPrototype();
	app_class->Set(String::New("TestId"), Number::New(23));

	global->Set(String::New("CloudApp"), app_class);

	Handle<FunctionTemplate> skeyapp_class = SkeyApp_Class();
	global->Set(String::New("SkeyApp"), skeyapp_class );
}  
  
void InitialnilizeInterface(Handle<ObjectTemplate> global)
{  
    SetupCloudAppInterface(global);  
}  

char *jsscritp =  "\
CloudApp.TestId = 24;\
var iid = 25;\
var app = new CloudApp( iid ); \
app.start();\
app = null"	\
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
void LoadJsAndRun()
{  
	Handle<String> source = String::New( jsscritp );  
    Handle<Script> script = Script::Compile(source);  
    Handle<Value> result = script->Run();  
  
    //printValue(result);  
}  
  
void Regist2JsContext(Handle<ObjectTemplate>& object, Persistent<Context>& context)
{  
    context = Context::New(NULL, object);  
}  
 

Handle<Value> Yell(const Arguments& args) {  
	HandleScope  handle_scope;  
	char buffer[4096];  

	memset(buffer, 0, sizeof(buffer));  
	Handle<String> str = args[0]->ToString();  
	str->WriteAscii(buffer);  
	printf("Yell: %s\n", buffer);  

	return Undefined();  
}  

int main2(int argc, char** argv) 
{  
	HandleScope handle_scope;  

	//A  
	Handle<FunctionTemplate> fun = FunctionTemplate::New(Yell);  

	//B  
	Handle<ObjectTemplate> global = ObjectTemplate::New();  
	global->Set(String::New("yell"), fun);  

	//C  
	Persistent<Context> cxt = Context::New(NULL, global);  

	Context::Scope context_scope(cxt);  
	Handle<String> source = String::New("yell('Google V8!')");  
	Handle<Script> script = Script::Compile(source);  
	Handle<Value> result = script->Run();  

	cxt.Dispose(); 
	return 0;
}  


int main(int argc, char** argv)
{  
    HandleScope handle_scope;  
    Handle<ObjectTemplate> global = ObjectTemplate::New();  
    Persistent<Context> context;  
      
    InitialnilizeInterface(global);  
    Regist2JsContext(global, context);  
    Context::Scope context_scope(context);  
    LoadJsAndRun();  
  
    context.Dispose();  
	
	V8::LowMemoryNotification();
      
    return 0;  
}  