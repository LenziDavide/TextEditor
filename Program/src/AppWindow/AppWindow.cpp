#include "AppWindow.h"
#include "AppIncludes.h"

#include <resource.h>

#include "AppTexture/stb_image.h"

#include "AppGui/AppGuiLayer.h"
std::unique_ptr<AppGuiLayer> AppGui;

#include "AppContent/AppContent.h"
std::unique_ptr<AppContent> Content;

#include "AppSaveRead/AppSaveRead.h"
std::unique_ptr<ReadWrite> IniFile;

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
GLFWwindow* m_Window;

AppWindow::AppWindow()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	std::filesystem::create_directory(folderToCreate);

	glfwInit();

	ReadSizePosition();
	m_Window = glfwCreateWindow(WindowSizeX, WindowSizeY, WindowTitle.c_str(), 0, 0);

	glfwSetWindowPos(m_Window, WindowPosX, WindowPosY);
	
	HWND hwnd = glfwGetWin32Window(m_Window);

	HANDLE hIcon = LoadIconW(GetModuleHandleW(NULL), L"GLFW_ICON");
	if (!hIcon)
	{
		hIcon = static_cast<HICON>(::LoadImage(GetModuleHandleW(NULL),
			MAKEINTRESOURCE(IDI_ICON1),
			IMAGE_ICON,
			256, 256,    
			LR_DEFAULTCOLOR));
	}
	
	::SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	::SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

	glfwSetWindowSizeLimits(m_Window, 450, 400, 5000, 4000);

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
	{
		Content->SaveBefExit();
	});

	glfwSetDropCallback(m_Window, [](GLFWwindow* window, int count, const char** paths)
	{
		wchar_t* fileName = NULL;
		UINT longestFileNameLength = 0;

		for (int i = 0; i < count; i++)
		{
			Content->DragDropFile(*paths);
		}
		free(fileName);
	});

	glfwMakeContextCurrent(m_Window);
	SetVSync(true);

	AppGui->OnAttach(m_Window);
}

void AppWindow::Run(char** openpath)
{
	const char* openPath = openpath[1];

	while (!glfwWindowShouldClose(m_Window))
	{
		glfwPollEvents();

		AppGui->Begin();

		Content->Shortcuts();
		Content->ReadIni();
		Content->Init(openPath);
		Content->WriteIni();

		AppGui->End();

		glfwGetWindowSize(m_Window, &WindowSizeX, &WindowSizeY);
		glfwGetWindowPos(m_Window, &WindowPosX, &WindowPosY);
		WriteSizePosition();
		glfwSwapBuffers(m_Window);
	}
}

AppWindow::~AppWindow()
{
	AppGui->OnDetach();
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void AppWindow::ReadSizePosition()
{
	WindowSizeX = IniFile->ReadInteger(fileToWrite.c_str(), IniSection.c_str(), "SizeX", 800);
	WindowSizeY = IniFile->ReadInteger(fileToWrite.c_str(), IniSection.c_str(), "SizeY", 600);
	WindowPosX = IniFile->ReadInteger(fileToWrite.c_str(), IniSection.c_str(), "PosX", 200);
	WindowPosY = IniFile->ReadInteger(fileToWrite.c_str(), IniSection.c_str(), "PosY", 200);
}

void AppWindow::WriteSizePosition()
{
	IniFile->WriteInteger(fileToWrite.c_str(), IniSection.c_str(), "SizeX", WindowSizeX);
	IniFile->WriteInteger(fileToWrite.c_str(), IniSection.c_str(), "SizeY", WindowSizeY);
	IniFile->WriteInteger(fileToWrite.c_str(), IniSection.c_str(), "PosX", WindowPosX);
	IniFile->WriteInteger(fileToWrite.c_str(), IniSection.c_str(), "PosY", WindowPosY);
}

void AppWindow::SetWindowTitle(const char* title)
{
	WindowTitle = title;

	glfwSetWindowTitle(m_Window, WindowTitle.c_str());
}

void AppWindow::SetWindowSize(int sizeX, int sizeY)
{
	WindowSizeX = sizeX;
	WindowSizeY = sizeY;

	glfwSetWindowSize(m_Window, WindowSizeX, WindowSizeY);
}

void AppWindow::SetWindowPos(int posX, int posY)
{
	WindowPosX = posX;
	WindowPosY = posY;

	glfwSetWindowPos(m_Window, WindowPosX, WindowPosY);
}

void AppWindow::SetVSync(bool enabled)
{
	vSync = enabled;

	if (enabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
}