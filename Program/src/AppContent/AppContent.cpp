#include "AppContent.h"
#include "AppIncludes.h"
#include "AppGui/AppFont.h"

#include "AppTexture/AppTexture.h"
std::unique_ptr<Texture> AppTexture;

#include "AppGui/AppGuiStyle.h"
std::unique_ptr<AppGuiStyle> GuiStyle;

#include "AppWindow/AppWindow.h"
std::unique_ptr<AppWindow> AppWnd;

#include "AppTextEditor/AppTextEditor.h"
static TextEditor Editor;

#include <imgui.h>
#include <imgui_internal.h>

void AppContent::Init(const char* openPath)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus;

	const static TextEditor::Palette darkPalette = { {
		0xfff6f6f6,	// Default
		0xffd69c56,	// Keyword	
		0xff00ff00,	// Number
		0xff7070e0,	// String
		0xff70a0e0, // Char literal
		0xffffffff, // Punctuation
		0xff408080,	// Preprocessor
		0xffaaaaaa, // Identifier
		0xff9bc64d, // Known identifier
		0xffc040a0, // Preproc identifier
		0xff206020, // Comment (single line)
		0xff406020, // Comment (multi line)
		ImColor(20, 20, 20, 255), // Background
		0xffe0e0e0, // Cursor
		0x80a06020, // Selection
		0x800020ff, // ErrorMarker
		0x40f08000, // Breakpoint
		ImColor(0, 120, 190, 255), // Line number
		0x40000000, // Current line fill
		0x40808080, // Current line fill (inactive)
		0x40a0a0a0, // Current line edge
	} };
	const static TextEditor::Palette lightPalette = { {
		ImColor(40, 40, 40,255),	// None
		0xffff0c06,	// Keyword	
		0xff008000,	// Number
		0xff2020a0,	// String
		0xff304070, // Char literal
		0xff000000, // Punctuation
		0xff406060,	// Preprocessor
		0xff404040, // Identifier
		0xff606010, // Known identifier
		0xffc040a0, // Preproc identifier
		0xff205020, // Comment (single line)
		0xff405020, // Comment (multi line)
		0xffffffff, // Background
		0xff000000, // Cursor
		0x80600000, // Selection
		0xa00010ff, // ErrorMarker
		0x80f08000, // Breakpoint
		ImColor(0, 120, 190, 255), // Line number
		0x40000000, // Current line fill
		0x40808080, // Current line fill (inactive)
		0x40000000, // Current line edge
	} };
	const static TextEditor::Palette redPalette = { {
		0xfff6f6f6,	// Default
		0xffd69c56,	// Keyword	
		0xff00ff00,	// Number
		0xff7070e0,	// String
		0xff70a0e0, // Char literal
		0xffffffff, // Punctuation
		0xff408080,	// Preprocessor
		0xffaaaaaa, // Identifier
		0xff9bc64d, // Known identifier
		0xffc040a0, // Preproc identifier
		0xff206020, // Comment (single line)
		0xff406020, // Comment (multi line)
		ImColor(20, 20, 20, 255), // Background
		0xffe0e0e0, // Cursor
		0x80a06020, // Selection
		0x800020ff, // ErrorMarker
		0x40f08000, // Breakpoint
		ImColor(150, 20, 20, 255), // Line number
		0x40000000, // Current line fill
		0x40808080, // Current line fill (inactive)
		0x40a0a0a0, // Current line edge
	} };

	style.FrameRounding = 0.0f;
	style.ChildRounding = 0.0f;
	style.PopupRounding = 0.0f;
	style.WindowRounding = 0.0f;
	style.DisplaySafeAreaPadding = ImVec2(0.0f,0.0f);

	style.FrameBorderSize = 1.0f;

	if (darkTheme == true)
	{
		Editor.SetPalette(darkPalette);
		GuiStyle->DarkTheme();
	}
	if (darkTheme == false)
	{
		Editor.SetPalette(lightPalette);
		GuiStyle->LightTheme();
	}

	ImGui::SetNextWindowSize(ImVec2(viewport->Size), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(viewport->Pos), ImGuiCond_Always);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	if (ImGui::Begin("##mainWnd", NULL, flags))
	{
		if (Editor.CanUndo())
			FileEdited = true;
		else
			FileEdited = false;

		if (FileSaved)
			FileEdited = false;

		if (FileSaved && Editor.IsTextChanged())
		{
			FileSaved = false;
			FileEdited = Editor.CanUndo();
		}

		std::string WindowTitle = FileName + " - " + "Text-Editor";
		std::string WindowTitleEdited = "*" + FileName + " - " + "Text-Editor";
		if (FileEdited)
		{
			AppWnd->SetWindowTitle(WindowTitleEdited.c_str());
		}
		else
		{
			AppWnd->SetWindowTitle(WindowTitle.c_str());
		}

		Init_Infobar_Menubar();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);

		Editor.SetReadOnly(readonly);
		Editor.SetShowWhitespaces(whitespaces);
		Editor.SetColorizerEnable(false);
		Editor.SetLanguageDefinition(TextEditor::LanguageDefinition::LanguageDefinition());
		std::string FileToEdit = Editor.GetText();
		ImFont* editorFont = io.Fonts->Fonts[0];
		ImGui::PushFont(editorFont);
		Editor.Render("TextEditor", ImVec2(0.0f, statusBar ? io.DisplaySize.y - 45 : io.DisplaySize.y - 20));
		ImGui::PopFont();

		static bool fileOpened = false;
		if (openPath != NULL && !FileOpened && !fileOpened)
		{
			std::filesystem::path fileName = openPath;

			FileName = fileName.filename().string();
			FilePath = fileName.string();

			static const char* filetoedit = openPath;
			std::ifstream t(filetoedit);
			std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			Editor.SetText(str);

			FileOpened = true;
			fileOpened = true;
		}

		FileCreated = true;

		FileContent = FileToEdit.c_str();

		AboutPopup();

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			if (ImGui::IsPopupOpen("##editToolmenu"))
				ImGui::CloseCurrentPopup();

			Sleep(5);

			ImGui::OpenPopup("##editToolmenu");
		}
		ImGui::PopStyleVar();
	}
	ImGui::End();
	ImGui::PopStyleVar(3);
}

