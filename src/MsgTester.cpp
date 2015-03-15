#include "MsgTester.h"
#include "nppexec_msgs.h"
#include "NppExecPluginMsgSender.h"
#include <WindowsX.h>


const TCHAR* CMsgTester::PLUGIN_NAME = _T("PESMU Compile");

FuncItem* CMsgTester::nppGetFuncsArray(int* pnbFuncItems)
{
    *pnbFuncItems = CMsgTesterMenu::N_NBFUNCITEMS;
    return CMsgTesterMenu::arrFuncItems;
}

const TCHAR* CMsgTester::nppGetName()
{
    return CMsgTester::PLUGIN_NAME;
}

void CMsgTester::nppBeNotified(SCNotification* pscn)
{
    if ( pscn->nmhdr.hwndFrom == m_nppMsgr.getNppWnd() )
    {
        // >>> notifications from Notepad++
        switch ( pscn->nmhdr.code )
        {
            case NPPN_READY:
                OnNppReady();
                break;
            
            case NPPN_SHUTDOWN:
                OnNppShutdown();
                break;

            default:
                break;
        }
        // <<< notifications from Notepad++
    }
}

LRESULT CMsgTester::nppMessageProc(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    if ( uMessage == NPPM_MSGTOPLUGIN )
    {
        CommunicationInfo* pci = (CommunicationInfo *) lParam;
        if ( pci )
        {
            if ( pci->internalMsg == NPEN_RESULT )
            { 
                // NPEN_RESULT notification...
                if ( lstrcmpi(pci->srcModuleName, _T("NppExec.dll")) == 0 )
                {
                    // ...from NppExec plugin
                    DWORD dwResult = *((DWORD *) pci->info);
                    if ( dwResult == NPE_RESULT_OK )
                    {
                        // OK, the script has been executed
                        ::MessageBox( getNppWnd(), _T("OK"), _T("NPEN_RESULT notification"), MB_OK );
                    }
                    else
                    {
                        // failed, maybe internal error in NppExec
                        ::MessageBox( getNppWnd(), _T("FAILED"), _T("NPEN_RESULT notification"), MB_OK | MB_ICONERROR );
                    }
                }
            }
        }
    }
    return 1;
}

void CMsgTester::OnNppSetInfo(const NppData& nppd)
{
    m_nppPluginMenu.setNppData(nppd);
}

void CMsgTester::OnNppReady()
{
    // TODO:  add your code here :)
}

void CMsgTester::OnNppShutdown()
{
    // TODO:  add your code here :)
}

void CMsgTester::funcGenerateJavadocFile()
{
	//// Get the full filepath of the currently open file
	//TCHAR fullPath[MAX_PATH];
	//::SendMessage(getNppWnd(), NPPM_GETFULLCURRENTPATH, (WPARAM)MAX_PATH, (LPARAM)fullPath);
	//// Get just the name and extension of the file
	//TCHAR namePart[MAX_PATH];
	//::SendMessage(getNppWnd(), NPPM_GETFILENAME, (WPARAM)MAX_PATH, (LPARAM)namePart);
	//// Get the length of paths
	//int fullLength = _tcslen(fullPath);
	//int nameLength = _tcslen(namePart);
	//// Get the path without the filename
	//TCHAR modifiedPath[MAX_PATH];
	//_tcsncpy_s(modifiedPath, fullPath, fullLength - nameLength);
	//// Concat the javadoc index location and do some weird type conversions, see http://stackoverflow.com/a/6814691
	//const char* javadoc = "javadoc\\index.html && exit)";
	//wchar_t *wjavadoc = new wchar_t[strlen(javadoc) + 1];
	//mbstowcs(wjavadoc, javadoc, strlen(javadoc) + 1);
	//_tcscat(modifiedPath, wjavadoc);

	//const char* script = "cd $(CURRENT_DIRECTORY) \n javadoc -d javadoc $(FILE_NAME) \n cmd /K (start ";
	//wchar_t *wscript = new wchar_t[strlen(script) + 1];
	//mbstowcs(wscript, script, strlen(script) + 1);
	//_tcscat(wscript, modifiedPath);

	//::MessageBox(getNppWnd(), wscript, _T("NPEM_EXECUTE"), MB_OK | MB_ICONERROR);

	// Create the message to send to NPPExec
	NpeExecuteParam nep;
	nep.szScriptBody = _T("cd $(CURRENT_DIRECTORY) \n javadoc -d javadoc $(FILE_NAME)");
	nep.dwResult = 0; // send notification when executed

	// Send message and check for failure
	CNppExecPluginMsgSender npeMsgr(getNppWnd(), getDllFileName());
	if (npeMsgr.NpeExecute(&nep) != NPE_EXECUTE_OK)
	{
		::MessageBox(getNppWnd(), _T("Operation failed"), _T("NPEM_EXECUTE"), MB_OK | MB_ICONERROR);
		return;
	}
	// Else NPPExec succeeded, open the generated javadoc index file
	//else
	//{
	//	// Get the full filepath of the currently open file
	//	TCHAR fullPath[MAX_PATH];
	//	::SendMessage(getNppWnd(), NPPM_GETFULLCURRENTPATH, (WPARAM)MAX_PATH, (LPARAM)fullPath);
	//	// Get just the name and extension of the file
	//	TCHAR namePart[MAX_PATH];
	//	::SendMessage(getNppWnd(), NPPM_GETFILENAME, (WPARAM)MAX_PATH, (LPARAM)namePart);
	//	// Get the length of paths
	//	int fullLength = _tcslen(fullPath);
	//	int nameLength = _tcslen(namePart);
	//	// Get the path without the filename
	//	TCHAR modifiedPath[MAX_PATH];
	//	_tcsncpy_s(modifiedPath, fullPath, fullLength - nameLength);
	//	// Concat the javadoc index location and do some weird type conversions, see http://stackoverflow.com/a/6814691
	//	const char* javadoc = "javadoc\\index.html";
	//	wchar_t *wjavadoc = new wchar_t[strlen(javadoc) + 1];
	//	mbstowcs(wjavadoc, javadoc, strlen(javadoc) + 1);
	//	_tcscat(modifiedPath, wjavadoc);
	//	// Open the default browser
	//	ShellExecute(getNppWnd(), _T("open"), modifiedPath, NULL, NULL, SW_SHOWNORMAL);
	//}
}

void CMsgTester::functionGenerateJavadocProject()
{
	::MessageBox(getNppWnd(), _T("Generate Javadoc project\nTo be implemented"), _T("NPEM_EXECUTE"), MB_OK | MB_ICONERROR);
}
