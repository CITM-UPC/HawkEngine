#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

void Transform::translate(const vec3& v)
{
	_mat = glm::translate(_mat, v);
}

void Transform::rotate(double rads, const vec3& v) 
{
	_mat = glm::rotate(_mat, rads, v);
}
void Transform::alignToGlobalUp(const vec3& worldUp) 
{
	vec3 fwd = glm::normalize(_fwd);
	vec3 right = glm::normalize(glm::cross(worldUp, fwd));
	vec3 up = glm::cross(fwd, right);

	_left = right;
	_up = up;
	_fwd = fwd;
	_pos = _pos;
	_mat = mat4(vec4(_left, 0.0f), vec4(_up, 0.0f), vec4(_fwd, 0.0f), vec4(_pos, 1.0f));
}
void Transform::LookAt(const vec3& target) 
{
    _fwd = glm::normalize(target - _pos);    
    _left = glm::normalize(glm::cross(vec3(0, 1, 0), _fwd));
    _up = glm::cross(_fwd, _left);          
   
    _mat[0] = vec4(_left, 0.0);
    _mat[1] = vec4(_up, 0.0);
    _mat[2] = vec4(_fwd, 0.0);
    _mat[3] = vec4(_pos, 1.0);
}