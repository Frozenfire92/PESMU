//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "PluginDefinition.h"
#include "menuCmdID.h"
#include "Resource.h"

#include <list>
#include <Windows.h>
#include "TaskListDlg.h"
#include "config.h"

//TODO: refactor/rename

TaskListDlg _goToLine;

//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

#define DOCKABLE_DEMO_INDEX 15

void reload_config_file()
{
	e_config_load_result load_result= load_config_file();

	if (load_result==_config_load_default_failed)
	{
		MessageBox(NULL, TEXT("Failed to load default config, Task List will not work"), TEXT("NPP Task List"), MB_OK);
	}
	else if (load_result==_config_load_file_failed)
	{
		MessageBox(NULL, TEXT("Failed loading config file, falling back to defaults (only 'TODO:' is supported)"), TEXT("NPP Task List"), MB_OK);
	}

	findTasks();
}

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE hModule)
{
	// Initialize dockable demo dialog
	_goToLine.init((HINSTANCE)hModule, NULL);
	reload_config_file();
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
	unload_config_file();
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );
    setCommand(0, TEXT("Show Task List"), &displayDialog, NULL, false);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//

void displayDialog()
{
	//open pane
	DockableDlgDemo();

	findTasks();
}

//----------------------------------------------//
//-- HELPER FUNCTIONS --//
//----------------------------------------------//

//find all tasks
void findTasks()
{
	if (!_goToLine.isCreated())
		return;
    // Open a new document
    //::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);

    // Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

	//list of todo items
	std::list<TodoItem> todos;

	//get length SCI_GETLENGTH
    int length = ::SendMessage(curScintilla, SCI_GETLENGTH, 0, 0);
	//search for todos: (starting at character 0) SCI_FINDTEXT

	int keyword_count;
	const char * const *keywords= get_keyword_list(&keyword_count);
	char search_pattern_1[k_max_keyword_length+16]; // + 16 for extra space for regex search strings and \0
	char search_pattern_2[k_max_keyword_length+16]; // + 16 for extra space for regex search strings and \0

	for (int keyword_index= 0; keyword_index<keyword_count; keyword_index++)
	{
		const char *keyword= keywords[keyword_index];
		int keyword_length= strlen(keyword);

		if (keyword_length<k_max_keyword_length) // this should be an assert
		{
			//sprintf(search_pattern_1, "^.*%s.*$", keyword);
			sprintf(search_pattern_1, keyword);
			//sprintf(search_pattern_2, "%s.*$", keyword);
			sprintf(search_pattern_2, ".*$");

			Sci_TextToFind search;
			search.lpstrText = search_pattern_1;
			search.chrg.cpMin = 0;
			search.chrg.cpMax = length;
			int len;
			int totalLen = 0;
			Sci_TextRange result;
			TodoItem item;
			item.hScintilla = curScintilla;
			while( ::SendMessage(curScintilla, SCI_FINDTEXT, SCFIND_MATCHCASE | SCFIND_WHOLEWORD, (LPARAM)&search) > -1 )
			{
				//narrow down text to what we actually want
				search.lpstrText = search_pattern_2;
				search.chrg.cpMin = search.chrgText.cpMin;
				::SendMessage(curScintilla, SCI_FINDTEXT, SCFIND_REGEXP, (LPARAM)&search);
				//get text and add it to list
				len = search.chrgText.cpMax - search.chrgText.cpMin + 1; //+1 for \0
				result.chrg = search.chrgText;
				result.lpstrText = new char[len];
				::SendMessage(curScintilla, SCI_GETTEXTRANGE, NULL, (LPARAM)&result);
				//get meta-data to include with text: scintilla handle, text start/end
				item.text = result.lpstrText;
				item.startPosition = search.chrgText.cpMin;
				item.endPosition = search.chrgText.cpMax;
				todos.push_back(item);

				//restore search pattern
				search.lpstrText = search_pattern_1;
				//advance search position
				search.chrg.cpMin = search.chrgText.cpMax + 1;
			}
		}
	}

	//display all todo's
	_goToLine.SetList(todos);
	
	//cleanup list
	std::list<TodoItem>::iterator it;
	for ( it = todos.begin(); it != todos.end(); ++it )
	{
		delete[] it->text;
	}
	todos.clear();
}

//Example Code Using CharToWideChar
	//char buffer[256];
	//BSTR unicodestr;
	//itoa(length, buffer, 10);
	//if ( CharToWideChar(buffer, &unicodestr) )
	//{
	//	::MessageBox(NULL, unicodestr, TEXT("Notepad++ Plugin Template"), MB_OK);
	//	// when done, free the BSTR
	//	::SysFreeString(unicodestr);
	//}
bool CharToWideChar( const char* _inString, BSTR* _out )
{
	if ( !_inString )
	{
		return 0;
	}
	int len = lstrlenA(_inString);
	int lenW = ::MultiByteToWideChar(CP_ACP, 0, _inString, len, 0, 0);
	if (lenW > 0)
	{
		// Check whether conversion was successful
		*_out = ::SysAllocStringLen(0, lenW);
		::MultiByteToWideChar(CP_ACP, 0, _inString, len, *_out, lenW);
	}
	else
	{
		// handle the error
		return false;
	}
	return true;
}

// Dockable Dialog Demo
// 
// This demonstration shows you how to do a dockable dialog.
// You can create your own non dockable dialog - in this case you don't nedd this demonstration.
// You have to create your dialog by inherented DockingDlgInterface class in order to make your dialog dockable
// - please see DemoDlg.h and DemoDlg.cpp to have more informations.
void DockableDlgDemo()
{
	_goToLine.setParent(nppData._nppHandle);
	tTbData	data = {0};

	if (!_goToLine.isCreated())
	{
		_goToLine.create(&data);

		// define the default docking behaviour
		data.uMask = DWS_DF_CONT_RIGHT;

		data.pszModuleName = _goToLine.getPluginFileName();

		// the dlgDlg should be the index of funcItem where the current function pointer is
		// in this case is DOCKABLE_DEMO_INDEX
		data.dlgID = DOCKABLE_DEMO_INDEX;
		::SendMessage(nppData._nppHandle, NPPM_DMMREGASDCKDLG, 0, (LPARAM)&data);
	}
	_goToLine.display();
}