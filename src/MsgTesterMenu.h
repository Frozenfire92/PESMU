#ifndef _npp_exec_plugin_msg_tester_menu_h_
#define _npp_exec_plugin_msg_tester_menu_h_
//----------------------------------------------------------------------------
#include "core/NppPluginMenu.h"
//#include "resource.h"
#include <string>
#include "tinyxml2.h"

class CMsgTesterMenu : public CNppPluginMenu
{
    public:
        enum NMenuItems {
            N_NPE_GETVERDWORD = 0,
            N_NPE_GETVERSTR,
            N_NPE_GETSTATE,
            N_NPE_PRINT,
            N_NPE_EXECUTE,
            N_NPE_NPPEXEC,
            
            N_NBFUNCITEMS
        };
        static FuncItem arrFuncItems[N_NBFUNCITEMS];

    protected:
          const static int MAXCMDSIZE = 8192;
		static void funcCompileFile();
		static void funcExecuteFile();
		static void funcCompileExecuteFile();
		static void funcCompileProject();
		static void funcJARProject();
          static void funcCompileFolder();
          static void funcCompileToFolder(const char*);
          static void funcCompileToFolder(const char*, const char*);
          static void funcExecCommand(const TCHAR*);
          static void funcParseXMLFolder(tinyxml2::XMLElement*, std::string&, std::string, int);
          static wchar_t* convertCharArrayToLPCWSTR(const char*);
};

//----------------------------------------------------------------------------
#endif
