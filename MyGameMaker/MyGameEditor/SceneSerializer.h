#pragma once
#include "Module.h"

#include <yaml-cpp/yaml.h>

class SceneSerializer : public Module
{
	
public:

	SceneSerializer(App* app) : Module(app) { ; }

	void Serialize();

	void DeSerialize();


};

