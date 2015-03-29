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
    nep.dwResult = 0; // Don't send notification after the script will be executed

    // Send message and check for failure
    CNppExecPluginMsgSender npeMsgr(thePlugin.getNppWnd(), thePlugin.getDllFileName());
    if (npeMsgr.NpeExecute(&nep) != NPE_EXECUTE_OK)
        ::MessageBox(thePlugin.getNppWnd(), _T("Operation failed!"), _T("NPEM_EXECUTE"), MB_OK | MB_ICONERROR);
}

void CMsgTesterMenu::funcExecuteFile()
{
    // Create the message to send to NPPExec
    NpeExecuteParam nep;
    nep.szScriptBody = _T("cd $(CURRENT_DIRECTORY) \n java $(NAME_PART)");
    nep.dwResult = 0; // Don't send notification after the script will be executed

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
    nep.dwResult = 0; // Don't send notification after the script will be executed

    // Send message and check for failure
    CNppExecPluginMsgSender npeMsgr(thePlugin.getNppWnd(), thePlugin.getDllFileName());
    if (npeMsgr.NpeExecute(&nep) != NPE_EXECUTE_OK)
        ::MessageBox(thePlugin.getNppWnd(), _T("Operation failed"), _T("NPEM_EXECUTE"), MB_OK | MB_ICONERROR);
}

//Helper function to bypass limitations of LPCWSTR
wchar_t* CMsgTesterMenu::convertCharArrayToLPCWSTR(const char* charArray)
{
    wchar_t* wString = new wchar_t[MAXCMDSIZE];
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, MAXCMDSIZE);
    return wString;
}

void CMsgTesterMenu::funcParseXMLFolder(tinyxml2::XMLElement* folder, std::string& sumURI, const std::string projectLocation, int recurse = 0)
{
    if (!folder->NoChildren())
    {
        //Get the first file node
        tinyxml2::XMLElement* file;
        file = folder->FirstChildElement("File");
        if (file != NULL)
        {
            //Add the first file name to the sumURI string
            std::string localFileLocation = projectLocation;
            localFileLocation.append(file->Attribute("name"));
            //Only add files that use the .java extension
            if (localFileLocation.substr(localFileLocation.length() - 5, localFileLocation.length()).compare(".java") == 0)
            {
                sumURI.append(" ");
                sumURI.append(localFileLocation);
            }
            //Now get the remaining files and add their names to the sumURI string
            while (file->NextSiblingElement("File") != NULL)
            {
                file = file->NextSiblingElement("File");
                localFileLocation = projectLocation;
                localFileLocation.append(file->Attribute("name"));
                if (localFileLocation.substr(localFileLocation.length() - 5, localFileLocation.length()).compare(".java") == 0)
                {
                    sumURI.append(" ");
                    sumURI.append(localFileLocation);
                }
            }
        }
        //Now look if the folder had any subfolders. Assume it does, and parse it if non-null
        tinyxml2::XMLElement* subFolder;
        subFolder = folder->FirstChildElement("Folder");
        if (subFolder != NULL)
        {
            if (recurse > 0)
            {
                recurse--;
                funcParseXMLFolder(subFolder, sumURI, projectLocation, recurse);
            }
        }
        subFolder = subFolder->NextSiblingElement("Folder");
        if (subFolder != NULL)
        {
            if (recurse > 0)
            {
                recurse--;
                funcParseXMLFolder(subFolder, sumURI, projectLocation, recurse);
            }
        }
        subFolder = folder->LastChildElement("Folder");
        if (subFolder != NULL)
        {
            if (recurse > 0)
            {
                recurse--;
                funcParseXMLFolder(subFolder, sumURI, projectLocation, recurse);
            }
        }
        //Now parse the remaining subfolders
        //while (subFolder->NextSiblingElement("Folder") != NULL)
        //{
        //    subFolder = subFolder->NextSiblingElement("Folder");
        //    funcParseXMLFolder(subFolder, sumURI, projectLocation);
        //}

    }
}

void CMsgTesterMenu::funcCompileProject()
{
    //Set the workspace location
    char* workspaceLocation = "C:\\Users\\Cameron\\Desktop\\npp.6.6.7.bin\\project1\\";
    char* workspaceName = "workspace1.java";
    char* workspaceURI = "C:\\Users\\Cameron\\Desktop\\npp.6.6.7.bin\\project1\\workspace1.xml";
    char* classLocation = "C:\\Users\\Cameron\\Desktop\\npp.6.6.7.bin\\project1\\compiled";

    //Grab the xml document and print error if unsuccessful
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError err = doc.LoadFile(workspaceURI);
    if (err != tinyxml2::XML_SUCCESS)
    {
        ::MessageBox(thePlugin.getNppWnd(), _T("Error opening workspace file"), _T("NPPCompileProject"), MB_OK | MB_ICONERROR);
        return;
    }

    //Parse XML
    tinyxml2::XMLElement* project = doc.FirstChildElement("NotepadPlus")->FirstChildElement("Project");
    std::string sumURI = "";
    std::string projectLocation(workspaceLocation);
    funcParseXMLFolder(project, sumURI, projectLocation, 2);
    //Compile the command to the target folder
    funcCompileToFolder(classLocation, sumURI.c_str());
}

void CMsgTesterMenu::funcCompileToFolder(const char* directory)
{
    //Need to convert directory to TCHAR*
    char directory2[MAXCMDSIZE];
    strcpy_s(directory2, MAXCMDSIZE, "NPP_SAVE \n cd $(CURRENT_DIRECTORY) \n javac $(FILE_NAME) -d ");
    strcat_s(directory2, MAXCMDSIZE, directory);
    const TCHAR* cmd = convertCharArrayToLPCWSTR(directory2);
    //Debug:
    ::MessageBox(thePlugin.getNppWnd(), cmd, _T("NPEM_EXECUTE"), MB_OK | MB_ICONERROR);
    funcExecCommand(cmd);
    delete cmd;
}

void CMsgTesterMenu::funcCompileToFolder(const char* directory, const char* fileURI)
{
    //Note fileURI must include workspace location!
    char directory2[MAXCMDSIZE];
    strcpy_s(directory2, MAXCMDSIZE, "NPP_SAVE \n javac ");
    strcat_s(directory2, MAXCMDSIZE, fileURI);
    strcat_s(directory2, MAXCMDSIZE, " -d ");
    strcat_s(directory2, MAXCMDSIZE, directory);
    //Debug:
    const TCHAR* cmd = convertCharArrayToLPCWSTR(directory2);
    funcExecCommand(cmd);
    delete cmd;
}

void CMsgTesterMenu::funcExecCommand(const TCHAR* command){
    NpeExecuteParam nep;
    nep.szScriptBody = command;
    nep.dwResult = 0; // Don't send notification after the script will be executed

    // Send message and check for failure
    CNppExecPluginMsgSender npeMsgr(thePlugin.getNppWnd(), thePlugin.getDllFileName());
    if (npeMsgr.NpeExecute(&nep) != NPE_EXECUTE_OK)
        ::MessageBox(thePlugin.getNppWnd(), _T("Error executing command: \n"), _T("NPEM_EXECUTE"), MB_OK | MB_ICONERROR);
    ::MessageBox(thePlugin.getNppWnd(), command, _T("NPEM_EXECUTE"), MB_OK | MB_ICONERROR);
}

void CMsgTesterMenu::funcJARProject()
{

}