void AppContent::Init_Infobar_Menubar()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 7.0f));

	if (ImGui::BeginMenuBar())
	{
		ImGui::SetCursorPosX(4.0f);
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItemEx("New", " " ICON_FA_FILE, "\tCTRL+N"))
				FileNew();
			ImGui::Separator();
			if (ImGui::MenuItemEx("Open", " " ICON_FA_FOLDER, "\tCTRL+O"))
				FileOpen();
			std::string recentFile = ini.ReadString(path.c_str(), iniSection, "RecentFile", "");
			if (ImGui::BeginMenuEx("Open Recent", " " ICON_FA_FOLDER_OPEN))
			{
				recentFilesAvail = ini.ReadBoolean(path.c_str(), iniSection, "RecentFileAvail", true);

				if (FilePath > "")
					recentFilesAvail = true;

				if (recentFilesAvail)
				{
					std::string recentFileToDisplay = recentFile;
					if (ImGui::MenuItemEx(recentFileToDisplay.c_str(), NULL))
					{
						static bool exists = std::filesystem::exists(recentFile.c_str());
						if (!exists)
						{
							int MB = MessageBox(NULL, "Couldn't open the file, check the file exists. Do you want to remove it from Recent Files", "Error", MB_YESNO | MB_ICONERROR);

							switch (MB)
							{
							case IDYES:
								recentFilesAvail = false;
							}
						}
						else
						{
							FileOpened = true;
							std::filesystem::path FILE = recentFile.c_str();
							FilePath = FILE.string().c_str();
							FileName = FILE.filename().string().c_str();
							static const char* filetoedit = FilePath.c_str();
							std::ifstream t(filetoedit);
							std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
							Editor.SetText(str);
						}
					}
				}
				else
				{
					ImGui::TextDisabled("NO RECENT FILES");
				}

				ImGui::Separator();

				ImGui::BeginDisabled(!recentFilesAvail);
				if (ImGui::MenuItemEx("Remove Recent Files", NULL))
				{
					recentFilesAvail = false;
				}
				ImGui::EndDisabled();

				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::MenuItemEx("Save", " " ICON_FA_FLOPPY_O ,"\tCTRL+S"))
			{
				if (FilePath == "")
					FileSaveDlg();
				else
					FileSave();
			}
			if (ImGui::MenuItemEx("Save As", " " ICON_FA_PENCIL_SQUARE_O, "\tCTRL+SHIFT+S"))
				FileSaveAs();
			ImGui::Separator();
			if (ImGui::MenuItemEx("Close", " " ICON_FA_TIMES_CIRCLE, "\tCTRL+SHIFT+C"))
				FileClose();
			ImGui::Separator();
			if (ImGui::MenuItemEx("Print", " " ICON_FA_PRINT, "\tCTRL+P"))
				PrintDialog();
			ImGui::Separator();
			if (ImGui::MenuItemEx("Exit", " " ICON_FA_POWER_OFF,"\tALT+F4"))
				exit(0);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItemEx("Undo", " " ICON_FA_UNDO, "\tCTRL+Z", false, !Editor.IsReadOnly() && Editor.CanUndo()))
				Editor.Undo();

			if (ImGui::MenuItemEx("Redo", " " ICON_FA_REPEAT,"\tCTRL+Y", false, !Editor.IsReadOnly() && Editor.CanRedo()))
				Editor.Redo();

			ImGui::Separator();
			if (ImGui::MenuItemEx("Copy", " " ICON_FA_FILES_O,"\tCTRL+C", false, !Editor.IsReadOnly() && Editor.HasSelection()))
				Editor.Copy();

			if (ImGui::MenuItemEx("Cut", " " ICON_FA_SCISSORS,"\tCTRL+X", false, !Editor.IsReadOnly() && Editor.HasSelection()))
				Editor.Cut();

			if (ImGui::MenuItemEx("Paste", " " ICON_FA_CLIPBOARD,"\tCTRL+V", false, !Editor.IsReadOnly()))
				Editor.Paste();

			if (ImGui::MenuItemEx("Select All", " " ICON_FA_CLONE, "\tCTRL+A"))
				Editor.SelectAll();

			ImGui::Separator();
			if (ImGui::MenuItemEx("Delete", " " ICON_FA_TRASH, "\tCANC", false, !Editor.IsReadOnly() && Editor.HasSelection()))
			{
				Editor.Delete();
			}
			ImGui::Separator();

			if (ImGui::MenuItemEx("Search on browser", " " ICON_FA_GLOBE, "\t\t", false, Editor.HasSelection()))
			{
				std::string searchUrl = "https://www.bing.com/search?q=" + Editor.GetSelectedText();
				ShellExecute(NULL, "open", searchUrl.c_str(), NULL, NULL, NULL);
			}
			ImGui::Separator();
			if (ImGui::MenuItemEx("Add time and date", " " ICON_FA_CLOCK_O, "\tF5"))
			{
				time_t rawtime;
				struct tm* TIME;
				time(&rawtime);

				auto cpos = Editor.GetCursorPosition();

				char completeTimeandDate[20] = { 0 };
				strftime(completeTimeandDate, sizeof(completeTimeandDate), "%H:%M %d/%m/%Y", localtime(&rawtime));

				std::string stringToSet = completeTimeandDate;
				Editor.InsertText(stringToSet);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItemEx("Open New Window", " " ICON_FA_LAPTOP, "\tCTRL+SHIFT+N"))
				system("start Text-Editor.exe");
			ImGui::Separator();

			if (ImGui::MenuItem("Overwrite", overwrite ? "\t\t" ICON_FA_TOGGLE_ON : "\t\t" ICON_FA_TOGGLE_OFF))
			{
				overwrite = !overwrite;
				Editor.SetOverwrite(overwrite);
			}

			if (ImGui::MenuItem("Read Only", readonly ? "\t\t" ICON_FA_TOGGLE_ON : "\t\t" ICON_FA_TOGGLE_OFF, false, FileCreated || FileSaved || FileOpened))
				readonly = !readonly;

			if (ImGui::MenuItem("Show Whitespaces", whitespaces ? "\t\t" ICON_FA_TOGGLE_ON : "\t\t" ICON_FA_TOGGLE_OFF))
				whitespaces = !whitespaces;

			ImGui::Separator();
			ImGui::MenuItem("Preferences:", "", false, false);
			ImGui::Separator();

			if (ImGui::MenuItem("Statusbar", statusBar ? "\t\t" ICON_FA_TOGGLE_ON : "\t\t" ICON_FA_TOGGLE_OFF))
				statusBar = !statusBar;

			if (statusBar)
				if (ImGui::MenuItem("File Path in Statusbar", pathinStatusbar ? "\t\t" ICON_FA_TOGGLE_ON : "\t\t" ICON_FA_TOGGLE_OFF))
					pathinStatusbar = !pathinStatusbar;

			if (ImGui::MenuItem("Dark Theme", darkTheme ? "\t\t" ICON_FA_TOGGLE_ON : "\t\t" ICON_FA_TOGGLE_OFF))
				darkTheme = !darkTheme;
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItemEx("About Text-Editor", " " ICON_FA_INFO_CIRCLE))
				About = true;

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	if (ImGui::BeginPopup("##editToolmenu", ImGuiWindowFlags_NoMove))
	{
		if (ImGui::MenuItemEx("Undo", " " ICON_FA_UNDO, "\tCTRL+Z", false, !Editor.IsReadOnly() && Editor.CanUndo()))
			Editor.Undo();

		if (ImGui::MenuItemEx("Redo", " " ICON_FA_REPEAT, "\tCTRL+Y", false, !Editor.IsReadOnly() && Editor.CanRedo()))
			Editor.Redo();

		ImGui::Separator();
		if (ImGui::MenuItemEx("Copy", " " ICON_FA_FILES_O, "\tCTRL+C", false, !Editor.IsReadOnly() && Editor.HasSelection()))
			Editor.Copy();

		if (ImGui::MenuItemEx("Cut", " " ICON_FA_SCISSORS, "\tCTRL+X", false, !Editor.IsReadOnly() && Editor.HasSelection()))
			Editor.Cut();

		if (ImGui::MenuItemEx("Paste", " " ICON_FA_CLIPBOARD, "\tCTRL+V", false, !Editor.IsReadOnly()))
			Editor.Paste();

		if (ImGui::MenuItemEx("Select All", " " ICON_FA_CLONE, "\tCTRL+A"))
			Editor.SelectAll();

		ImGui::Separator();
		if (ImGui::MenuItemEx("Delete", " " ICON_FA_TRASH, "\tCANC", false, !Editor.IsReadOnly() && Editor.HasSelection()))
		{
			Editor.Delete();
		}
		ImGui::Separator();

		if (ImGui::MenuItemEx("Search on browser", " " ICON_FA_GLOBE, "\t\t", false, Editor.HasSelection()))
		{
			std::string searchUrl = "https://www.bing.com/search?q=" + Editor.GetSelectedText();
			ShellExecute(NULL, "open", searchUrl.c_str(), NULL, NULL, NULL);
		}
		ImGui::Separator();
		if (ImGui::MenuItemEx("Add time and date", " " ICON_FA_CLOCK_O, "\tF5"))
		{
			time_t rawtime;
			struct tm* TIME;
			time(&rawtime);

			auto cpos = Editor.GetCursorPosition();

			char completeTimeandDate[20] = { 0 };
			strftime(completeTimeandDate, sizeof(completeTimeandDate), "%H:%M %d/%m/%Y", localtime(&rawtime));

			std::string stringToSet = completeTimeandDate;
			Editor.InsertText(stringToSet);
		}
		ImGui::EndPopup();
	}

	ImGui::PopStyleVar(2);

	if (statusBar)
	{
		if (BeginInfoBar())
		{
			auto cpos = Editor.GetCursorPosition();

			ImGui::Text("Lines: %d", Editor.GetTotalLines());

			ImGui::Separator();

			ImGui::Text("Columns: %d", cpos.mColumn + 1);

			ImGui::Separator();

			ImGui::Text("%s", Editor.IsOverwrite() ? "Ovr" : "Ins");

			ImGui::Separator();

			ImGui::Text("UTF-8");

			if (pathinStatusbar)
			{
				ImGui::Separator();
				ImGui::Text("Path: ");
				ImGui::SameLine();
				if (FilePath == "")
				{
					ImGui::TextDisabled("NO PATH");
				}
				else
				{
					ImGui::TextDisabled("%s", FilePath.c_str());
				}
			}
		}
		ImGui::EndMainMenuBar();
	}
}

