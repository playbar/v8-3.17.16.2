#ifndef __CLOUDAPP_H__
#define __CLOUDAPP_H__

#include "v8.h"  
using namespace v8;
#include <iostream>  
#include <string>  
using namespace std;

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
		if (app->appId == this->appId)
		{
			return true;
		}
		return false;
	}

	int getState() { return state; }
	int getAppId() { return appId; }

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

extern char *jsscritp;

void SetupCloudAppInterface(Handle<ObjectTemplate> global);

#endif


