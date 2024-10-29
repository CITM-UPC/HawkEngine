#include "UISettings.h"
#include "App.h"
#include "MyGUI.h"
#include "Input.h"
#include "HardwareSettings.h"

#include "imgui.h"


UISettings::UISettings(UIType type, std::string name) : UIElement(type, name), fpsRecord(RECORD_SIZE), msRecord(RECORD_SIZE) 
{
}


UISettings::~UISettings()
{
}

void UISettings::AddFpsValue(int fps)
{
	fpsRecord.push_back(fps);
	
	if (fpsRecord.size() > RECORD_SIZE)
	{
		fpsRecord.erase(fpsRecord.begin());
	}
}

bool UISettings::Draw()
{
	ImGuiWindowFlags settingsFlags = 0;
	settingsFlags = ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

	ImVec2 mainViewportPos = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(ImVec2(mainViewportPos.x, mainViewportPos.y), ImGuiCond_Appearing, ImVec2(0.5, 0.8));

	if (ImGui::Begin("Settings", &enabled, settingsFlags))
	{
		ImVec2 leftChildSize = ImVec2(ImGui::GetWindowSize().x * 0.3f, ImGui::GetWindowSize().y);
		ImVec2 buttonSize = ImVec2(ImGui::GetWindowSize().x * 0.2, 20);

		if (ImGui::BeginChild("Settings", leftChildSize, true))
		{
			if (ImGui::Button("Performance", buttonSize))
			{
				settingType = SettingType::PERFORMANCE;
			}

			if (ImGui::Button("Hardware", buttonSize))
			{
				settingType = SettingType::HARDWARE;
			}

			if (ImGui::Button("Software", buttonSize))
			{
				settingType = SettingType::SOFTWARE;
			}

			ImGui::EndChild();
		}

		ImGui::SameLine();

		settingsFlags &= ~(ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		if (ImGui::BeginChild("Settings Type", ImVec2(ImGui::GetWindowSize().x * 0.8f, ImGui::GetWindowSize().y), false, settingsFlags))
		{
			switch (settingType)
			{
			case SettingType::PERFORMANCE:
				PerformanceData();
				break;
			case SettingType::HARDWARE:
				HardwareData();
				break;
			case SettingType::SOFTWARE:
				SoftwareData();
				break;
			default:
				break;
			}

			ImGui::EndChild();
		}

		ImGui::End();

	}

	return true;
}

void UISettings::PerformanceData()
{
	//int frameRate = Application->GetFrameRate();

	// Plotting FPS (need library)
}

void UISettings::HardwareData()
{
	// Esto lo ha hecho ChatGPT, ni de co�a lo hago a mano
	HardwareSettings settings = Application->hardwareInfo->GetSettings();

	ImGui::Text("Hardware Information");
	ImGui::Separator();

	// CPU Information
	ImGui::Text("CPU:");
	ImGui::BulletText("Cores: %d", settings.cpuCores);
	ImGui::BulletText("Cache Line Size: %d", settings.cacheLineSize);
	ImGui::BulletText("RDTSC: %s", settings.rdtsc ? "Supported" : "Not Supported");
	ImGui::BulletText("AltiVec: %s", settings.altiVec ? "Supported" : "Not Supported");
	ImGui::BulletText("3DNow: %s", settings.now3d ? "Supported" : "Not Supported");
	ImGui::BulletText("MMX: %s", settings.mmx ? "Supported" : "Not Supported");
	ImGui::BulletText("SSE: %s", settings.sse ? "Supported" : "Not Supported");
	ImGui::BulletText("SSE2: %s", settings.sse2 ? "Supported" : "Not Supported");
	ImGui::BulletText("SSE3: %s", settings.sse3 ? "Supported" : "Not Supported");
	ImGui::BulletText("SSE4.1: %s", settings.sse41 ? "Supported" : "Not Supported");
	ImGui::BulletText("SSE4.2: %s", settings.sse42 ? "Supported" : "Not Supported");
	ImGui::BulletText("AVX: %s", settings.avx ? "Supported" : "Not Supported");
	ImGui::BulletText("AVX2: %s", settings.avx2 ? "Supported" : "Not Supported");

	ImGui::Separator();

	// Memory Information
	ImGui::Text("Memory:");
	ImGui::BulletText("RAM: %.2f GB", settings.ram);

	ImGui::Separator();

	// GPU Information
	ImGui::Text("GPU:");
	ImGui::BulletText("Vendor: %s", settings.gpuVendor.c_str());
	ImGui::BulletText("Brand: %s", settings.gpuBrand.c_str());
	ImGui::BulletText("Driver Version: %s", settings.gpuDriverVersion.c_str());
	ImGui::BulletText("VRAM Budget: %.2f MB", settings.vramBudget);
	ImGui::BulletText("VRAM Available: %.2f MB", settings.vramAvailable);
	ImGui::BulletText("VRAM Usage: %.2f MB", settings.vramUsage);

	ImGui::Separator();

	// VRAM Information
	ImGui::Text("VRAM:");
	ImGui::BulletText("Total Budget: %.2f MB", settings.vramBudget);
	ImGui::BulletText("Available: %.2f MB", settings.vramAvailable);
	ImGui::BulletText("Usage: %.2f MB", settings.vramUsage);

	ImGui::Separator();

}

void UISettings::SoftwareData()
{
	HardwareSettings settings = Application->hardwareInfo->GetSettings();

	ImGui::Text("Software Information");
	ImGui::Separator();

	// SDL Version
	ImGui::Text("SDL:");
	ImGui::BulletText("Version: %s", settings.sdlVersion);

	ImGui::Separator();

	// OpenGL Information
	ImGui::Text("OpenGL:");
	ImGui::BulletText("Vendor: %s", settings.gpuVendor.c_str());
	ImGui::BulletText("Renderer: %s", settings.gpuBrand.c_str());
	ImGui::BulletText("Version: %s", settings.gpuDriverVersion.c_str());

	ImGui::Separator();
}