#pragma once

#include <vector>
#include "GameObject.h"

class Scene
{
public:

	std::vector<std::shared_ptr<GameObject>> _children;


};

