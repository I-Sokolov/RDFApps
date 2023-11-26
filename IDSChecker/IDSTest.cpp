#include "pch.h"
#include "IDS.h"
#include "IDSTest.h"

static void IDSTest(std::string& idsFile, std::string& ifcFile)
{
    printf("<Test idspath='%s'>\n", idsFile.c_str());

    RDF::IDS::File ids;
    ids.Read(idsFile.c_str());

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