void AppContent::FileNew()
{
	if (FileEdited)
	{
		int MB = MessageBox(NULL, Desc.c_str(), Title.c_str(), MB_YESNOCANCEL | MB_ICONQUESTION);

		switch (MB)
		{
		case IDYES:
			if (FilePath == "")
				FileSaveAs();
			else
				FileSave();
			break;

		case IDNO:
			FileCreated = true;
			FileEdited = false;
			FileOpened = false;
			FileSaved = false;
			FilePath = "";
			FileName = "Untitled.txt";
			Editor.SetText("");
			break;
		}
	}
	else
	{
		FileCreated = true;
		FileEdited = false;
		FileOpened = false;
		FileSaved = false;
		FilePath = "";
		FileName = "Untitled.txt";
		Editor.SetText("");
	}
}

void AppContent::FileOpen()
{
	if (FileEdited)
	{
		int MB = MessageBox(NULL, Desc.c_str(), Title.c_str(), MB_YESNOCANCEL | MB_ICONQUESTION);

		switch (MB)
		{
		case IDYES:
			if (FilePath == "")
				FileSaveAs();
			else
				FileSave();
			break;

		case IDNO:
			OpenFileDialog("All Files (*.*)\0*.*\0Text File (*.txt)\0*.txt").c_str();
			break;
		}
	}
	else
	{
		OpenFileDialog("All Files (*.*)\0*.*\0Text File (*.txt)\0*.txt").c_str();
	}
}

