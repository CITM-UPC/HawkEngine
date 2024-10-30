#include "UIInspector.h"
#include "App.h"
#include "MyGUI.h"

#include "..\MyGameEngine\TransformComponent.h"
#include "..\MyGameEngine\Mesh.h"
#include "Camera.h"
#include "Input.h"
#include "..\MyGameEngine\MeshRendererComponent.h"
#include "..\MyGameEngine\Mesh.h"
#include "..\MyGameEngine\Image.h"

#include <imgui.h>
#include <imgui_internal.h>

UIInspector::UIInspector(UIType type, std::string name) : UIElement(type, name)
{
	matrixDirty = false;

	position = { 0, 0 ,0 };
	rotationRadians = { 0, 0, 0 };
	scale = { 1, 1, 1 };

}

UIInspector::~UIInspector()
{
}

bool UIInspector::Draw()
{
	ImGuiWindowFlags inspectorFlags = ImGuiWindowFlags_NoFocusOnAppearing;
	
	if (ImGui::Begin("Inspector", &enabled, inspectorFlags))
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::SetNextWindowSize(ImVec2(250, 650), ImGuiCond_Once);

		auto selectedGameObject = Application->input->GetSelectedGameObject();

		if (Application->input->GetSelectedGameObject() == nullptr)
		{
			ImGui::Text("No GameObject selected");
		}

		if (selectedGameObject != nullptr)
		{
			ImGui::SameLine(); ImGui::Text("GameObject:");
			ImGui::SameLine(); ImGui::Text(selectedGameObject->GetName().c_str());

			std::shared_ptr<Transform_Component> transform = selectedGameObject->GetTransform();

			if (transform)
			{
				ImGui::Text("Transform");
				ImGui::Separator();

				glm::dvec3 currentPosition = transform->GetPosition();
				glm::dvec3 currentRotation = transform->GetEulerAngles();
				glm::dvec3 currentScale = transform->GetScale();

				float pos[3] = { static_cast<float>(currentPosition.x), static_cast<float>(currentPosition.y), static_cast<float>(currentPosition.z) };
				// Ojo que hay que castear a grados creo
				float rot[3] = { static_cast<float>(currentRotation.x), static_cast<float>(currentRotation.y), static_cast<float>(currentRotation.z) };
				float sca[3] = { static_cast<float>(currentScale.x), static_cast<float>(currentScale.y), static_cast<float>(currentScale.z) };

				if (ImGui::DragFloat3("Postition", pos, 0.1f))
				{
					glm::dvec3 newPosition = { pos[0], pos[1], pos[2] };
					glm::dvec3 deltaPos = newPosition - currentPosition;
					transform->Translate(deltaPos);
				}

				if (ImGui::DragFloat3("Rotation", rot, 0.1f))
				{
					glm::dvec3 newRotation = { rot[0], rot[1], rot[2] };
					glm::dvec3 deltaRot = newRotation - currentRotation;
					transform->Rotate(deltaRot.x, { 1, 0, 0 });
					transform->Rotate(deltaRot.y, { 0, 1, 0 });
					transform->Rotate(deltaRot.z, { 0, 0, 1 });
				}

				if (ImGui::DragFloat3("Scale", sca, 0.1f))
				{
					glm::dvec3 newScale = { sca[0], sca[1], sca[2] };
					glm::dvec3 deltaScale = newScale - currentScale;
					transform->Scale(deltaScale);
				}

				// Methods to rotate and scale

			}

			ImGui::Separator();

			std::shared_ptr<Mesh> mesh = selectedGameObject->GetComponent<MeshRenderer>()->GetMesh();

			if (mesh)
			{
				ImGui::Text("Mesh");
				ImGui::Separator();

			}

			ImGui::Separator();

			std::shared_ptr<Image> image = selectedGameObject->GetComponent<MeshRenderer>()->GetImage();
			if (image)
			{
				ImGui::Text("Image");
				ImGui::Separator();
				ImGui::Text("Width: %d", image->width());
				ImGui::Text("Heigth: %d", image->width());
			}
		}

		ImGui::End();
	}

	return true;
}
