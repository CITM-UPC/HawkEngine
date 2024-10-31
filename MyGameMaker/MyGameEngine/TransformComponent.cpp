#include "TransformComponent.h"
#include "GameObject.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

Transform_Component::Transform_Component(std::weak_ptr<GameObject> owner) : Component(owner) {}

Transform_Component::Transform_Component(const Transform_Component& other) : Component(other)
{
	matrix = other.matrix;
	position = other.position;
	left = other.left;
	up = other.up;
	forward = other.forward;
}

Transform_Component& Transform_Component::operator=(const Transform_Component& other)
{
	if (this != &other)
	{
		Component::operator=(other);
		matrix = other.matrix;
		position = other.position;
		left = other.left;
		up = other.up;
		forward = other.forward;
	}
	return *this;
}

std::shared_ptr<Component> Transform_Component::Clone()
{
	auto clone = std::make_shared<Transform_Component>(*this);
	clone->matrix = this->matrix;
	clone->left = this->left;
	clone->up = this->up;
	clone->forward = this->forward;
	clone->position = this->position;
	return clone;
}

void Transform_Component::Translate(const glm::dvec3& translation)
{
	matrix = glm::translate(matrix, translation);
}

void Transform_Component::Rotate(double rads, const glm::dvec3& axis)
{
	matrix = glm::rotate(matrix, rads, axis);
}

void Transform_Component::Scale(const glm::dvec3& scale)
{
	matrix = glm::scale(matrix, scale);
}

void Transform_Component::LookAt(const glm::dvec3& target)
{
	matrix = glm::lookAt(position, target, up);
}