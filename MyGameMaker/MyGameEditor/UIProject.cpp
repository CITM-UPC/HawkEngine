#include "UIProject.h"
#include "App.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "MyGUI.h"
#include "../MyGameEngine/types.h"

#include <filesystem>
#include <string>
#include <fstream>

#define BIT(x) (1 << x)

UIProject::UIProject(UIType type, std::string name) : UIElement(type, name)
{
	// Definir el path del directorio (cambiar en types.h)
	directoryPath = ASSETS_PATH;
	currentSceneFile = "";
}

UIProject::~UIProject()
{
}


bool UIProject::Draw()
{
	ImGuiWindowFlags projectFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_None;
	ImGuiWindowClass windowClass;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	if (firstDraw)
	{
		ImGui::SetNextWindowSize(ImVec2(300, 600), ImGuiCond_FirstUseEver);
		firstDraw = false;
	}

	ImGui::SetNextWindowClass(&windowClass);
	windowClass.DockingAllowUnclassed = false;

	if (ImGui::Begin("Project", &enabled, projectFlags))
	{
		static ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit;

		if (ImGui::BeginTable("Table", 2, tableFlags))
		{
			ImGui::TableNextColumn();
			if (ImGui::CollapsingHeader("Assets"), ImGuiTreeNodeFlags_DefaultOpen)
			{
				uint32_t count = 0;
				// Esto no s� si est� bien
				for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath))
				{
					count++;
				}

				static int selectionMask = 0;

				auto clickState = DirectoryView(directoryPath, &count, &selectionMask);

				if (clickState.first) // Esto es para la selecci�n m�ltiple
				{
					if (ImGui::GetIO().KeyCtrl)
					{
						selectionMask ^= BIT(clickState.second);
					}
					else
					{
						selectionMask = BIT(clickState.second);
					}
				}

			}

			ImGui::TableNextColumn();

			// Aqu� creo que va algo
			ImGui::Text("Properties or Preview here...");
			ImGui::EndTable();
		}

		ImGui::PopStyleVar();
		ImGui::End();
	}

	return true;
}

std::pair<bool, uint32_t> UIProject::DirectoryView(const std::filesystem::path& path, uint32_t* count, int* selection_mask)
{
	std::pair<bool, uint32_t> ret = { false, 0 };

	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	bool anyNodeClicked = false;
	uint32_t nodeClicked = 0;

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		ImGuiTreeNodeFlags treeFlags = nodeFlags;
		const bool isSelected = (*selection_mask & BIT(*count)) != 0;
		if (isSelected)
		{
			treeFlags |= ImGuiTreeNodeFlags_Selected;
		}

		std::string name = entry.path().filename().string();
		
		bool entryIsFile = !std::filesystem::is_directory(entry.path());
		
		if (entryIsFile)
		{
			treeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		}

		bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)(*count), treeFlags, name.c_str());

		if (entryIsFile && ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			const std::string filePath = entry.path().string();
			ImGui::SetDragDropPayload("FilePath", filePath.c_str(), filePath.size() + 1);
			ImGui::Text("Dragging: %s", filePath.c_str());
			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemClicked())
		{
			nodeClicked = *count;
			anyNodeClicked = true;
			HandleFileSelection(entry.path().string());
		}

		if (ImGui::BeginPopupContextItem(name.c_str()))
		{
			if (name.ends_with(".scene"))
			{
				if (ImGui::MenuItem("Save & Load Scene"))
				{
					Application->scene_serializer->Serialize(currentSceneFile);
					Application->scene_serializer->DeSerialize(entry.path().string());
					currentSceneFile = entry.path().string();
				}
				if (ImGui::MenuItem("Load Scene without saving"))
				{
					Application->scene_serializer->DeSerialize(entry.path().string());
					currentSceneFile = entry.path().string();
				}
			}

			if (ImGui::MenuItem("Delete"))
			{
				std::filesystem::remove_all(entry.path());
			}

			ImGui::EndPopup();
		}
	
		(*count)--;
		
		if (!entryIsFile)
		{
			if (nodeOpen)
			{
				auto clickState = DirectoryView(entry.path(), count, selection_mask);
			
				if (!anyNodeClicked)
				{
					anyNodeClicked = clickState.first;
					nodeClicked = clickState.second;
				}

				ImGui::TreePop();
			}
		}

	}

	return { anyNodeClicked, nodeClicked };
}

void UIProject::HandleFileSelection(const std::string& filePath)
{
	if (filePath.ends_with(".scene"))
	{
		currentSceneFile = filePath;
		ImGui::OpenPopup(("Load Scene"));
	}

	if (ImGui::BeginPopupModal("Load Scene"))
	{
		if (ImGui::Button("Save & Load"))
		{
			Application->scene_serializer->Serialize(currentSceneFile);
			Application->scene_serializer->DeSerialize(filePath);
			currentSceneFile = filePath;
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("Load without saving"))
		{
			Application->scene_serializer->DeSerialize(filePath);
			currentSceneFile = filePath;
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}





