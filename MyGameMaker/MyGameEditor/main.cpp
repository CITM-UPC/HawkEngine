#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#define GLM_ENABLE_EXPERIMENTAL
#define CHECKERS_HEIGHT 64
#define CHECKERS_WIDTH 64

#include <string>
#include <GL/glew.h>
#include <chrono>
#include <thread>
#include <exception>
#include <iostream>
#include <glm/glm.hpp>
#include "MyWindow.h"
#include "MyGUI.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "assimp/cimport.h" 
#include "assimp/scene.h" 
#include "assimp/postprocess.h"
#include "../MyGameEngine/Mesh.h"
#include "Camera.h"
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <IL/il.h>
#include <IL/ilu.h>	
#include <IL/ilut.h>
#include "Input.h"
#include "MyGUI.h"
#include "UISceneWindow.h"


//pruebas de include "StaticLibEngineIncludes"
#include "MyGameEngine/GameObject.h"
#include "MyGameEngine/TransformComponent.h"
#include "MyGameEngine/MeshRendererComponent.h"
#include "MyGameEngine/Shaders.h"
#include "App.h"


#include "Gizmos.h"

using namespace std;


enum MainState
{
	CREATE,
	AWAKE,
	START,
	LOOP,
	FREE,
	FAIL,
	EXIT
};

GLuint textureID;

using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;

GLuint depthMapFBO;
GLuint depthMap;
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

Shaders shadowShader;
Shaders mainShader;

static const ivec2 WINDOW_SIZE(1280, 720);
static const auto FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

static Camera* camera = nullptr;

App* Application = NULL;

