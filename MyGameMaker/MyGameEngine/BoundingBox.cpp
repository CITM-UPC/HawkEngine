#include "BoundingBox.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

void drawWiredQuad(const vec3& v0, const vec3& v1, const vec3& v2, const vec3& v3);

BoundingBox::BoundingBox(const vec3* vertices, size_t num_verts) {
	min = max = vertices[0];
	for (size_t i = 1; i < num_verts; ++i) {
		min = glm::min(min, vertices[i]);
		max = glm::max(max, vertices[i]);
	}
}

bool BoundingBox::contains(const vec3& point) const {
	return (point.x >= min.x && point.x <= max.x) &&
		(point.y >= min.y && point.y <= max.y) &&
		(point.z >= min.z && point.z <= max.z);
}

BoundingBox BoundingBox::operator+(const BoundingBox& other) const {
	BoundingBox result;
	result.min = glm::min(min, other.min);
	result.max = glm::max(max, other.max);
	return result;
}

BoundingBox operator*(const mat4& mat, const BoundingBox& bbox) {
	auto vertices = bbox.vertices();
	for (auto& v : vertices) v = mat * vec4(v, 1);
	return BoundingBox(vertices.data(), vertices.size());
}

bool BoundingBox::intersects(const BoundingBox& other) const {
	return (min.x <= other.max.x && max.x >= other.min.x) &&
		(min.y <= other.max.y && max.y >= other.min.y) &&
		(min.z <= other.max.z && max.z >= other.min.z);
}

std::array<BoundingBox, 8> BoundingBox::subdivide() const {
	vec3 center = this->center();
	return {
		BoundingBox{min, center},
		BoundingBox{{center.x, min.y, min.z}, {max.x, center.y, center.z}},
		BoundingBox{{min.x, center.y, min.z}, {center.x, max.y, center.z}},
		BoundingBox{{center.x, center.y, min.z}, {max.x, max.y, center.z}},
		BoundingBox{{min.x, min.y, center.z}, {center.x, center.y, max.z}},
		BoundingBox{{center.x, min.y, center.z}, {max.x, center.y, max.z}},
		BoundingBox{{min.x, center.y, center.z}, {center.x, max.y, max.z}},
		BoundingBox{center, max}
	};
}

bool BoundingBox::contains(const GameObject& go) const {

	BoundingBox box = go.boundingBox();
	return contains(box);

}

void BoundingBox::draw() {
	glLineWidth(2.0);
	drawWiredQuad(v000(), v001(), v011(), v010());
	drawWiredQuad(v100(), v101(), v111(), v110());
	drawWiredQuad(v000(), v001(), v101(), v100());
	drawWiredQuad(v010(), v011(), v111(), v110());
	drawWiredQuad(v000(), v010(), v110(), v100());
	drawWiredQuad(v001(), v011(), v111(), v101());
}

void glVertex3(const vec3& v) { glVertex3dv(&v.x); }

void drawWiredQuad(const vec3& v0, const vec3& v1, const vec3& v2, const vec3& v3) {
	glBegin(GL_LINE_LOOP);
	glVertex3(v0);
	glVertex3(v1);
	glVertex3(v2);
	glVertex3(v3);
	glEnd();

}

