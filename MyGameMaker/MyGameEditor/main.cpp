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

//pruebas de include "GameObject.h"

#include "MyGameEngine/GameObject.h"
#include "MyGameEngine/TransformComponent.h"
#include "MyGameEngine/MeshRendererComponent.h"

using namespace std;


#define CHECKERS_HEIGHT 64
#define CHECKERS_WIDTH 64

GLuint textureID;

using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;
Mesh mesh;

static Camera camera;
static const ivec2 WINDOW_SIZE(1280, 720);
static const auto FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;


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

	/*glm::dvec3 forward = camera.transform().fwd();
	glm::dvec3 right = camera.transform().left();
	glm::dvec3 up = camera.transform().up();

	double moveSpeed = 0.1;

	if (ImGui::IsKeyDown(ImGuiKey_W)) camera.transform().translate(-forward * moveSpeed);
	if (ImGui::IsKeyDown(ImGuiKey_S)) camera.transform().translate(forward * moveSpeed);
	if (ImGui::IsKeyDown(ImGuiKey_A)) camera.transform().translate(-right * moveSpeed);
	if (ImGui::IsKeyDown(ImGuiKey_D)) camera.transform().translate(right * moveSpeed);
	if (ImGui::IsKeyDown(ImGuiKey_Q)) camera.transform().translate(-up * moveSpeed);
	if (ImGui::IsKeyDown(ImGuiKey_E)) camera.transform().translate(up * moveSpeed);*/

	//default movement
	//move the transform of the camera
	if (ImGui::IsKeyDown(ImGuiKey_W)) camera.transform().translate(-camera.transform().fwd() * 0.1);
	if (ImGui::IsKeyDown(ImGuiKey_S)) camera.transform().translate(camera.transform().fwd() * 0.1);
	if (ImGui::IsKeyDown(ImGuiKey_A)) camera.transform().translate(-camera.transform().left() * 0.1);
	if (ImGui::IsKeyDown(ImGuiKey_D)) camera.transform().translate(camera.transform().left() * 0.1);
	if (ImGui::IsKeyDown(ImGuiKey_Q)) camera.transform().translate(-camera.transform().up() * 0.1);
	if (ImGui::IsKeyDown(ImGuiKey_E)) camera.transform().translate(camera.transform().up() * 0.1);
	//rotate the transform of the camera
	if (ImGui::IsKeyDown(ImGuiKey_LeftArrow)) camera.transform().rotate(0.02, vec3(0, 1, 0));
	if (ImGui::IsKeyDown(ImGuiKey_RightArrow)) camera.transform().rotate(-0.02, vec3(0, 1, 0));
	if (ImGui::IsKeyDown(ImGuiKey_UpArrow)) camera.transform().rotate(0.02, camera.transform().left());
	if (ImGui::IsKeyDown(ImGuiKey_DownArrow)) camera.transform().rotate(-0.02, camera.transform().left());

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

	mesh.Draw();

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

int main(int argc, char** argv) {
	
	//initialize devil
	ilInit();
	iluInit();
	ilutInit();
	MyWindow window("ImGUI with SDL2 Simple Example", WINDOW_SIZE.x, WINDOW_SIZE.y);
	MyGUI gui(window.windowPtr(), window.contextPtr());

	init_openGL();
	camera.transform().pos() = vec3(0, 1, 4);
	camera.transform().rotate(glm::radians(180.0), vec3(0, 1, 0));

	std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>("TesteoLocoJIJI");
	gameObject->AddComponent<Transform_Component>();
	auto renderer = gameObject->AddComponent<MeshRenderer>();

	std::shared_ptr<Mesh> myMesh = std::make_shared<Mesh>();
	myMesh->LoadMesh("BakerHouse.fbx");
	myMesh->LoadTexture("Baker_house.png");
	//myMesh->LoadCheckerTexture(); // uV caramba

	renderer->SetMesh(myMesh);	

	//mesh.LoadMesh("BakerHouse.fbx");
	//mesh.LoadTexture("Baker_house.png");
	//mesh.LoadCheckerTexture();

	MarcoTests(); //hola buenas tardes

	while (window.processEvents(&gui) && window.isOpen()) {
		const auto t0 = hrclock::now();
		display_func();

		renderer->Render();

		gui.render();
		move_camera();
		window.swapBuffers();
		const auto t1 = hrclock::now();
		const auto dt = t1 - t0;
		if (dt < FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);
	}

	return 0;
}