void AppContent::FileSave()
{
	std::string filename(FilePath);
	std::fstream file_out;

	file_out.open(filename, std::ios_base::out);

	file_out << FileContent << std::endl;

	FileSaved = true;
}

void AppContent::FileSaveDlg()
{
	SaveFileDialog("Text File (*.txt)\0*.txt");
}

void AppContent::FileSaveAs()
{
	SaveFileDialog("All Files (*.*)\0*.*");
}

void AppContent::FileClose()
{
	if (FileEdited)
	{
		int MB = MessageBox(NULL, Desc.c_str(), Title.c_str(), MB_YESNOCANCEL | MB_ICONQUESTION);

		switch (MB)
		{
		case IDYES:
			if (FilePath == "")
				FileSaveAs();
			else
				FileSave();
			break;

		case IDNO:
			FileCreated = true;
			FileSaved = false;
			FileOpened = false;
			FileName = "Untitled.txt";
			FilePath = "";
			Editor.SetText("");
			break;
		}
	}
	else
	{
		FileCreated = true;
		FileSaved = false;
		FileOpened = false;
		FileName = "Untitled.txt";
		FilePath = "";
		Editor.SetText("");
	}
}

void AppContent::Shortcuts()
{
	auto control = GetAsyncKeyState(VK_CONTROL);
	auto shift = GetAsyncKeyState(VK_SHIFT);

	//CTRL+N
	if (GetFocus())
	{
		if (GetAsyncKeyState('N'))
		{
			if (control)
			{
				FileNew();
			}
		}
	}

	//CTRL+O
	if (GetFocus())
	{
		if (GetAsyncKeyState('O'))
		{
			if (control)
			{
				FileOpen();
			}
		}
	}

	//CTRL+S
	if (GetFocus())
	{
		if (GetAsyncKeyState('S'))
		{
			if (!shift)
			{
				if (control)
				{
					if (FilePath == "")
						FileSaveDlg();
					else
						FileSave();
				}
			}
		}
	}

	//CTRL+SHIFT+S
	if (GetFocus())
	{
		if (GetAsyncKeyState('S'))
		{
			if (control)
			{
				if (shift)
				{
					FileSaveAs();
				}
			}
		}
	}

	//CTRL+SHIFT+C
	if (GetFocus())
	{
		if (GetAsyncKeyState('C'))
		{
			if (control)
			{
				if (shift)
				{
					FileClose();
				}
			}
		}
	}

	//CTRL+SHIFT+N
	if (GetFocus())
	{
		if (GetAsyncKeyState('N'))
		{
			if (control)
			{
				if (shift)
				{
					system("start Text-Editor.exe");
				}
			}
		}
	}

	//CTRL+P
	if (GetFocus())
	{
		if (GetAsyncKeyState('P'))
		{
			if (control)
			{
				PrintDialog();
			}
		}
	}

	//F5
	if (GetFocus())
	{
		if (GetAsyncKeyState(VK_F5))
		{
			if (!control)
			{
				if (!shift)
				{
					time_t rawtime;
					struct tm* TIME;
					time(&rawtime);

					char completeTimeandDate[20] = { 0 };
					strftime(completeTimeandDate, sizeof(completeTimeandDate), "%H:%M %d/%m/%Y", localtime(&rawtime));

					std::string stringToSet = completeTimeandDate;
					Editor.InsertText(stringToSet);
					Sleep(200);
				}
			}
		}
	}
}

