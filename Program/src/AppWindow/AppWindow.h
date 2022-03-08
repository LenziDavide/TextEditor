#pragma once
#include <string>

class __declspec(dllexport) AppWindow
{
public:
	AppWindow();
	virtual ~AppWindow();

	void Run(char** openpath);

	void ReadSizePosition();
	void WriteSizePosition();

	void SetWindowTitle(const char* title);
	void SetWindowSize(int sizeX, int sizeY);
	void SetWindowPos(int posX, int posY);
	void SetVSync(bool enabled);

	int GetWindowSizeX() const { return WindowSizeX; }
	int GetWindowSizeY() const { return WindowSizeY; }

	int GetWindowPosX() const { return WindowPosX; }
	int GetWindowPosY() const { return WindowPosY; }
private:
	static inline std::string WindowTitle = "Text-Editor";

	static inline bool vSync = false;

	static inline int WindowSizeX = 800;
	static inline int WindowSizeY = 600;

	static inline int WindowPosX = 200;
	static inline int WindowPosY = 200;
private:
	static inline std::string IniSection = "Window";
	std::string appdata = getenv("APPDATA");
	std::string folderToCreate = appdata + "\\Text-Editor";

	std::string fileToWrite = appdata + "\\Text-Editor\\configs.ini";
};