#ifndef GIZMOS_H
#define GIZMOS_H
#pragma once
#include "Module.h"
#include "Transform.h"


class Gizmos : public Module {

public:

	Gizmos(App* app);

	virtual ~Gizmos() = default;

	bool FixedUpdate();

	bool Start();

	bool Awake();

	bool Update(double dt);

	void DrawGizmos();

	void ProcessMousePicking();
	
	bool IntersectAxis(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, glm::vec3& outAxis);

	bool IntersectPlanes(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, glm::vec3& outAxis);

	void MoveObjectAlongAxis(const glm::vec3& rayOrigin, glm::vec3& rayDirection, const glm::vec3& axis);

	void MoveObjectAlongPlane(const glm::vec3& rayOrigin, glm::vec3& rayDirection, const glm::vec3& axis);

	glm::vec3 sphere1;
	glm::vec3 sphere2;
	glm::vec3 sphere3;

private:

	glm::vec3 selectedAxis = glm::vec3(0,0,0);
	glm::vec3 selectedPlane = glm::vec3(0, 0, 0);
	bool isAxisSelected = false;
	glm::vec3 rayStartPos;
	glm::vec3 rayDir;

	void DrawMovementArrows();
	void DrawAxisPlanesMovement();
};

#endif