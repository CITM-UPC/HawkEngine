#include "UIHierarchy.h"

#include "App.h"
#include "Root.h"
#include "MyGameEditor/Input.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <iostream>
#include <SDL2/SDL.h>

UIHierarchy::UIHierarchy(UIType type, std::string name) : UIElement(type, name) {
}

UIHierarchy::~UIHierarchy() {
}

bool UIHierarchy::Draw() {
	ImGuiWindowFlags hierarchyFlags = ImGuiWindowFlags_None;

	if (ImGui::Begin("Hierarchy", &enabled, hierarchyFlags)) {
		ImGuiIO& io = ImGui::GetIO();

		Scene* currentScene = Application->root->currentScene.get();

		if (currentScene == nullptr) {
			ImGui::Text("No Scene loaded");
		}

		if (currentScene != nullptr) {
			RenderSceneHierarchy(currentScene);
		}

		if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered()) {
			Application->input->ClearSelection();
		}
	}

	ImGui::End();
	return true;
}


void UIHierarchy::RenderSceneHierarchy(Scene* currentScene) {
	//ImGui::Begin("Scene Hierarchy");

	for (size_t i = 0; i < Application->root->currentScene->children().size(); ++i) {
		DrawSceneObject(*Application->root->currentScene->children()[i]);
	}

	//ImGui::End();
}

void UIHierarchy::DrawSceneObject(GameObject& obj) {
	bool color = false;

	if (obj.isSelected) {
		color = true;
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.5f, 0.0f, 1.0f)); // Orange color for selected
	}

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	// Si el GameObject no tiene hijos, deshabilita el �cono de la flecha
	if (obj.GetChildren().empty()) {
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	bool open = ImGui::TreeNodeEx(obj.GetName().c_str(), flags);

	if (ImGui::IsItemClicked(0)) {
		if (Application->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT) {
			if (obj.isSelected) {
				Application->input->RemoveFromSelection(&obj);
			}
			else {
				Application->input->AddToSelection(&obj);
			}
		}
		else {
			Application->input->ClearSelection();
			Application->input->AddToSelection(&obj);
		}
	}

	if (obj.isSelected && color) {
		ImGui::PopStyleColor(); // Orange color for selected
	}

	if (open) {
		for (auto& child : obj.GetChildren()) { // Usa GetChildren()
			DrawSceneObject(*child); // Pasa el hijo por referencia
		}

		if (!(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
			ImGui::TreePop();
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("Delete")) {
		Application->root->RemoveGameObject(&obj);
	}
}