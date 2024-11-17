#ifndef __UI_HIERARCHY_H__
#define __UI_HIERARCHY_H__
#pragma once

#include "UIElement.h"
#include "MyGameEngine/HawkScene.h"
#include <vector>
#include <string>
#include <memory>

class UIHierarchy : public UIElement
{
public:
	UIHierarchy(UIType type, std::string name);
	~UIHierarchy();

	void RenderSceneHierarchy(std::shared_ptr<Scene>& currentScene);
	void DrawSceneObject(GameObject& obj);

	bool Draw();
};
#endif // !__UI_HIERARCHY_H__