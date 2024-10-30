#include "MyGUI.h"
#include "App.h"
#include "Module.h"
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>
#include <imgui.h>
#include <imgui_internal.h>	
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <vector>
#include <iostream>

#include "UIElement.h"
#include "UIConsole.h"
#include "UISettings.h"
#include "UIMainMenuBar.h"
#include "UIInspector.h"

MyGUI::MyGUI(App* app) : Module(app) {
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(app->window->windowPtr(), app->window->contextPtr());
	ImGui_ImplOpenGL3_Init();
}

MyGUI::~MyGUI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

struct SceneObject {
	const char* name;
	std::vector<SceneObject> children; // List of child objects
};

bool MyGUI::Awake() {
	// Example UI
	LOG(LogType::LOG_INFO, "UI Awake");

	bool ret = true;

	UIconsolePanel = new UIConsole(UIType::CONSOLE, "Console");
	elements.push_back(UIconsolePanel);
	ret *= isInitialized(UIconsolePanel);

	UIsettingsPanel = new UISettings(UIType::SETTINGS, "Settings");
	elements.push_back(UIsettingsPanel);
	ret *= isInitialized(UIsettingsPanel);

	UIMainMenuBarPanel = new UIMainMenuBar(UIType::DEFAULT, "MainMenuBar");
	elements.push_back(UIMainMenuBarPanel);
	ret *= isInitialized(UIMainMenuBarPanel);

	UIinspectorPanel = new UIInspector(UIType::INSPECTOR, "Inspector");
	elements.push_back(UIinspectorPanel);
	ret *= isInitialized(UIinspectorPanel);

	// Other UI elements
	
	return ret;
}

bool MyGUI::isInitialized(UIElement* element) {
	if (!element)
	{
		LOG(LogType::LOG_ERROR, "-%s", element->GetName().c_str());
		return false;
	}
	LOG(LogType::LOG_INFO, "+%s", element->GetName().c_str());
	return true;
}

bool MyGUI::Start() {
	
	LOG(LogType::LOG_INFO, "Initializing ImGui/ImPlot...");

	IMGUI_CHECKVERSION();
	LOG(LogType::LOG_OK, "-ImGui Version: %s", IMGUI_VERSION);

	ImGuiContext* contextGUI = ImGui::CreateContext();

	if (!contextGUI)
	{
		LOG(LogType::LOG_ERROR, "-ImGui Context not created");
	}
	else
	{
		LOG(LogType::LOG_OK, "-ImGui Context created");
	}


	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	if (!&io)
	{
		LOG(LogType::LOG_ERROR, "-ImGui IO not created");
	}
	else
	{
		LOG(LogType::LOG_OK, "-ImGui IO created");
	}

	ImGui::StyleColorsClassic();

	Application->gui->UIconsolePanel->SetState(true);
	Application->gui->UIsettingsPanel->SetState(true);
	Application->gui->UIinspectorPanel->SetState(true);

	return true;
}


bool MyGUI::PreUpdate() { 
	

	return true;
}

bool MyGUI::Update(double dt) { 

	//Render();

	////move the transform of the camera
	//


	//if (ImGui::IsKeyDown(ImGuiKey_W)) {
	//	camera.transform().translate(-camera.transform().fwd() * 0.1);
	//}
	//if (ImGui::IsKeyDown(ImGuiKey_S)) camera.transform().translate(camera.transform().fwd() * 0.1);
	//if (ImGui::IsKeyDown(ImGuiKey_A)) camera.transform().translate(-camera.transform().left() * 0.1);
	//if (ImGui::IsKeyDown(ImGuiKey_D)) camera.transform().translate(camera.transform().left() * 0.1);
	//if (ImGui::IsKeyDown(ImGuiKey_Q)) camera.transform().translate(-camera.transform().up() * 0.1);
	//if (ImGui::IsKeyDown(ImGuiKey_E)) camera.transform().translate(camera.transform().up() * 0.1);
	////rotate the transform of the camera
	//
	//if (ImGui::IsKeyDown(ImGuiKey_LeftArrow)) {
	//	camera.transform().rotate(0.02, vec3(0, 1, 0));
	//}
	//if (ImGui::IsKeyDown(ImGuiKey_RightArrow)) {
	//	camera.transform().rotate(-0.02, vec3(0, 1, 0));
	//}
	//if (ImGui::IsKeyDown(ImGuiKey_UpArrow)) camera.transform().rotate(0.02, camera.transform().left());
	//if (ImGui::IsKeyDown(ImGuiKey_DownArrow)) camera.transform().rotate(-0.02, camera.transform().left());




	//Application->window->SwapBuffers();

	return true; 

}

void RenderSceneHierarchy(std::shared_ptr<Scene>& currentScene);

bool MyGUI::PostUpdate() { 
	
	/*ImGui::Render();*/
	//glViewport(100, 300, 100, 100); // Set your viewport
	//glClearColor(0.45f, 0.55f, 0.60f, 1.00f); // Clear color
	//glClear(GL_COLOR_BUFFER_BIT);

	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	//if (Application) { SDL_GL_SwapWindow(Application->window->windowPtr()); } // Swap the window buffer


	return true; 

}
bool MyGUI::CleanUp() { return true; }

void MyGUI::Render() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	RenderSceneHierarchy(Application->root->currentScene);

	if (UIconsolePanel) {
		UIconsolePanel->Draw();
	}

	if (UIsettingsPanel) {
		UIsettingsPanel->Draw();
	}

	if (UIMainMenuBarPanel) {
		UIMainMenuBarPanel->Draw();
	}

	if (UIinspectorPanel) {
		UIinspectorPanel->Draw();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MyGUI::processEvent(const SDL_Event& event) {
	ImGui_ImplSDL2_ProcessEvent(&event);
}

void DrawSceneObject(GameObject& obj);

void RenderSceneHierarchy(std::shared_ptr<Scene>& currentScene) {
	ImGui::Begin("Scene Hierarchy");

	for (auto& obj : currentScene->children()) {
		DrawSceneObject(obj);
	}

	ImGui::End(); 
}


// TODO : Fix forSome Reason only the first button is clickable

void DrawSceneObject(GameObject& obj) {
	// Create a tree node for the current object
	bool open = ImGui::TreeNode(obj.GetName().c_str());

	if (open) {
		// If the node is open, draw its children
		for (auto& child : obj.children()) {
			DrawSceneObject(child); // Recursively draw children
		}
		ImGui::TreePop(); 
	}

	ImGui::SameLine(); // Place a button next to the tree node
	if (ImGui::Button("Remove")) 
	{
		std::cout << "Remove " << obj.GetName();
		//Application->root->RemoveGameObject(std::make_shared<GameObject>(obj.GetName()));
	}
}