void AppContent::DragDropFile(const char* filePath)
{
	std::filesystem::path dropPath = filePath;
	if (!FileEdited && dropPath.has_extension())
	{
		if (dropPath.string().c_str() > "")
		{
			FileOpened = true;
		}

		FileName = dropPath.filename().string().c_str();
		FilePath = dropPath.string().c_str();

		static const char* filetoedit = FilePath.c_str();
		std::ifstream t(filetoedit);
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		Editor.SetText(str);
	}
	else
	{
		if (FileEdited)
		{
			int MB = MessageBox(NULL, Desc.c_str(), Title.c_str(), MB_YESNOCANCEL | MB_ICONQUESTION);

			switch (MB)
			{
			case IDYES:
				FileSaveAs();
				break;

			case IDNO:
				std::filesystem::path dropPath = filePath;

				if (dropPath.string().c_str() > "")
				{
					FileOpened = true;
				}

				FileName = dropPath.filename().string().c_str();
				FilePath = dropPath.string().c_str();

				static const char* filetoedit = FilePath.c_str();
				std::ifstream t(filetoedit);
				std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
				Editor.SetText(str);
				break;
			}
		}
		if (!dropPath.has_extension())
		{
			MessageBox(NULL, "File Format isn't valid", "ERROR", MB_ICONERROR);
		}
	}
}

