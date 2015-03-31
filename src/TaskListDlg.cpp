//this file is part of notepad++
//Copyright (C)2003 Don HO ( donho@altern.org )
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

#include "TaskListDlg.h"
#include "PluginDefinition.h"
#include <tchar.h>

extern NppData nppData;

BOOL CALLBACK TaskListDlg::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		/*
			This code is related to unsucessful attempts at right clicking the list box and creating a button
		*/
		/*case WM_RBUTTONDOWN:
		{
							   MessageBox(nppData._nppHandle, _T("Right button lo"), _T("RBM nest"), MB_OK);
		}*/
		/*case WM_INITDIALOG:
		{
						  CreateWindowEx(0,
							  L"LISTBOX",
							  TEXT("FamilyListing"),
							  WS_BORDER | WS_CHILD | WS_VISIBLE,
							  10, 10, 140, 120,
							  hWndDlg,
							  NULL,
							  hInst,
							  NULL);
						  return TRUE;
		}*/
		case WM_COMMAND : 
		{
			switch ( LOWORD(wParam) )
			{
				case ID_TODO_LIST :
				{
					switch( HIWORD(wParam) )
					{
						case LBN_DBLCLK: //go to selected item
						{
							// Get the current scintilla
							int which = -1;
							::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
							if (which == -1)
								return FALSE;
							HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

							//get selected item
							unsigned int index;
							if ( LB_ERR != (index = ::SendMessage((HWND)lParam, LB_GETCURSEL, NULL, NULL)) )
							{
								TodoItem item = todoItems[index];
						
								//make sure the line is visible
								int line = ::SendMessage(curScintilla, SCI_LINEFROMPOSITION, item.startPosition, 0);
								::SendMessage(curScintilla, SCI_ENSUREVISIBLE, line, 0);
								//highlight selected item in text SCI_SETSEL
								::SendMessage(curScintilla, SCI_SETSEL, item.endPosition, item.startPosition);
							}
							else //nothing was selected
							{
								return FALSE;
							}

							return TRUE;
							break;
						}
						case BN_DBLCLK:
						{
							// Get the current scintilla
							int which = -1;
							::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
							if (which == -1)
								return FALSE;
							HWND curScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;

							//get selected item
							unsigned int index;
							if (LB_ERR != (index = ::SendMessage((HWND)lParam, LB_GETCURSEL, NULL, NULL)))
							{
								TodoItem item = todoItems[index];

								//make sure the line is visible
								int line = ::SendMessage(curScintilla, SCI_LINEFROMPOSITION, item.startPosition, 0);
								::SendMessage(curScintilla, SCI_ENSUREVISIBLE, line, 0);
								//highlight selected item in text SCI_SETSEL
								::SendMessage(curScintilla, SCI_SETSEL, item.endPosition, item.startPosition - 2);
								//Deleted selected text
								::SendMessage(curScintilla, SCI_REPLACESEL, 0, (LPARAM)"");
							}
							else //nothing was selected
							{
								return FALSE;
							}
							break;
						}
					}
				}
			}
			return FALSE;
		}
		case WM_SIZE: //the dialog box was resized, resize the list box to fit
		{
			// get list box handle
			HWND _hList = ::GetDlgItem( _hSelf, ID_TODO_LIST );
			if ( !_hList )
				return FALSE;

			// resize list box
			int margin = 2;
			return ::SetWindowPos( _hList, NULL, margin, margin, LOWORD(lParam) - margin * 2, HIWORD(lParam) - margin * 2, NULL );
		}

		default :
			return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
	}
}

