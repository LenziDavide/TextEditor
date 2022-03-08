#pragma once
#include "AppIncludes.h"
#include "AppGui/AppGuiWidgets.h"
#include "AppSaveRead/AppSaveRead.h"

class AppContent
{
public:
	void Init(const char* openPath);

	void DragDropFile(const char* filePath);

	bool GetFileEdited() const { return FileEdited; };
	std::string GetFileName() const { return FileName; }

	void SaveBefExit();

	void Shortcuts();

	void ReadIni();
	void WriteIni();
private:
	void Init_Infobar_Menubar();
	bool BeginInfoBar();
private:
	std::string OpenFileDialog(const char* filter);
	std::string SaveFileDialog(const char* filter);
	void PrintDialog();
	void FileNew();
	void FileOpen();
	void FileSave();
	void FileSaveDlg();
	void FileSaveAs();
	void FileClose();
private:
	void AboutPopup();
private:
	static inline const char* appdataloc = getenv("APPDATA");
	static inline std::string iniLocation = "\\Text-Editor\\configs.ini";
	static inline const char* iniSection = "GENERAL";
	static inline std::string path = appdataloc + iniLocation;
private:
	ReadWrite ini;

	static inline bool readonly = false;
	static inline bool overwrite = false;
	static inline bool whitespaces = false;

	static inline bool FileEdited = false;
	static inline bool FileOpened = false;
	static inline bool FileCreated = false;
	static inline bool FileSaved = false;
	static inline std::string FileName = "Untitled.txt";
	static inline std::string FilePath = "";
	static inline std::string FileContent = "";

	static inline bool About = false;

	static inline bool pathinStatusbar = false;

	static inline bool recentFilesAvail;

	AppGuiWidgets widgets;

	static inline std::string AppVersion = "1.0.0";

	static inline bool darkTheme = true;
	static inline bool statusBar = true;

	//SAVEFILEMESSAGEBOX
	static inline std::string Title = "Save";
	static inline std::string Desc = "Do you want to save " + FileName + "?";
};