void initDepthMap() {
	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static void init_openGL() {
	glewInit();
	if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API is not available.");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glScaled(1.0, (double)WINDOW_SIZE.x/WINDOW_SIZE.y, 1.0);
	
	glMatrixMode(GL_MODELVIEW);

	initDepthMap();
}

struct Triangle {
	vec3 v0, v1, v2;
	void draw_triangle(const u8vec4& color) {
		//glColor4ub(color.r, color.g, color.b, color.a);
		glBegin(GL_TRIANGLES);
		glVertex3d(v0.x, v0.y, v0.z);
		glVertex3d(v1.x, v1.y, v1.z);
		glVertex3d(v2.x, v2.y, v2.z);
		//glColor4ub(255.0f, 255.0f, 255.0f, 0.0f);
		glEnd();
	}
};

static void draw_triangle(const u8vec4& color, const vec3& center, double size) {
	glColor4ub(color.r, color.g, color.b, color.a);
	glBegin(GL_TRIANGLES);
	glVertex3d(center.x, center.y + size, center.z);
	glVertex3d(center.x - size, center.y - size, center.z);
	glVertex3d(center.x + size, center.y - size, center.z);
	
	glEnd();
}

static void drawFloorGrid(int size, double step) {
	//glColor3ub(0, 2, 200);
	glBegin(GL_LINES);
	for (double i = -size; i <= size; i += step) {
		glVertex3d(i, 0, -size);
		glVertex3d(i, 0, size);
		glVertex3d(-size, 0, i);
		glVertex3d(size, 0, i);
	}
	
	glEnd();
}



// Initializes camera
void configureCamera() {
	glm::dmat4 projectionMatrix = camera->projection();
	glm::dmat4 viewMatrix = camera->view();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(glm::value_ptr(projectionMatrix));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(glm::value_ptr(viewMatrix));
}
#pragma region RayPickingCode(MoveSomewhereElse)

bool CheckRayAABBCollision(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const BoundingBox& bBox) {
	float tmin = (bBox.min.x - rayOrigin.x) / rayDir.x;
	float tmax = (bBox.max.x - rayOrigin.x) / rayDir.x;

	if (tmin > tmax) std::swap(tmin, tmax);

	float tymin = (bBox.min.y - rayOrigin.y) / rayDir.y;
	float tymax = (bBox.max.y - rayOrigin.y) / rayDir.y;

	if (tymin > tymax) std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (bBox.min.z - rayOrigin.z) / rayDir.z;
	float tzmax = (bBox.max.z - rayOrigin.z) / rayDir.z;

	if (tzmin > tzmax) std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	return true;
}

void Draw3DRectangle(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, float width, float height, float depth) {
	glm::vec3 right = glm::normalize(glm::cross(rayDirection, glm::vec3(0.0f, 1.0f, 0.0f))) * width;
	glm::vec3 up = glm::normalize(glm::cross(right, rayDirection)) * height;
	glm::vec3 forward = glm::normalize(rayDirection) * depth;

	glm::vec3 vertices[8];
	vertices[0] = rayOrigin;
	vertices[1] = rayOrigin + right;
	vertices[2] = rayOrigin + right + up;
	vertices[3] = rayOrigin + up;
	vertices[4] = rayOrigin + forward;
	vertices[5] = rayOrigin + forward + right;
	vertices[6] = rayOrigin + forward + right + up;
	vertices[7] = rayOrigin + forward + up;

	glBegin(GL_LINES);
	// Front face
	glVertex3fv(glm::value_ptr(vertices[0]));
	glVertex3fv(glm::value_ptr(vertices[1]));
	glVertex3fv(glm::value_ptr(vertices[1]));
	glVertex3fv(glm::value_ptr(vertices[2]));
	glVertex3fv(glm::value_ptr(vertices[2]));
	glVertex3fv(glm::value_ptr(vertices[3]));
	glVertex3fv(glm::value_ptr(vertices[3]));
	glVertex3fv(glm::value_ptr(vertices[0]));

	// Back face
	glVertex3fv(glm::value_ptr(vertices[4]));
	glVertex3fv(glm::value_ptr(vertices[5]));
	glVertex3fv(glm::value_ptr(vertices[5]));
	glVertex3fv(glm::value_ptr(vertices[6]));
	glVertex3fv(glm::value_ptr(vertices[6]));
	glVertex3fv(glm::value_ptr(vertices[7]));
	glVertex3fv(glm::value_ptr(vertices[7]));
	glVertex3fv(glm::value_ptr(vertices[4]));

	// Connecting lines
	glVertex3fv(glm::value_ptr(vertices[0]));
	glVertex3fv(glm::value_ptr(vertices[4]));
	glVertex3fv(glm::value_ptr(vertices[1]));
	glVertex3fv(glm::value_ptr(vertices[5]));
	glVertex3fv(glm::value_ptr(vertices[2]));
	glVertex3fv(glm::value_ptr(vertices[6]));
	glVertex3fv(glm::value_ptr(vertices[3]));
	glVertex3fv(glm::value_ptr(vertices[7]));
	glEnd();
}


#pragma endregion

void renderScene(Shaders& shader) {

	glm::vec3 rayOrigin = glm::vec3(glm::inverse(camera->view()) * glm::vec4(0, 0, 0, 1));
	glm::vec3 rayDirection = Application->input->getMousePickRay();

	// TODO cambiar esto de sitio
	for (size_t i = 0; i < Application->root->currentScene->children().size(); ++i)
	{
		GameObject* object = Application->root->currentScene->children()[i].get();

		object->Update(0.16f, shader);

		if (object->HasComponent<MeshRenderer>()) {

			BoundingBox bbox = object->GetComponent<MeshRenderer>()->GetMesh()->boundingBox();

			bbox = object->GetTransform()->GetMatrix() * bbox;

			if (CheckRayAABBCollision(rayOrigin, rayDirection, bbox))
			{
				Application->input->SetDraggedGameObject(object);
			}

			if (Application->input->GetMouseButton(1) == KEY_DOWN)
				if (CheckRayAABBCollision(rayOrigin, rayDirection, bbox))
				{
					std::cout << "Hit: " << object->GetName();
					Application->input->AddToSelection(object);
				}
		}
	}

}

void renderSceneFromLight() {
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 7.5f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(glm::vec3(0.0f, 4.0f, 4.0f), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

	// Render the scene to the depth map
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Use your shadow shader here
	shadowShader.Bind();
	shadowShader.SetUniform("lightSpaceMatrix", lightSpaceMatrix);
	renderScene(shadowShader); // You need to implement this function to render your scene

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



static void display_func() {

	renderSceneFromLight();

	glBindFramebuffer(GL_FRAMEBUFFER, Application->gui->fbo);
	glViewport(0, 0, Application->window->width(), Application->window->height());
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	configureCamera();
	drawFloorGrid(16, 0.25);



	
	//Debug for mousepicking
	//Draw3DRectangle(rayOrigin, rayDirection, 1.0f, 1.0f, 1.0f);
	
	renderScene(mainShader);
	
	//It has to go AFTER drawing the objects
	//Application->gizmos->DrawGizmos();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Application->root->sceneManagement.Update(0.16f);

}

void PauCode2(MyGUI* gui) {

	if (Application->window->IsOpen()) {

		const auto t0 = hrclock::now();

		display_func();
	
		gui->Render();

		/*move_camera();*/
		Application->window->SwapBuffers();
		const auto t1 = hrclock::now();
		const auto dt = t1 - t0;
		if (dt < FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);
	}
}

int main(int argc, char** argv) {

	MainState state = CREATE;

	// The application is created
	Application = new App();

	/*Application->camera = new Camera(Application);*/
	
	//std::shared_ptr<Camera> cameraPtr = std::make_shared<Camera>(Application);
	//camera = cameraPtr.get();

	//Application->camera = shared_ptr<Camera>
	

	//initialize devil
	ilInit();
	iluInit();
	ilutInit();

	init_openGL();

	if (mainShader.LoadShaders("vertex_shader.glsl", "fragment_shader.glsl")) 
	{
		//error
	}
	if (shadowShader.LoadShaders("vertex_shader.glsl", "fragment_shader.glsl"))
	{
		//error
	}

	camera = Application->camera;

	while (state != EXIT) 
	{
		switch (state)
		{

		case CREATE:

			/*Application = new App();*/

			if (Application) {	
				state = AWAKE;

				/*gui = PauCode();*/
			}
			else { state = FAIL; printf("Failed on Create"); }
			break;


		case AWAKE:
			//Application->AddModule(cameraPtr.get(), true);

			camera->transform().pos() = vec3(0, 1, 4);
			camera->transform().rotate(glm::radians(180.0), vec3(0, 1, 0));
			if (Application->Awake()) { state = START; }

			else
			{
				printf("Failed on Awake");
				state = FAIL;
			}
			break;

		case START:

			if (Application->Start()) { state = LOOP; }
			else { state = FAIL; printf("Failed on START"); }
			break;

		case LOOP:
			

			PauCode2(Application->gui);

			if (!Application->Update()) {
				state = FREE;
			}
			break;


		case FREE:

			// TODO Free all classes and memory
			state = EXIT;
			break;
		}

	}






	//initialize devil
	//ilInit();
	//iluInit();
	//ilutInit();
	//Window window("ImGUI with SDL2 Simple Example", WINDOW_SIZE.x, WINDOW_SIZE.y);
	//MyGUI gui(window.windowPtr(), window.contextPtr());

	//init_openGL();
	//camera.transform().pos() = vec3(0, 1, 4);
	//camera.transform().rotate(glm::radians(180.0), vec3(0, 1, 0));

	//

	//mesh.LoadMesh("BakerHouse.fbx");
	//mesh.LoadTexture("Baker_house.png");
	//mesh.LoadCheckerTexture();

	//while (window.processEvents(&gui) && window.isOpen()) {
	//	const auto t0 = hrclock::now();
	//display_func();
	//	gui.render();
	//	move_camera();
	//	window.swapBuffers();
	//	const auto t1 = hrclock::now();
	//	const auto dt = t1 - t0;
	//	if (dt < FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);
	//}

	return 0;
}

