#pragma once
#include <iostream>
#include <filesystem>
#include <vector>

namespace fsys = std::filesystem;

class ProjectInfo{
    public:
        std::string ProjectName;
        fsys::path ProjectRoot;
        fsys::path IncludeDirectory;
        std::vector <fsys::path> SourceFiles;
        std::vector <fsys::path> SourceIncludes;
        std::string CppStandard;
        std::string CMakeVersion;
};
