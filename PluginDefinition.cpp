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
#include <tchar.h>
#include <wchar.h>
#include <WindowsX.h>

// Keyboard shortcuts
ShortcutKey skSearch = { true, true, false, 'Q' };

// Menu items
FuncItem funcItem[nbFunc] = {
    //{ TEXT("Search Java 7 API for selected text"), search7, 0, false, &skSearch },
    { TEXT("Search Java 8 API for selected text"), search8, 0, false, &skSearch },
    //{ TEXT("Open Java Standard Edition 7 API"), open7, 0, false, NULL },
    { TEXT("Open Java Standard Edition 8 API"), open8, 0, false, NULL } 
};

// The data of Notepad++ that you can use in your plugin commands
NppData nppData;

// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE hModule)
{
}

// Here you can do the clean up, save the parameters (if any) for the next session
void pluginCleanUp()
{
}

// Search Java SE 7 API for selected text
/*void search7()
{
    HWND curScintilla;
    const int MAX_QUERY = 400;
    const char* pathPart1 = "https://search.oracle.com/search/search?search_p_main_operator=all&group=Documentation&q=";
    const char* pathPart2 = "+url%3A%2Fjavase%2F7%2Fdocs%2Fapi&searchField=";
    const char* pathPart3 = "&docsets=%2Fjavase%2F7%2Fdocs%2Fapi";
    TCHAR selectedText[MAX_QUERY];
    TCHAR pathFull[MAX_QUERY];

    // Get the current scintilla
    int which;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    curScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;
    // Get selected text
    ::SendMessage(curScintilla, SCI_GETSELTEXT, 0, (LPARAM)selectedText);

    // Build URI
    wchar_t *wpathPart1 = new wchar_t[strlen(pathPart1) + 1];
    mbstowcs(wpathPart1, pathPart1, strlen(pathPart1) + 1);
    _tcscpy(pathFull, wpathPart1);

    int selectedTextLength = _tcslen(selectedText);
    const char* cselectedText = (const char*)selectedText;
    wchar_t *wselectedText = new wchar_t[selectedTextLength];
    mbstowcs(wselectedText, cselectedText, strlen(cselectedText) + 1);
    _tcscat_s(pathFull, wselectedText);

    wchar_t *wpathPart2 = new wchar_t[strlen(pathPart2) + 1];
    mbstowcs(wpathPart2, pathPart2, strlen(pathPart2) + 1);
    _tcscat(pathFull, wpathPart2);

    _tcscat(pathFull, wselectedText);

    wchar_t *wpathPart3 = new wchar_t[strlen(pathPart3) + 1];
    mbstowcs(wpathPart3, pathPart3, strlen(pathPart3) + 1);
    _tcscat(pathFull, wpathPart3);

    // Open the link in default browser
    ShellExecute(nppData._nppHandle, _T("open"), pathFull, NULL, NULL, SW_SHOWNORMAL);

    // Dealocate memory
    delete[]wselectedText;
    delete[]wpathPart1;
    delete[]wpathPart2;
    delete[]wpathPart3;
}*/

// Search Java SE 8 API for selected text
void search8()
{
    HWND curScintilla;
    const int MAX_QUERY = 400;
    const char* pathPart1 = "https://search.oracle.com/search/search?search_p_main_operator=all&group=Documentation&q=";
    const char* pathPart2 = "+url%3A%2Fjavase%2F8%2Fdocs%2Fapi&searchField=";
    const char* pathPart3 = "&docsets=%2Fjavase%2F8%2Fdocs%2Fapi";
    TCHAR selectedText[MAX_QUERY];
    TCHAR pathFull[MAX_QUERY];
    
    // Get the current scintilla
    int which;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
    // Get selected text
    ::SendMessage(curScintilla, SCI_GETSELTEXT, 0, (LPARAM)selectedText);
    
    // Build URI
    wchar_t *wpathPart1 = new wchar_t[strlen(pathPart1) + 1];
    mbstowcs(wpathPart1, pathPart1, strlen(pathPart1) + 1);
    _tcscpy(pathFull, wpathPart1);

    int selectedTextLength = _tcslen(selectedText);
    const char* cselectedText = (const char*)selectedText;
    wchar_t *wselectedText = new wchar_t[selectedTextLength];
    mbstowcs(wselectedText, cselectedText, strlen(cselectedText) + 1);
    _tcscat_s(pathFull, wselectedText);

    wchar_t *wpathPart2 = new wchar_t[strlen(pathPart2) + 1];
    mbstowcs(wpathPart2, pathPart2, strlen(pathPart2) + 1);
    _tcscat(pathFull, wpathPart2);

    _tcscat(pathFull, wselectedText);

    wchar_t *wpathPart3 = new wchar_t[strlen(pathPart3) + 1];
    mbstowcs(wpathPart3, pathPart3, strlen(pathPart3) + 1);
    _tcscat(pathFull, wpathPart3);
    
    // Open the link in default browser
    ShellExecute(nppData._nppHandle, _T("open"), pathFull, NULL, NULL, SW_SHOWNORMAL);
    
    // Dealocate memory
    delete []wselectedText;
    delete []wpathPart1;
    delete []wpathPart2;
    delete []wpathPart3;
}

// Open Java SE 7 API for selected text
/*void open7()
{
    ShellExecute(nppData._nppHandle, _T("open"), _T("http://docs.oracle.com/javase/7/docs/api/"), NULL, NULL, SW_SHOWNORMAL);
}*/

// Open Java SE 8 API for selected text
void open8()
{
    ShellExecute(nppData._nppHandle, _T("open"), _T("http://docs.oracle.com/javase/8/docs/api/"), NULL, NULL, SW_SHOWNORMAL);
}