void AppContent::SaveBefExit()
{
	if (FileEdited)
	{
		int MB = MessageBox(NULL, Desc.c_str(), Title.c_str(), MB_YESNO | MB_ICONQUESTION);

		switch (MB)
		{
		case IDYES:
			if (FilePath == "")
				FileSaveAs();
			else
				FileSave();
			break;

		case IDNO:
			exit(0);
			break;
		}
	}
}

void AppContent::AboutPopup()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
	if (About)
		ImGui::OpenPopup("About Text-Editor");

	ImGui::SetNextWindowPos(ImVec2(viewport->GetWorkCenter()), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(200.0f, 65.0f), ImGuiCond_Always);
	if (ImGui::BeginPopupModal("About Text-Editor", &About, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::SetCursorPosX(200.0f / 2 - 32);		
		ImGui::Text("Text-Editor");

		ImGui::Separator();

		ImGui::SetCursorPosX(200.0f / 2 - 75);
		ImGui::TextDisabled("2021-2022 (C) - by DavLen");

		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();
}

void AppContent::ReadIni()
{
	//POPUPS
	About = ini.ReadBoolean(path.c_str(), iniSection, "AboutPopup", false);

	//EDITOROPT
	readonly = ini.ReadBoolean(path.c_str(), iniSection, "ReadOnly", false);
	overwrite = ini.ReadBoolean(path.c_str(), iniSection, "Overwrite", false);
	whitespaces = ini.ReadBoolean(path.c_str(), iniSection, "Whitespaces", false);

	pathinStatusbar = ini.ReadBoolean(path.c_str(), iniSection, "Path in Statusbar", true);

	//PREFERENCES
	darkTheme = ini.ReadBoolean(path.c_str(), iniSection, "Dark Theme", false);
	statusBar = ini.ReadBoolean(path.c_str(), iniSection, "StatusBar", true);

	//RECENTFILE
	recentFilesAvail = ini.ReadBoolean(path.c_str(), iniSection, "RecentFileAvail", false);
}

void AppContent::WriteIni()
{
	//POPUPS
	ini.WriteBoolean(path.c_str(), iniSection, "AboutPopup", About);

	//EDITOROPT
	ini.WriteBoolean(path.c_str(), iniSection, "ReadOnly", readonly);
	ini.WriteBoolean(path.c_str(), iniSection, "Overwrite", overwrite);
	ini.WriteBoolean(path.c_str(), iniSection, "Whitespaces", whitespaces);

	ini.WriteBoolean(path.c_str(), iniSection, "Path in Statusbar", pathinStatusbar);

	//PREFERENCES
	ini.WriteBoolean(path.c_str(), iniSection, "Dark Theme", darkTheme);
	ini.WriteBoolean(path.c_str(), iniSection, "StatusBar", statusBar);

	//RECENTFILE
	if (FilePath > "")
	{
		ini.WriteString(path.c_str(), iniSection, "RecentFile", FilePath.c_str());
	}
	ini.WriteBoolean(path.c_str(), iniSection, "RecentFileAvail", recentFilesAvail);
}

bool AppContent::BeginInfoBar()
{
	using namespace ImGui;
	ImGuiContext& g = *GImGui;
	ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)GetMainViewport();

	SetCurrentViewport(NULL, viewport);

	g.NextWindowData.MenuBarOffsetMinVal = ImVec2(g.Style.DisplaySafeAreaPadding.x, ImMax(g.Style.DisplaySafeAreaPadding.y - g.Style.FramePadding.y, 0.0f));
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
	float height = GetFrameHeight();
	bool is_open = BeginViewportSideBar("##MyMainMenuBar", viewport, ImGuiDir_Down, height, window_flags);
	g.NextWindowData.MenuBarOffsetMinVal = ImVec2(0.0f, 0.0f);

	if (is_open)
		BeginMenuBar();
	else
		End();
	return is_open;
}

