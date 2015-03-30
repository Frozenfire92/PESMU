#include "MsgTesterMenu.h"
#include "MsgTester.h"
#include "NppExecPluginMsgSender.h"
#include <Commdlg.h>
#include <Windows.h>
#include <shobjidl.h>


extern CMsgTester thePlugin;

FuncItem CMsgTesterMenu::arrFuncItems[N_NBFUNCITEMS] = {
    { _T("Compile file"), funcCompileFile, 0, false, NULL },
    { _T("Execute file"), funcExecuteFile, 0, false, NULL },
    { _T("Compile and Execute file"), funcCompileExecuteFile, 0, false, NULL },
    { _T("Compile Project"), funcCompileProject, 0, false, NULL },
    { _T("Run Project"), funcExecuteProject, 0, false, NULL },
    { _T("Create JAR"), funcJARProject, 0, false, NULL }
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

//Parses an xml file, and places the contents of any file names with the .java extension in sumURI, separated by spaces.
//'folder' must point to the top-level project element in the tiny-xml doc.
//'projectLocation' is the base location of the workspace file we are parsing.
//DOES NOT SUPPORT file names outside of 'projectLocation' directory! Later compilation with ignore these files, with an error message.
void CMsgTesterMenu::funcParseXMLFolder(tinyxml2::XMLElement* folder, std::string& sumURI, const std::string projectLocation, int recurse)
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
            //Now parse the remaining subfolders
            while (subFolder->NextSiblingElement("Folder") != NULL)
            {
                subFolder = subFolder->NextSiblingElement("Folder");
                funcParseXMLFolder(subFolder, sumURI, projectLocation);
            }
        }
    }
}

//Compiles the specified project to the specified directory.
void CMsgTesterMenu::funcCompileProject()
{
    //Get the workspace location from user
    PWSTR pWorkspaceLocation = funcGetMenu();
    if (pWorkspaceLocation == NULL) return;
    //get location as a char*
    char  workspacePath[MAXCMDSIZE/32];
    wcstombs(workspacePath, pWorkspaceLocation, MAXCMDSIZE / 32);
    //Parse the location so it excludes the file name
    std::string path = workspacePath;
    size_t pos = path.find_last_of("\\");
    if (pos < path.size()) path = path.substr(0, pos + 1);
    char* parsedWorkSpacePath = (char* )path.c_str();
    //Debug: print parsedWorkSpacePath
    //TCHAR* cmdd = convertCharArrayToLPCWSTR(parsedWorkSpacePath);
    //::MessageBox(thePlugin.getNppWnd(), cmdd, _T("NPPCompileProject"), MB_OK | MB_ICONERROR);

    //Ask the user for the folder to compile to, and convert it to char*
    PWSTR pClassLocation = funcGetMenu(FOS_PICKFOLDERS);
    if (pClassLocation == NULL) return;
    char classLocationPath[MAXCMDSIZE/32];
    wcstombs(classLocationPath, pClassLocation, MAXCMDSIZE / 32);

    //Grab the xml document and print error if unsuccessful
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError err = doc.LoadFile(workspacePath);
    if (err != tinyxml2::XML_SUCCESS)
    {
        ::MessageBox(thePlugin.getNppWnd(), _T("Error opening workspace file"), _T("NPPCompileProject"), MB_OK | MB_ICONERROR);
        ::MessageBox(thePlugin.getNppWnd(), pWorkspaceLocation, _T("NPPCompileProject"), MB_OK | MB_ICONERROR);
        return;
    }

    //Parse XML
    tinyxml2::XMLElement* project = doc.FirstChildElement("NotepadPlus")->FirstChildElement("Project");
    std::string sumURI = "";
    std::string projectLocation(parsedWorkSpacePath);
    funcParseXMLFolder(project, sumURI, projectLocation);
    //Compile the project to the target folder
    funcCompileToFolder(classLocationPath, sumURI.c_str());
}

//Save the current file to a specified directory
void CMsgTesterMenu::funcCompileToFolder(const char* directory)
{
    //Need to convert directory to TCHAR*
    char directory2[MAXCMDSIZE];
    strcpy_s(directory2, MAXCMDSIZE, "NPP_SAVE \n cd $(CURRENT_DIRECTORY) \n javac $(FILE_NAME) -d ");
    strcat_s(directory2, MAXCMDSIZE, directory);
    const TCHAR* cmd = convertCharArrayToLPCWSTR(directory2);
    //Debug: print command
    //::MessageBox(thePlugin.getNppWnd(), cmd, _T("NPEM_EXECUTE"), MB_OK | MB_ICONERROR);
    funcExecCommand(cmd);
    delete cmd;
}

//Save the specified file to the specified directory.
void CMsgTesterMenu::funcCompileToFolder(const char* directory, const char* fileURI)
{
    char directory2[MAXCMDSIZE];
    strcpy_s(directory2, MAXCMDSIZE, "NPP_SAVE \n javac ");
    strcat_s(directory2, MAXCMDSIZE, fileURI);
    if (directory != NULL){
        strcat_s(directory2, MAXCMDSIZE, " -d ");
        strcat_s(directory2, MAXCMDSIZE, directory);
    }
    const TCHAR* cmd = convertCharArrayToLPCWSTR(directory2);
    funcExecCommand(cmd);
    delete cmd;
}

