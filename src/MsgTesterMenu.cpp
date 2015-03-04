#include "MsgTesterMenu.h"
#include "MsgTester.h"
#include "NppExecPluginMsgSender.h"


extern CMsgTester thePlugin;


FuncItem CMsgTesterMenu::arrFuncItems[N_NBFUNCITEMS] = {
    { _T("NPEM_GETVERDWORD"), funcNpeGetVerDword,  0, false, NULL },
    { _T("NPEM_GETVERSTR"),   funcNpeGetVerStr,    0, false, NULL },
    { _T("NPEM_GETSTATE"),    funcNpeGetState,     0, false, NULL },
    { _T("NPEM_PRINT"),       funcNpePrint,        0, false, NULL },
    { _T("NPEM_EXECUTE"),     funcNpeExecute,      0, false, NULL },
    { _T("NPEM_NPPEXEC"),     funcNpeNppExec,      0, false, NULL }
};

void CMsgTesterMenu::funcNpeGetVerDword()
{
    CNppExecPluginMsgSender npeMsgr( thePlugin.getNppWnd(), thePlugin.getDllFileName() );
    DWORD dwVersion = npeMsgr.NpeGetVerDword();

    TCHAR str[20];
    ::wsprintf(str, _T("0x%04X"), dwVersion);
    ::MessageBox( thePlugin.getNppWnd(), str, _T("NppExec version (as dword)"), MB_OK);
}

void CMsgTesterMenu::funcNpeGetVerStr()
{
    CNppExecPluginMsgSender npeMsgr( thePlugin.getNppWnd(), thePlugin.getDllFileName() );
    TCHAR str[NPE_MAXVERSTR];
    npeMsgr.NpeGetVerStr(str);

    ::MessageBox( thePlugin.getNppWnd(), str, _T("NppExec version (as string)"), MB_OK);
}

void CMsgTesterMenu::funcNpeGetState()
{
    CNppExecPluginMsgSender npeMsgr( thePlugin.getNppWnd(), thePlugin.getDllFileName() );
    DWORD dwState = npeMsgr.NpeGetState();
    if ( dwState == NPE_STATEREADY )
        ::MessageBox( thePlugin.getNppWnd(), _T("Ready"), _T("NppExec state"), MB_OK);
    else if ( dwState & NPE_STATEBUSY )
        ::MessageBox( thePlugin.getNppWnd(), _T("Busy"), _T("NppExec state"), MB_OK);
    else
        ::MessageBox( thePlugin.getNppWnd(), _T("UNKNOWN"), _T("NppExec state"), MB_OK);
}

void CMsgTesterMenu::funcNpePrint()
{
    const TCHAR* cszText = _T("Hello!\nThis is a test message.\r\nAre you happy to see it? ;-)");

    CNppExecPluginMsgSender npeMsgr( thePlugin.getNppWnd(), thePlugin.getDllFileName() );
    if ( npeMsgr.NpePrint(cszText) != NPE_STATEREADY )
        ::MessageBox( thePlugin.getNppWnd(), _T("Operation failed"), _T("NPEM_PRINT"), MB_OK | MB_ICONERROR);
}

void CMsgTesterMenu::funcNpeExecute()
{
    NpeExecuteParam nep;
    nep.szScriptBody = _T("cd c:\\ \n dir\r\ncd $(NPP_DIRECTORY)\ndir \ndir *.txt");
    nep.dwResult = 1; // send notification when executed
    
    CNppExecPluginMsgSender npeMsgr( thePlugin.getNppWnd(), thePlugin.getDllFileName() );
    if ( npeMsgr.NpeExecute(&nep) != NPE_EXECUTE_OK )
        ::MessageBox( thePlugin.getNppWnd(), _T("Operation failed"), _T("NPEM_EXECUTE"), MB_OK | MB_ICONERROR);
}

void CMsgTesterMenu::funcNpeNppExec()
{
    NpeNppExecParam npep;
    npep.szScriptName = _T("script");
    // npep.szScriptName = _T("C:\\Program Files\\My NppExec Scripts\\test script.txt");
    npep.szScriptArguments = _T("\"arg 1\" \"arg 2\"");
    npep.dwResult = 1; // send notification when executed

    CNppExecPluginMsgSender npeMsgr( thePlugin.getNppWnd(), thePlugin.getDllFileName() );
    if ( npeMsgr.NpeNppExec(&npep) != NPE_NPPEXEC_OK )
        ::MessageBox( thePlugin.getNppWnd(), _T("Operation failed"), _T("NPEM_NPPEXEC"), MB_OK | MB_ICONERROR);
}

