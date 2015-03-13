#include "MsgTesterMenu.h"
#include "MsgTester.h"
#include "NppExecPluginMsgSender.h"

extern CMsgTester thePlugin;

FuncItem CMsgTesterMenu::arrFuncItems[N_NBFUNCITEMS] = {
	{ _T("Compile file"), funcCompileFile, 0, false, NULL },
	{ _T("Execute file"), funcExecuteFile, 0, false, NULL },
	{ _T("Compile and Execute file"), funcCompileExecuteFile, 0, false, NULL },
	{ _T("Compile Project"), funcCompileProject, 0, false, NULL },
	{ _T("JAR Project"), funcJARProject, 0, false, NULL }
};

void CMsgTesterMenu::funcCompileFile()
{
	// Create the message to send to NPPExec
	NpeExecuteParam nep;
	nep.szScriptBody = _T("NPP_SAVE \n cd $(CURRENT_DIRECTORY) \n javac $(FILE_NAME)");
	nep.dwResult = 0; // send notification when executed
	
	// Send message and check for failure
	CNppExecPluginMsgSender npeMsgr( thePlugin.getNppWnd(), thePlugin.getDllFileName() );
	if ( npeMsgr.NpeExecute(&nep) != NPE_EXECUTE_OK )
	    ::MessageBox( thePlugin.getNppWnd(), _T("Operation failed"), _T("NPEM_EXECUTE"), MB_OK | MB_ICONERROR);
}

void CMsgTesterMenu::funcExecuteFile()
{
	// Create the message to send to NPPExec
	NpeExecuteParam nep;
	nep.szScriptBody = _T("cd $(CURRENT_DIRECTORY) \n java $(NAME_PART)");
	nep.dwResult = 0; // send notification when executed

	// Send message and check for failure
	CNppExecPluginMsgSender npeMsgr(thePlugin.getNppWnd(), thePlugin.getDllFileName());
	if (npeMsgr.NpeExecute(&nep) != NPE_EXECUTE_OK)
		::MessageBox(thePlugin.getNppWnd(), _T("Operation failed"), _T("NPEM_EXECUTE"), MB_OK | MB_ICONERROR);
}

void CMsgTesterMenu::funcCompileExecuteFile()
{
	// Create the message to send to NPPExec
	NpeExecuteParam nep;
	nep.szScriptBody = _T("NPP_SAVE \n cd $(CURRENT_DIRECTORY) \n javac $(FILE_NAME) \n java $(NAME_PART)");
	nep.dwResult = 0; // send notification when executed

	// Send message and check for failure
	CNppExecPluginMsgSender npeMsgr(thePlugin.getNppWnd(), thePlugin.getDllFileName());
	if (npeMsgr.NpeExecute(&nep) != NPE_EXECUTE_OK)
		::MessageBox(thePlugin.getNppWnd(), _T("Operation failed"), _T("NPEM_EXECUTE"), MB_OK | MB_ICONERROR);
}

void CMsgTesterMenu::funcCompileProject()
{

}

void CMsgTesterMenu::funcJARProject()
{

}
