// PSDChecker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <pch.h>
#include <PSDChecker.h>

extern int SmokeTest(const char* psdFolder);


/// <summary>
/// 
/// </summary>
static int PrintUsage()
{
    printf("This application checks IFC files against Property Set Definitions\n");
    printf("Usage:\n");
    printf("   PSDChecker  <PSD directory> [/d] <IFC filepath/wildcard>\n");
    return -1;
}

/// <summary>
/// 
/// </summary>
static void CheckFolder(std::string dir, RDF::PSD::Checker& checker)
{    
    if (dir.back() != '/' && dir.back() != '\\') {
        dir.push_back('\\');
    }

    WIN32_FIND_DATA ffd;
    auto hFind = FindFirstFile((dir+"*").c_str(), &ffd);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::string path(dir);
            path.append(ffd.cFileName);
            if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (strcmp(ffd.cFileName, ".") && strcmp(ffd.cFileName, "..")) {
                    CheckFolder(path, checker);
                }
            }
            else if (0 == _stricmp(ffd.cFileName + strlen(ffd.cFileName) - 4, ".ifc")) {
                printf("Checking file %s\n", path.c_str());
                checker.Check(path.c_str());                
            }
        } while (FindNextFile(hFind, &ffd) != 0);
    }

    FindClose(hFind);
}

/// <summary>
/// 
/// </summary>
extern int main(int argc, char* argv[])
{
    if (argc < 3) return PrintUsage();

    auto psdFolder = argv[1];
    bool checkFolder = (argc > 3);
    auto ifcFile = argv[argc-1];

#ifdef _DO_TEST
    auto test = SmokeTest(psdFolder);
    assert(test == 0);
    return test;
#endif

    RDF::PSD::Checker checker(psdFolder);
    if (checkFolder) {
        CheckFolder(ifcFile, checker);
    }
    else {
        checker.Check(ifcFile);
    }

    return (int)checker.GetErrorLevel();
}
