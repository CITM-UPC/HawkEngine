#pragma once

#include <string>
#include "GameObject.h"
#include <vector>
#include <iostream>

using namespace std;

class ModelImporter {
public:
	shared_ptr<GameObject> loadFromFile(const std::string& path);


	std::vector<std::shared_ptr<Mesh>> meshes;
};