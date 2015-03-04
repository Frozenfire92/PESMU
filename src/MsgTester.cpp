#include "MsgTester.h"
#include "nppexec_msgs.h"


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
