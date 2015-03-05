// This file is part of DoxyIt.
// 
// Copyright (C)2013 Justin Dailey <dail8859@yahoo.com>
// 
// DoxyIt is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include <WindowsX.h>
#include "PluginDefinition.h"
#include "AboutDialog.h"
#include "resource.h"
#include "Hyperlinks.h"
#include "Version.h"

INT_PTR CALLBACK abtDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		ConvertStaticToHyperlink(hwndDlg, IDC_GITHUB);
		ConvertStaticToHyperlink(hwndDlg, IDC_README);
		Edit_SetText(GetDlgItem(hwndDlg, IDC_VERSION), VERSION_TEXT TEXT(" ") VERSION_STAGE);
		return true;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			DestroyWindow(hwndDlg);
			return true;
		case IDC_GITHUB:
			ShellExecute(hwndDlg, TEXT("open"), TEXT("https://github.com/dail8859/DoxyIt"), NULL, NULL, SW_SHOWNORMAL);
			return true;
		case IDC_README:
			ShellExecute(hwndDlg, TEXT("open"), TEXT("https://github.com/dail8859/DoxyIt/blob/v") VERSION_TEXT TEXT("/README.md"), NULL, NULL, SW_SHOWNORMAL);
			return true;
		}
	case WM_DESTROY:
		DestroyWindow(hwndDlg);
		return true;
	}
	return false;
}