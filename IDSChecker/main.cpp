// IDSChecker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <pch.h>
#include "IDS.h"
#include "IDSRun.h"

static int PrintUsage()
{
    printf("This application checks IFC files against IDS specifications\n");
    printf("See https://github.com/buildingSMART/IDS/tree/master/Documentation\n");
    printf("Usage:\n");
    printf("   IDSChecker  <IDS file path> <IFC file path> [-s] [-lN]\n");
    printf("   IDSChecker  -d <path to folder with IDS and IFC files pairs> [-s] [-lN] [-f <filter>]\n");
    printf("      -s: stop on firts error\n");
    printf("      -l: output details level. 3 all, 2 warings, 1 errors\n");
    printf("      -f: filter file names by containment <filter> substring");
    printf("Exit code is 0 when no errors found\n");
    printf("(c) RDF Ltd https://rdf.bg\n");
    return -1;
}

static void CleanPath(std::string& path)
{
    if (path.empty()) {
        return;
    }

    if (path.front() == '\"' || path.front() == '\'') {
        path = path.c_str() + 1;
    }

    if (path.back() == '\"' || path.back() == '\'') {
        path.pop_back();
    }

    if (path.back() == '\\') {
        path.pop_back();
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) return PrintUsage();

    // Parse cmd line
    //
    std::string dir;
    std::string idsFile;
    std::string ifcFile;
    std::string filter;
    bool stopOnError = false;
    RDF::IDS::MsgLevel level = RDF::IDS::MsgLevel::All;

    int i = 0;

    if (0 == strcmp(argv[1], "-d")) {
        if (argc < 3) return PrintUsage();
        dir = argv[2];
        i = 3;
    }
    else {
        if (argc < 3) return PrintUsage();
        idsFile = argv[1];
        ifcFile = argv[2];
        i = 3;
    }

    CleanPath(dir);
    CleanPath(idsFile);
    CleanPath(ifcFile);

    for (; i < argc; i++) {
        if (0 == strcmp(argv[i], "-s")) {
            stopOnError = true;
        }
        else if (0 == strncmp(argv[i], "-l", 2)) {
            int lev = atoi(argv[i] + 2);
            switch (lev) {
                case 2: level = RDF::IDS::MsgLevel::Warning; break;
                case 1: level = RDF::IDS::MsgLevel::Error; break;
            }
        }
        else if (0 == strncmp(argv[i], "-f", 2)) {
            i++;
            if (i < argc) {
                filter = argv[i];
            }
            else {
                printf("\n\nExpected filter string argument\n\n");
                return PrintUsage();
            }
        }
        else {
            return PrintUsage();
        }
    }

    //
    // Run 
    //
    int err = 0;

    if (!dir.empty()) {
        err += IDSRun_dir(dir.c_str(), stopOnError, level, filter.c_str());
    }

    if (!idsFile.empty() && !ifcFile.empty()) {
        err += IDSRun(idsFile.c_str(), ifcFile.c_str(), stopOnError, level);
    }

    printf("(c) RDF Ltd https://rdf.bg\n");
    return err;
}
