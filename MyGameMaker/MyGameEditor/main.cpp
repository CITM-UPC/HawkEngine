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
#include "EditorCamera.h"
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <IL/il.h>
#include <IL/ilu.h>
#include <stack>
#include <IL/ilut.h>
#include "Input.h"
#include "MyGUI.h"
#include "UISceneWindow.h"
#include "MyGameEngine/CameraBase.h"
#include "MyGameEngine/BoundingBox.h"
#include "MyGameEngine/CameraComponent.h"
#include "MyGameEngine/GameObject.h"
#include "MyGameEngine/TransformComponent.h"
#include "MyGameEngine/MeshRendererComponent.h"
#include "App.h"

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

static const ivec2 WINDOW_SIZE(1280, 720);
static const auto FPS = 120;
static const auto FRAME_DT = 1.0s / FPS;

static EditorCamera* camera = nullptr;

App* Application = NULL;

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
}

static void drawFloorGrid(int size, double step) {
	//glColor3ub(0, 2, 200);
	glBegin(GL_LINES);
	Application->root->currentScene->DebugDrawTree();

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

	//camera->frustum.Update(projectionMatrix * viewMatrix);
}

void configureGameCamera()
{
	if (Application->root->mainCamera != nullptr)
	{
		glm::dmat4 projectionMatrix = Application->root->mainCamera->GetComponent<CameraComponent>()->projection();
		glm::dmat4 viewMatrix = Application->root->mainCamera->GetComponent<CameraComponent>()->view();

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixd(glm::value_ptr(projectionMatrix));

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(glm::value_ptr(viewMatrix));

		//Application->root->mainCamera->GetComponent<CameraComponent>()->frustum.Update(projectionMatrix * viewMatrix);
	}
}

void drawFrustum(const CameraBase& camera)
{
	const auto& frustum = camera.frustum;

	glBegin(GL_LINES);
	for (int i = 0; i < 4; i++) {
		glVertex3fv(glm::value_ptr(frustum.vertices[i]));
		glVertex3fv(glm::value_ptr(frustum.vertices[(i + 1) % 4]));

		glVertex3fv(glm::value_ptr(frustum.vertices[i + 4]));
		glVertex3fv(glm::value_ptr(frustum.vertices[(i + 1) % 4 + 4]));

		glVertex3fv(glm::value_ptr(frustum.vertices[i]));
		glVertex3fv(glm::value_ptr(frustum.vertices[i + 4]));
	}
	glEnd();
}

bool isInsideFrustum(const BoundingBox& bbox, const std::list<CameraBase::Plane>& frustumPlanes) {
	for (const auto& plane : frustumPlanes) {
		// Si todos los v�rtices del BoundingBox est�n fuera de un plano, entonces el BoundingBox est� fuera del frustum.
		if (plane.distanceToPoint(bbox.v000()) < 0 &&
			plane.distanceToPoint(bbox.v001()) < 0 &&
			plane.distanceToPoint(bbox.v010()) < 0 &&
			plane.distanceToPoint(bbox.v011()) < 0 &&
			plane.distanceToPoint(bbox.v100()) < 0 &&
			plane.distanceToPoint(bbox.v101()) < 0 &&
			plane.distanceToPoint(bbox.v110()) < 0 &&
			plane.distanceToPoint(bbox.v111()) < 0) {
			return false;
		}
	}
	return true;
}

static void display_func() {
	glBindFramebuffer(GL_FRAMEBUFFER, Application->gui->fbo);
	glViewport(0, 0, Application->window->width(), Application->window->height());
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	configureCamera();
	//drawFrustum(*camera);

	//configureGameCamera();
	//drawFrustum(*Application->root->mainCamera->GetComponent<CameraComponent>());

	drawFloorGrid(16, 0.25);

	glm::vec3 rayOrigin = glm::vec3(glm::inverse(camera->view()) * glm::vec4(0, 0, 0, 1));
	glm::vec3 rayDirection = Application->input->getMousePickRay();

	for (size_t i = 0; i < Application->root->currentScene->children().size(); ++i)
	{
		GameObject* object = Application->root->currentScene->children()[i].get();

		object->Update(Application->GetDt());

		if (object->HasComponent<MeshRenderer>()) {

			BoundingBox bbox = object->GetComponent<MeshRenderer>()->GetMesh()->boundingBox();

			bbox = object->GetTransform()->GetMatrix() * bbox;

			if (!isInsideFrustum(bbox, { camera->frustum._near, camera->frustum._far,
									camera->frustum.left, camera->frustum.right,
									camera->frustum.top, camera->frustum.bot })) {
				continue; // Aqu� omitimos el objeto si no est� en el frustum
			}

			if (Application->gui->UISceneWindowPanel->CheckRayAABBCollision(rayOrigin, rayDirection, bbox))
			{
				Application->input->SetDraggedGameObject(object);
			}

			if (Application->input->GetMouseButton(1) == KEY_DOWN && Application->gui->UISceneWindowPanel->isFoucused)
				if (Application->gui->UISceneWindowPanel->CheckRayAABBCollision(rayOrigin, rayDirection, bbox))
				{
					Application->input->ClearSelection();
					Application->input->AddToSelection(object);
				}
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void display_func2() {

	glBindFramebuffer(GL_FRAMEBUFFER, Application->gui->fboCamera);

	glViewport(0, 0, Application->window->width(), Application->window->height());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//configureGameCamera();
	//drawFrustum(*Application->root->mainCamera->GetComponent<CameraComponent>());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PauCode2(MyGUI* gui) {

	if (Application->window->IsOpen()) {

		const auto t0 = hrclock::now();
		display_func();
		//display_func2();
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

			camera->GetTransform().GetPosition() = vec3(0, 1, 4);
			camera->GetTransform().Rotate(glm::radians(180.0), vec3(0, 1, 0));
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