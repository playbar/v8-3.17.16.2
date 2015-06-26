#include "CloudApp.h"


char *jsscritp =
"\
var app = new CloudApp( 4 ); \
app.start();\
app.testaa = 56;\
var te = app.testaa;\
console.log(te);\
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

//��MakeWeakע���callback.  
void CloudAppWeakReferenceCallback(Persistent<Value> object, void * param)
{
	if (CloudApp* cloudapp = static_cast<CloudApp*>(param))
	{
		delete cloudapp;
	}
}

//��C++ָ��ͨ��External����ΪPersistent���󣬱����ָ�뱻����  
Handle<External> MakeWeakCloudApp(void* parameter)
{
	Persistent<External> persistentCloudApp = Persistent<External>::New(External::New(parameter));

	//MakeWeak�ǳ���Ҫ����JS����newһ��CloudApp����֮��  
	//C++Ҳ����newһ����Ӧ��ָ�롣  
	//JS��������֮�������취ȥ����C++��ָ�룬����ͨ��MakeWeak��ʵ�֣�  
	//MakeWeak����ҪĿ����Ϊ�˼��Persistent Handle���˵�ǰPersistent   
	//��Ψһ�����⣬û�����������ã��Ϳ����������Persistent Handle�ˣ�  
	//ͬʱ����MakeWeak��callback���������ǿ��������callback��delete   
	//C++ָ��  
	persistentCloudApp.MakeWeak(parameter, CloudAppWeakReferenceCallback);

	return persistentCloudApp;
}

//��JS�������Ĳ�������֮�󣬴���C++����  
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

//�൱��JS��Ӧ�Ĺ��캯������JS��ʹ��new CloudApp��ʱ�����callback���Զ�������  
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
	if (!args.IsConstructCall())
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

	if (args[0]->IsArray())
	{
		int i = args.Length();
		i = 0;
	}
	Handle<Array> arr = Handle<Array>::Cast(args[0]);
	int size = arr->Length();
	CloudApp app[2];
	for (int i = 0; i < size; i++)
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

	//////////////////////////////////////////////////////////////////////////

	Handle<FunctionTemplate> skeyapp_class = SkeyApp_Class();
	global->Set(String::New("SkeyApp"), skeyapp_class);
}
