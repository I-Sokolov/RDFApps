
#include "pch.h"
#include "PSDChecker.h"

static int TestReadPSD(std::string path)
{    
    try {
        printf("--- Test read %s\n", path.c_str());
        RDF::PSD::PropertySetDef psd;
        psd.Load(path.c_str(), true);
        return 0;
    }
    catch (exception ex) {
        printf("%s\n", ex.what());
        return 1;
    }
}


static int TestFolder(std::string psdFolder)
{
    int res = 0;

    if (psdFolder.back() != '\\' && psdFolder.back() != '/') {
        psdFolder.push_back('\\');
    }

    WIN32_FIND_DATA ffd;
    auto hFind = FindFirstFile((psdFolder + "*").c_str(), &ffd);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::string path(psdFolder);
            path.append(ffd.cFileName);
            if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (strcmp(ffd.cFileName, ".") && strcmp(ffd.cFileName, "..")) {
                    res = TestFolder(path);
                }
            }
            else if (0 == _stricmp(ffd.cFileName + strlen(ffd.cFileName) - 4, ".xml")) {
                res += TestReadPSD(path);
            }
        } while (FindNextFile(hFind, &ffd) != 0);
    }

    FindClose(hFind);

    return res;
}


extern int SmokeTest(const char* psdFolder)
{
    int res = 0;

    res += TestFolder(psdFolder);

    return res;
}