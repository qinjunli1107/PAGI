#pragma once

#include "Graphics.h"
#include "Input.h"

class System
{
private:
	string m_appName;

	Graphics* m_Graphics;

	float currentTime;
	float lastTime;

public:
	System(string appName);
	~System();

	bool Initialize();
	void Run();
	void Shutdown();
};

