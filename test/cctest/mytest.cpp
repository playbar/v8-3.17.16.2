#include "utils.h"  
#include "CloudApp.h"
   
// This function returns a new array with three elements, x, y, and z.
Handle<Array> NewPointArray(int x, int y, int z) 
{
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	// We will be creating temporary handles so we use a handle scope.
	HandleScope handle_scope(isolate); //声明了HandleScope了

	// Create a new empty array.
	Handle<Array> array = Array::New(3);

	// Return an empty result if there was an error creating the array.
	if (array.IsEmpty())
		return Handle<Array>();

	// Fill out the values
	array->Set(0, Integer::New(x));
	array->Set(1, Integer::New(y));
	array->Set(2, Integer::New(z));
	// Return the value through Close.
	return handle_scope.Close(array); //这样才能正确返回一个本地handle
}


void InitialnilizeInterface(Handle<ObjectTemplate> global)
{  
    SetupCloudAppInterface(global);  
}  

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