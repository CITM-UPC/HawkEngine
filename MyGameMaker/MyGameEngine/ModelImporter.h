#pragma once

#include <string>
#include "GameObject.h"
#include <vector>
#include <iostream>

using namespace std;

class ModelImporter {
public:
	void loadFromFile(const std::string& path);


	std::vector<std::shared_ptr<Mesh>> meshes;
};