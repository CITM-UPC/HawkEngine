#include "Gizmos.h"
#include "App.h"
#include "Input.h"
#include "MyGUI.h"
#include "UISceneWindow.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp> 

Gizmos::Gizmos(App* app) : Module(app) { ; }

bool Gizmos::FixedUpdate()
{
	return true;
}



bool Gizmos::Start() { return true; }

bool Gizmos::Awake() { return true; }

bool Gizmos::Update(double dt) {

    rayStartPos = glm::vec3(glm::inverse(Application->camera->view()) * glm::vec4(0, 0, 0, 1));
    rayDir = Application->input->getMousePickRay();

	if (Application->input->GetSelectedGameObject() != nullptr) {
        ProcessMousePicking();
	}


	return true;
}

//(Fornow) it cant go on the update since we need it inside the drawfunction on main
void Gizmos::DrawGizmos() {
 
    if (Application->input->GetSelectedGameObject() != nullptr) 
    {
		IntersectAxis(rayStartPos, rayDir, selectedAxis);
        glm::vec3 position = Application->input->GetSelectedGameObject()->GetTransform()->GetPosition();

        // Deshabilitar la prueba de profundidad para dibujar los gizmos sobre la malla
        glDisable(GL_DEPTH_TEST);

        glBegin(GL_LINES);

        // Dibujar eje X (rojo)
        if (selectedAxis == glm::vec3(1, 0, 0)) {
            glColor3f(1.0f, 1.0f, 0.0f); // Amarillo si est� seleccionado
        }
        else {
            glColor3f(1.0f, 0.0f, 0.0f);
        }
        glVertex3f(position.x, position.y, position.z);
        glVertex3f(position.x + 1.0f, position.y, position.z);

        // Dibujar eje Y (verde)
        if (selectedAxis == glm::vec3(0, 1, 0)) {
            glColor3f(1.0f, 1.0f, 0.0f); // Amarillo si est� seleccionado
        }
        else {
            glColor3f(0.0f, 1.0f, 0.0f);
        }
        glVertex3f(position.x, position.y, position.z);
        glVertex3f(position.x, position.y + 1.0f, position.z);

        // Dibujar eje Z (azul)
        if (selectedAxis == glm::vec3(0, 0, 1)) {
            glColor3f(1.0f, 1.0f, 0.0f); // Amarillo si est� seleccionado
        }
        else {
            glColor3f(0.0f, 0.0f, 1.0f);
        }
        glVertex3f(position.x, position.y, position.z);
        glVertex3f(position.x, position.y, position.z + 1.0f);

        glEnd();

        // Restaurar la prueba de profundidad
        glEnable(GL_DEPTH_TEST);

        glColor3f(1.0f, 1.0f, 1.0f);
    }
}

void Gizmos::ProcessMousePicking() {
    static bool isDragging = false;

    if (Application->input->GetMouseButton(1) == KEY_DOWN) {
    
		IntersectAxis(rayStartPos, rayDir, selectedAxis);
        // Detectar si el rayo intersecta con alguno de los ejes
        if (IntersectAxis(rayStartPos, rayDir, selectedAxis)) {
            isDragging = true;
            isAxisSelected = true;
        } else {
            isAxisSelected = false;
        }
    }

    if (Application->input->GetMouseButton(1) == KEY_UP) {
        isDragging = false;
        isAxisSelected = false;
        Application->input->SetDraggedGameObject(nullptr);
    }

    if (isDragging) {
    
        // Mover el objeto a lo largo del eje seleccionado
        MoveObjectAlongAxis(rayStartPos, rayDir, selectedAxis);
    }
}

bool RaysIntersect(const glm::vec3& origin1, const glm::vec3& direction1,
    const glm::vec3& origin2, const glm::vec3& direction2,
    float tolerance = 1e-2f)
{
    // Normalize directions
    glm::vec3 d1 = glm::normalize(direction1);
    glm::vec3 d2 = glm::normalize(direction2);

    // Cross product of directions
    glm::vec3 crossD = glm::cross(d1, d2);

    // If crossD is (near) zero, rays are parallel or coincident
    float crossLen2 = glm::length2(crossD);
    if (crossLen2 < tolerance) {
        // Check if the rays are collinear
        glm::vec3 diff = origin2 - origin1;
        if (glm::length2(glm::cross(diff, d1)) < tolerance) {
            return true; // Collinear rays intersect
        }
        return false; // Parallel but not intersecting
    }

    // Calculate shortest line connecting the rays
    glm::vec3 diff = origin2 - origin1;

    // Parameters for closest points on each ray
    float t1 = glm::dot(glm::cross(diff, d2), crossD) / crossLen2;
    float t2 = glm::dot(glm::cross(diff, d1), crossD) / crossLen2;

    // Closest points on each ray
    glm::vec3 point1 = origin1 + t1 * d1;
    glm::vec3 point2 = origin2 + t2 * d2;

    // Check if the closest points are within the tolerance
    return glm::length2(point1 - point2) < tolerance;
}

bool Gizmos::IntersectAxis(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, glm::vec3& outAxis) {
    glm::vec3 position = Application->input->GetSelectedGameObject()->GetTransform()->GetPosition();

    // Eje X
  
    if (RaysIntersect(rayOrigin,rayDirection,position,glm::vec3 (1,0,0)) && isAxisSelected == false){
        selectedAxis = glm::vec3(1, 0, 0);
        return true;
    }

    // Eje Y
    if (RaysIntersect(rayOrigin, rayDirection, position, glm::vec3(0, 1, 0)) && isAxisSelected == false) {
        selectedAxis = glm::vec3(0, 1, 0);
        return true;
    }

    // Eje Z
    if (RaysIntersect(rayOrigin, rayDirection, position, glm::vec3(0, 0, 1)) && isAxisSelected == false) {
        selectedAxis = glm::vec3(0, 0, 1);
        return true;
    }

	if (isAxisSelected == false)
    selectedAxis = glm::vec3(0, 0, 0);

    return false;
}


void Gizmos::MoveObjectAlongAxis(const glm::vec3& rayOrigin, glm::vec3& rayDirection, const glm::vec3& axis) {
 
    glm::vec3 normalizedRayDir = glm::normalize(rayDirection);
    glm::vec3 normalizedAxis = glm::normalize(axis);

    // Compute the projection of the ray direction onto the axis
    float projectionScale = glm::dot(normalizedRayDir, normalizedAxis);
    glm::dvec3 projectedRayDir = projectionScale * normalizedAxis;

    // Find the closest point on the axis to the ray origin
  
    // Move the selected game object to the projected point
    Application->input->GetSelectedGameObject()->GetTransform()->SetPosition(projectedRayDir);
}
