#ifndef __APP_H__
#define __APP_H__
#include <vector>
#include <list>
#include "Log.h"
#include "Camera.h"
#include "Root.h"
#include "MyGameEngine/Mesh.h"

#include <chrono>

#pragma once

class  Module;
class Window;
class Input;
class HardwareInfo;
class MyGUI;
class Root;

class Mesh;

class App
{
public:

	App();
	~App();

	bool Awake();
	bool Start();
	bool DoUpdate();

	void PrepareUpdate();
	bool PreUpdate();

	bool Update();

	bool PostUpdate();
	void FinishUpdate();

	bool CleanUP();

	std::vector<LogInfo> GetLogs();
	void AddLog(LogType type, const char* entry);
	void CleanLogs();

	int GetFps() const;
	void SetFpsCap(int fps);

	// Add a new module to handle
	void AddModule(Module* module, bool activate);

	Window* window = nullptr;
	Input* input = nullptr;
	HardwareInfo* hardwareInfo = nullptr;
	MyGUI* gui = nullptr;
	Root* root = nullptr;

	Mesh ElMesh;

private:

	LogInfo logInfo;
	std::vector<LogInfo> logs;

	std::list<Module*> modules;

	std::chrono::time_point<std::chrono::steady_clock> frameStart, frameEnd;
	double dt = 0.0;
	int fps = 0;
	int frameCount = 0;
	double dtCount = 0.0;
};

extern App* Application;



#endif	// !__APP_H__