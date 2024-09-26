#include "pch.h"
#include "IDS.h"
#include "IDSRun.h"


static int s_testNum = 0;

extern int IDSRun(std::string const& idsFile, std::string const& ifcFile, bool stopOnError, RDF::IDS::MsgLevel level)
{
    ++s_testNum;

    //if (s_testNum < 182)
    //   return;

            
    printf("<Check num='%d', filepath='%s' stopOnError='%s' msgLevel='%d'>\n", s_testNum, ifcFile.c_str(), stopOnError ? "yes" : "no", level);

    bool ok = false;

    RDF::IDS::File ids;
    if (ids.Read(idsFile.c_str())) {
        SdaiModel model = sdaiOpenModelBN((SdaiRep)0, ifcFile.c_str(), "");
        if (model) {
            ok = ids.Check(model, stopOnError, level);
        }
        else {
            printf(" <ERROR>\n  Failed to read IFC file: %s\n </ERROR>\n", ifcFile.c_str());
        }
        sdaiCloseModel(model);
    }
    else {
        printf(" <ERROR>\n  Failed to read IDS file: %s\n", idsFile.c_str());
    }

    //test mode
    //
    if (idsFile.find("\\pass-") != std::string::npos) {
        if (!ok) {
            printf(" <ERROR>\n  pass- test failed\n </ERROR>");
            assert(0);
        }
    }
    else if (idsFile.find("\\fail-") != std::string::npos || idsFile.find("\\invalid-") != std::string::npos){
        if (ok) {
            printf(" <ERROR>\n  fail- or invalid- test failed\n </ERROR>");
            assert(0);
        }
        ok = !ok;
    }
    else {
        printf(" <ERROR>\n  unsupported file prefix, expected pass- or fail-\n </ERROR>");
        assert(0);
        ok = false;
    }
    //

    printf(" <Result>\n  %s\n </Result>\n</Check>\n", ok ? "OK" : "FAIL");

    return ok ? 0 : 1;
}

extern int IDSRun_dir(const char* folder, bool stopOnError, RDF::IDS::MsgLevel level, const char* filter)
{
    int err = 0;

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
                    err += IDSRun_dir(path.c_str(), stopOnError, level, filter);
                }
            }
            else if (0 == _stricmp(ffd.cFileName + strlen(ffd.cFileName) - 4, ".ids")) {
                auto ifcpath = path.substr(0, path.length() - 3);
                if (!filter || !*filter || ifcpath.find(filter) != std::string::npos) {
                    ifcpath.append("ifc");
                    err += IDSRun(path, ifcpath, stopOnError, level);
                }
            }
        } while (FindNextFile(hFind, &ffd) != 0);
    }

    FindClose(hFind);
    
    return err;
}