//Execute a given command on specified file(s), with output ('d' flag) set to the specified directory.
void CMsgTesterMenu::funcCompileToFolder(const char* directory, const char* fileURI, const char* command)
{
    char directory2[MAXCMDSIZE];
    strcpy_s(directory2, MAXCMDSIZE, command);
    strcat_s(directory2, MAXCMDSIZE, fileURI);
    if (directory != NULL){
        strcat_s(directory2, MAXCMDSIZE, " -d ");
        strcat_s(directory2, MAXCMDSIZE, directory);
    }
    const TCHAR* cmd = convertCharArrayToLPCWSTR(directory2);
    funcExecCommand(cmd);
    delete cmd;
}

//Creates a JAR file of the file(s) @ directory, with jarName.
//Optionally supports directory = null, in which case we create it @ current NPP directory.
void CMsgTesterMenu::funcCompileJarToFolder(const char* directory, const char* fileURI, const char* jarName)
{
    char directory2[MAXCMDSIZE];
    if (directory != NULL){
        strcpy_s(directory2, MAXCMDSIZE, "NPP_SAVE \n cd ");
        strcat_s(directory2, MAXCMDSIZE, directory);
    }
    strcat_s(directory2, MAXCMDSIZE, " \n jar cvf ");
    strcat_s(directory2, MAXCMDSIZE, jarName);
    strcat_s(directory2, MAXCMDSIZE, " ");
    strcat_s(directory2, MAXCMDSIZE, fileURI);
    const TCHAR* cmd = convertCharArrayToLPCWSTR(directory2);
    funcExecCommand(cmd);
    delete cmd;
}

//Executes a generic command through NPPExec; prints message if it fails.
void CMsgTesterMenu::funcExecCommand(const TCHAR* command)
{
    NpeExecuteParam nep;
    nep.szScriptBody = command;
    nep.dwResult = 0; // Don't send notification after the script will be executed

    // Send message and check for failure
    CNppExecPluginMsgSender npeMsgr(thePlugin.getNppWnd(), thePlugin.getDllFileName());
    if (npeMsgr.NpeExecute(&nep) != NPE_EXECUTE_OK)
        ::MessageBox(thePlugin.getNppWnd(), _T("Error executing command: \n"), _T("NPEM_EXECUTE"), MB_OK | MB_ICONERROR);
    //debug:
    //::MessageBox(thePlugin.getNppWnd(), command, _T("NPEM_EXECUTE"), MB_OK | MB_ICONERROR);
}

//Asks the user for a workspace and jar name & location, and creates it.
//Prints error if fails, does nothing if user exits menu(s)
void CMsgTesterMenu::funcJARProject()
{
    //Get the workspace from the user
    PWSTR pWorkspaceLocation = funcGetMenu();
    if (pWorkspaceLocation == NULL) return;
    char  workspacePath[MAXCMDSIZE / 32];
    wcstombs(workspacePath, pWorkspaceLocation, MAXCMDSIZE / 32);
    //Parse out workspace name
    std::string path = workspacePath;
    size_t pos = path.find_last_of("\\");
    if (pos < path.size()) path = path.substr(0, pos + 1);
    char* parsedWorkspacePath = (char*)path.c_str();
    //Debug:
    //TCHAR* cmdd = convertCharArrayToLPCWSTR(parsedWorkspacePath);
    //::MessageBox(thePlugin.getNppWnd(), cmdd, _T("NPPCompileProject"), MB_OK | MB_ICONERROR);

    //Get the JAR name and location from the user
    PWSTR pfileNameLocation = funcGetSaveMenu(NULL);
    if (pfileNameLocation == NULL) return;
    char fileNamePath[MAXCMDSIZE / 32];
    wcstombs(fileNamePath, pfileNameLocation, MAXCMDSIZE / 32);
    std::string sPath = fileNamePath;
    std::string fileName = "";
    size_t posF = sPath.find_last_of("\\");
    //Get file path and name as separate char*
    if (posF < sPath.length() + 1) {
        fileName = sPath.substr(posF + 1);
        sPath = sPath.substr(0, posF + 1);
    }
    else{
        ::MessageBox(thePlugin.getNppWnd(), _T("Invalid file specified!"), _T("NPPCompileProject"), MB_OK | MB_ICONERROR);
        return;
    }
    char* cFilePath = (char*)sPath.c_str();
    char* cFileName = (char*)fileName.c_str();

    //Grab the xml document and print error if unsuccessful
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError err = doc.LoadFile(workspacePath);
    if (err != tinyxml2::XML_SUCCESS)
    {
        ::MessageBox(thePlugin.getNppWnd(), _T("Error opening workspace file"), _T("NPPCompileProject"), MB_OK | MB_ICONERROR);
        return;
    }

    //Parse XML
    tinyxml2::XMLElement* project = doc.FirstChildElement("NotepadPlus")->FirstChildElement("Project");
    std::string sumURI = "";
    std::string projectLocation(parsedWorkspacePath);
    funcParseXMLFolder(project, sumURI, projectLocation);
    //Create Jar with name and folder specified
    funcCompileJarToFolder(cFilePath, sumURI.c_str(), cFileName);
}

