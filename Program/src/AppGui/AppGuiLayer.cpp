#include "AppGuiLayer.h"

#include "AppFont.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>

#include <GLFW/glfw3.h>

void AppGuiLayer::OnAttach(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = NULL;

	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;

	io.Fonts->AddFontFromMemoryCompressedTTF(texteditor_compressed_data, texteditor_compressed_size, 16.0f);
	io.Fonts->AddFontFromMemoryCompressedTTF(segoe_compressed_data, segoe_compressed_size, 16.0f);
	io.FontDefault = io.Fonts->AddFontFromMemoryCompressedTTF(fontawesome_compressed_data, fontawesome_compressed_size, 14.0f, &icons_config, icons_ranges);
	
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL2_Init();
}

void AppGuiLayer::Begin()
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void AppGuiLayer::End()
{
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void AppGuiLayer::OnDetach()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}