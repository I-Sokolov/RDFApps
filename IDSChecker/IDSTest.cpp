#include "pch.h"
#include "IDS.h"
#include "IDSTest.h"

bool stopAtFirstError;
RDF::IDS::MsgLevel msgLevel;

static void IDSTest(std::string& idsFile, std::string& ifcFile)
{
    //make various options
    stopAtFirstError = !stopAtFirstError;
    msgLevel = (RDF::IDS::MsgLevel)((int(msgLevel) + 1) % (int(RDF::IDS::MsgLevel::Error)+1));
        
        
    printf("<Test idspath='%s' stopAtFirstError='%d' msgLevel='%d'>\n", idsFile.c_str(), stopAtFirstError, msgLevel);

    bool ok = false;

    RDF::IDS::File ids;
    if (ids.Read(idsFile.c_str())) {
        ok = ids.Check(ifcFile.c_str(), stopAtFirstError, msgLevel);
    }
    else {
        assert(!"Failed to read IDS file");
    }

    bool pass = (idsFile.find("pass")!=std::string::npos);

    //assert(ok == pass);

    printf("</Test>\n");
}

static void IDSTest(const char* folder)
{
    std::string wc(folder);
    wc.append("\\*");

    WIN32_FIND_DATA ffd;
    auto hFind = FindFirstFile(wc.c_str(), &ffd);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::string path(folder);
            path.append("\\");
            path.append(ffd.cFileName);
            if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (ffd.cFileName[0] != '.') {
                    IDSTest(path.c_str());
                }
            }
            else if (0 == _stricmp(ffd.cFileName + strlen(ffd.cFileName) - 4, ".ids")) {
                auto ifcpath = path.substr(0, path.length() - 3);
                ifcpath.append("ifc");
                IDSTest(path, ifcpath);
            }
        } while (FindNextFile(hFind, &ffd) != 0);
    }

    FindClose(hFind);

}

extern void IDSTest()
{
    IDSTest(R"(E:\DevArea\buildingSMART\IDS\Documentation\testcases)");
}