//Asks the user for a main .class file, and executes it.
//Does nothing if users exits menu, and NPPExec prints error if file is invalid.
void CMsgTesterMenu::funcExecuteProject(){
    //Get file name
    PWSTR pFileLocation = funcGetMenu();
    if (pFileLocation == NULL) return;
    char  filePath[MAXCMDSIZE / 32];
    wcstombs(filePath, pFileLocation, MAXCMDSIZE / 32);
    //Parse out the file name and path name as char*
    std::string path = filePath;
    std::string fileName = "";
    size_t pos = path.find_last_of("\\");
    if (pos < path.length() + 1) {
        fileName = path.substr(pos + 1);
        size_t pos2 = fileName.find_last_of(".");
        fileName = fileName.substr(0, pos2);
        path = path.substr(0, pos + 1);
    }
    else{
        ::MessageBox(thePlugin.getNppWnd(), _T("Invalid file specified!"), _T("NPPCompileProject"), MB_OK | MB_ICONERROR);
        return;
    }
    char* cPath = (char*)path.c_str();
    char* cFileName = (char*)fileName.c_str();

    //Executes 2 commands custom to this function; Could also use ::funcExecuteCommand twice
    char directory2[MAXCMDSIZE];
    strcpy_s(directory2, MAXCMDSIZE, "NPP_SAVE \n cd ");
    strcat_s(directory2, MAXCMDSIZE, cPath);
    strcat_s(directory2, MAXCMDSIZE, " \n java ");
    strcat_s(directory2, MAXCMDSIZE, cFileName);
    const TCHAR* cmd = convertCharArrayToLPCWSTR(directory2);
    funcExecCommand(cmd);
    delete cmd;
}

//Creates a FileOpenDialog, and returns the selected fileURI or null otherwise.
//'options' specifies options from FOS_<>, including if we want to select a folder (FOS_PICKFOLDERS)
PWSTR CMsgTesterMenu::funcGetMenu(DWORD options){
    PWSTR returnVal = NULL;
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog *pFileOpen;

        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
        
        if (SUCCEEDED(hr))
        {
            DWORD dwFlags;

            hr = pFileOpen->GetOptions(&dwFlags);
            if (SUCCEEDED(hr))
            {
                hr = pFileOpen->SetOptions(dwFlags | options);

                if (SUCCEEDED(hr))
                {
                // Show the Open dialog box.
                    hr = pFileOpen->Show(NULL);

                    // Get the file name from the dialog box.
                    if (SUCCEEDED(hr))
                    {
                        IShellItem *pItem;
                        hr = pFileOpen->GetResult(&pItem);
                        if (SUCCEEDED(hr))
                        {
                            PWSTR pszFilePath;
                            hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                            // Display the file name to the user.
                            if (SUCCEEDED(hr))
                            {
                                //MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
                                returnVal = pszFilePath;
                            }
                            pItem->Release();
                        }
                    }
                    pFileOpen->Release();
                }
            }
        }
        CoUninitialize();
    }
    return returnVal;
}

//Creates a Save as.. dialog, with ONLY .JAR file available, and returns fileURI or NULL if unsuccessful.
PWSTR CMsgTesterMenu::funcGetSaveMenu(DWORD options){
    PWSTR returnVal = NULL;
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileSaveDialog *pFileOpen;

        hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
            IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            DWORD dwFlags;

            hr = pFileOpen->GetOptions(&dwFlags);
            if (SUCCEEDED(hr))
            {
                hr = pFileOpen->SetOptions(dwFlags | options);

                if (SUCCEEDED(hr))
                {
                    COMDLG_FILTERSPEC c_rgSaveTypes[] =
                    {
                        { L"JAR file", L"*.jar" }
                    };

                    hr = pFileOpen->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
                    if (SUCCEEDED(hr))
                    {

                        hr = pFileOpen->SetDefaultExtension(L".jar");
                        if (SUCCEEDED(hr))
                        {

                            // Show the Open dialog box.
                            hr = pFileOpen->Show(NULL);

                            // Get the file name from the dialog box.
                            if (SUCCEEDED(hr))
                            {
                                IShellItem *pItem;
                                hr = pFileOpen->GetResult(&pItem);
                                if (SUCCEEDED(hr))
                                {
                                    PWSTR pszFilePath;
                                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                                    // Display the file name to the user.
                                    if (SUCCEEDED(hr))
                                    {
                                        //MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
                                        returnVal = pszFilePath;
                                    }
                                    pItem->Release();
                                }
                            }
                            pFileOpen->Release();
                        }
                    }
                }
            }
        }
        CoUninitialize();
    }
    return returnVal;
}
