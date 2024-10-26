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
#include <IL/il.h>
#include <IL/ilu.h>	
#include <IL/ilut.h>
#include "Input.h"

//pruebas de include "GameObject.h"

#include "MyGameEngine/GameObject.h"
#include "MyGameEngine/TransformComponent.h"
#include "MyGameEngine/MeshRendererComponent.h"
#include "App.h"


using namespace std;


#define CHECKERS_HEIGHT 64
#define CHECKERS_WIDTH 64

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
static const auto FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

static Camera camera;

static void init_openGL() {
	glewInit();
	if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API is not available.");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glClearColor(0.5, 0.5, 0.5, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glScaled(1.0, (double)WINDOW_SIZE.x/WINDOW_SIZE.y, 1.0);
	
	glMatrixMode(GL_MODELVIEW);
}

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
void move_camera() 
{
	static bool isPanning = false;
	static glm::dvec2 lastMousePos = glm::dvec2(0.0, 0.0);

	if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
	{
		glm::dvec2 mousePos = glm::dvec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);

		if (!isPanning) {
			isPanning = true;
			lastMousePos = mousePos;
		}

		glm::dvec2 delta = mousePos - lastMousePos;

		double sensitivity = 0.005;
		camera.transform().rotate(sensitivity * -delta.x, vec3(0, 1, 0));
		camera.transform().rotate(sensitivity * -delta.y, camera.transform().left());
	
		lastMousePos = mousePos;
	}
	else {
		isPanning = false;
	}


	if (Application->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		camera.transform().translate(-camera.transform().fwd() * 0.1);
	}

	if (Application->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) camera.transform().translate(camera.transform().fwd() * 0.1);
	if (Application->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) camera.transform().translate(-camera.transform().left() * 0.1);
	if (Application->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) camera.transform().translate(camera.transform().left() * 0.1);
	if (Application->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) camera.transform().translate(-camera.transform().up() * 0.1);
	if (Application->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) camera.transform().translate(camera.transform().up() * 0.1);
	//rotate the transform of the camera
	
	if (Application->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		camera.transform().rotate(0.02, vec3(0, 1, 0));
	}
	if (Application->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		camera.transform().rotate(-0.02, vec3(0, 1, 0));
	}
	if (Application->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) camera.transform().rotate(0.02, camera.transform().left());
	if (Application->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) camera.transform().rotate(-0.02, camera.transform().left());

}



void configureCamera() {
	glm::dmat4 projectionMatrix = camera.projection();
	glm::dmat4 viewMatrix = camera.view();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(glm::value_ptr(projectionMatrix));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(glm::value_ptr(viewMatrix));
}


static void display_func() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	//glLoadMatrixd(&camera.view()[0][0]);
	configureCamera();

	drawFloorGrid(16, 0.25);

	Application->ElMesh.Draw();

}


static void MarcoTests()
{
	std::shared_ptr<GameObject> parent = std::make_shared<GameObject>("Parent");
	std::shared_ptr<GameObject> child1 = std::make_shared<GameObject>("Child1");
	std::shared_ptr<GameObject> child2 = std::make_shared<GameObject>("Child2");

	parent->AddChild(child1);
	parent->AddChild(child2);

	std::cout << "Parent: " << parent->GetName() << std::endl;
	std::cout << "  Children:" << std::endl;
	for (const auto& child : parent->GetChildren()) {
		std::cout << "    " << child->GetName() << std::endl;
	}

	parent->RemoveChild(child1);

	std::cout << "Parent: " << parent->GetName() << std::endl;
	std::cout << "  Children:" << std::endl;
	for (const auto& child : parent->GetChildren()) {
		std::cout << "    " << child->GetName() << std::endl;
	}

	parent->AddComponent<Transform_Component>();
	auto transform = parent->GetComponent<Transform_Component>();
	transform->SetPosition(glm::vec3(1.0f, 2.0f, 3.0f));
	std::cout << "Parent position: " << transform->GetPosition().x << ", " << transform->GetPosition().y << ", " << transform->GetPosition().z << std::endl;

	std::cout << "Parent has Transform component: " << parent->HasComponent<Transform_Component>() << std::endl;
	parent->RemoveComponent<Transform_Component>();
	std::cout << "Parent has Transform component: " << parent->HasComponent<Transform_Component>() << std::endl;

	parent->SetActive(false);
	std::cout << "Parent is active: " << parent->IsActive() << std::endl;
	parent->SetActive(true);
	std::cout << "Parent is active: " << parent->IsActive() << std::endl;

	parent->Destroy();
	std::cout << "Parent is destroyed: " << parent->IsActive() << std::endl;
}
App* Application = NULL;



//MyGUI PauCode() {
//	//initialize devil
//	ilInit();
//	iluInit();
//	ilutInit();
//	/*Window window("ImGUI with SDL2 Simple Example", WINDOW_SIZE.x, WINDOW_SIZE.y);*/
//	MyGUI gui(Application->window->windowPtr() , Application->window->contextPtr());
//	
//	init_openGL();
//	camera.transform().pos() = vec3(0, 1, 4);
//	camera.transform().rotate(glm::radians(180.0), vec3(0, 1, 0));
//	
//	
//	
//	mesh.LoadMesh("BakerHouse.fbx");
//	mesh.LoadTexture("Baker_house.png");
//	mesh.LoadCheckerTexture();
//	
//
//	//while (window.processEvents(&gui) && window.isOpen()) {
//	//	const auto t0 = hrclock::now();
//	//display_func();
//	//	gui.render();
//	//	move_camera();
//	//	window.swapBuffers();
//	//	const auto t1 = hrclock::now();
//	//	const auto dt = t1 - t0;
//	//	if (dt < FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);
//	//}
//}

void PauCode2(MyGUI* gui) {

	if (Application->window->ProcessEvents(gui) && Application->window->IsOpen()) {

		const auto t0 = hrclock::now();
		display_func();
		gui->Render();

		move_camera();
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

	//initialize devil
	ilInit();
	iluInit();
	ilutInit();
	/*Window window("ImGUI with SDL2 Simple Example", WINDOW_SIZE.x, WINDOW_SIZE.y);*/
	//MyGUI gui(Application->window->windowPtr(), Application->window->contextPtr());

	init_openGL();
	camera.transform().pos() = vec3(0, 1, 4);
	camera.transform().rotate(glm::radians(180.0), vec3(0, 1, 0));



	Application->ElMesh.LoadMesh("BakerHouse.fbx");
	Application->ElMesh.LoadTexture("Baker_house.png");
	Application->ElMesh.LoadCheckerTexture();

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

