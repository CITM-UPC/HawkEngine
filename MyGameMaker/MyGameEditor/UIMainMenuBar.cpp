#include "UIMainMenuBar.h"
#include "App.h"
#include "MyGUI.h"
#include <imgui.h>
#include "Input.h"
#include "MyGameEngine/MeshRendererComponent.h"
#include "MyGameEngine/Image.h"
#include "Log.h"
//libraries to open websites
#include <windows.h>
#include <shellapi.h>

UIMainMenuBar::UIMainMenuBar(UIType type, std::string name) : UIElement(type, name)
{
}

UIMainMenuBar::~UIMainMenuBar()
{
}

bool UIMainMenuBar::Draw()
{
	
	if (ImGui::BeginMainMenuBar()) {
		// Inicia el men� "General"


		if (ImGui::BeginMenu("General"))
		{

			if (ImGui::MenuItem("About")) { ShellExecute(0, 0, L"https://github.com/CITM-UPC/HawkEngine", 0, 0, SW_SHOW); }
			if (ImGui::MenuItem("Quit")) { /*Poner aqui codigo de quitear*/ }
			//Aqui abajo mas botones para esconder las diferentes ventanas de Imgui
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GameObjects"))
		{

			if (ImGui::MenuItem("Create EmptyGameObject")) { Application->root->CreateEmptyObject("EmptyGameObject"); }
			if (ImGui::MenuItem("Create Cube")) { Application->root->CreateCubeObject("Cube"); }
			if (ImGui::MenuItem("Create Sphere")) { Application->root->CreateSphereObject("Sphere"); }
			if (ImGui::MenuItem("Create Plane")) { Application->root->CreatePlaneObject("Plane"); }
			if (ImGui::MenuItem("SetCheckerTexture")) { Application->input->GetSelectedGameObject()->GetComponent<MeshRenderer>()->GetImage()->LoadCheckerTexture(); }
			ImGui::EndMenu();
		}
		// Finaliza la barra de men� principal
		ImGui::EndMainMenuBar();
	}
	
	return true;
}