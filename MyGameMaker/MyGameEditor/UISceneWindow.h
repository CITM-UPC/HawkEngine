#ifndef __UI_SCENE_WINDOW_H__
#define __UI_SCENE_WINDOW_H__
#pragma once

#include "UIElement.h"
#include <vector>
#include <string>
#include "MyGameEngine/types.h"
#include "MyGameEngine/BoundingBox.h"

class UISceneWindow : public UIElement
{
public:
    UISceneWindow(UIType type, std::string name);
    ~UISceneWindow();
    void Init();
    bool Draw();
    vec2 winSize = vec2(0, 0);
    vec2 winPos = vec2(0, 0);
    vec2 winMousePos = vec2(0, 0);
    glm::vec3 gizmoOriginalScale = glm::vec3(1.0f);

    bool isFoucused = false;

    glm::vec3 ConvertMouseToWorldCoords(int mouse_x, int mouse_y, int screen_width, int screen_height, int window_x, int window_y);
    glm::vec3 GetMousePickDir(int mouse_x, int mouse_y, int screen_width, int screen_height, int window_x, int window_y);
    bool CheckRayAABBCollision(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const BoundingBox& bBox);
};
#endif // !__UI_SCENE_WINDOW_H__