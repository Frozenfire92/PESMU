#include "MsgTester.h"
#include "nppexec_msgs.h"
#include "NppExecPluginMsgSender.h"


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

	// Open generated index file
	//TODO
}

void CMsgTester::functionGenerateJavadocProject()
{
	::MessageBox(getNppWnd(), _T("Generate Javadoc project"), _T("NPEM_EXECUTE"), MB_OK | MB_ICONERROR);
}