std::string AppContent::OpenFileDialog(const char* filter)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn) == TRUE)
	{
		FileOpened = true;

		std::filesystem::path fileName = ofn.lpstrFile;

		FileName = fileName.filename().string();
		FilePath = fileName.string();

		static const char* filetoedit = ofn.lpstrFile;
		std::ifstream t(filetoedit);
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		Editor.SetText(str);

		return ofn.lpstrFile;
	}

	return std::string();
}

std::string AppContent::SaveFileDialog(const char* filter)
{
	OPENFILENAMEA sfn;
	CHAR szFile[260] = { 0 };
	strcpy(szFile, FileName.c_str());
	ZeroMemory(&sfn, sizeof(OPENFILENAME));
	sfn.lStructSize = sizeof(OPENFILENAME);
	sfn.lpstrFile = szFile;
	sfn.nMaxFile = sizeof(szFile);
	sfn.lpstrFilter = filter;
	sfn.nFilterIndex = 1;
	sfn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	sfn.lpstrDefExt = strchr(filter, '\0') + 1;

	if (GetSaveFileNameA(&sfn) == TRUE)
	{
		FileSaved = true;

		std::filesystem::path fileName = sfn.lpstrFile;

		FileName = fileName.filename().string();
		FilePath = fileName.string();

		std::vector<std::string> lines;
		std::string filename(sfn.lpstrFile);
		std::fstream file_out;

		file_out.open(filename, std::ios_base::out);

		file_out << FileContent << std::endl;

		return sfn.lpstrFile;
	}

	return std::string();
}

void AppContent::PrintDialog()
{
	HWND hwnd = GetActiveWindow();
	static PRINTDLG pDlg;
	static DOCINFO dInfo;

	dInfo.cbSize = sizeof(dInfo);
	dInfo.lpszDocName = FileName.c_str();

	pDlg.Flags = PD_RETURNDC;
	pDlg.lStructSize = sizeof(pDlg);
	pDlg.hwndOwner - hwnd;

	PrintDlg(&pDlg);

	StartDoc(pDlg.hDC, &dInfo);

	StartPage(pDlg.hDC);

	RECT rc;
	::SetRect(&rc, 600, 600, 4961 - 600, 7016 - 600);
	::DrawText(pDlg.hDC, FileContent.c_str(), ::strlen(FileContent.c_str()), &rc, DT_LEFT | DT_EXTERNALLEADING | DT_WORDBREAK);

	EndPage(pDlg.hDC);

	EndDoc(pDlg.hDC);

	ReleaseDC(hwnd, pDlg.hDC);
}