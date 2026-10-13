#include "ProjectInfo.h"
#include <iostream>
#include <sstream>
#include <filesystem>


std::string GenerateCMakeLists(const ProjectInfo& Info)
{
    
    //generating CMakeLists.txt
    std::stringstream CMakeContent;
    CMakeContent << "cmake_minimum_required(VERSION " << Info.CMakeVersion << ")\n";
    CMakeContent << "project(" << Info.ProjectName << ")\n\n";

    CMakeContent << "set(CMAKE_CXX_STANDARD " << Info.CppStandard << ")\n";
    CMakeContent << "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n\n";

    CMakeContent << "add_executable(" << Info.ProjectName;
    for (const auto& src : Info.SourceFiles)
    {
        fsys::path relativePath = fsys::relative(src, Info.ProjectRoot);
        CMakeContent << " " << relativePath.string();
    }
    CMakeContent << ")\n";

    //handling Include directory if it exists
    if (!Info.IncludeDirectory.empty())
    {
        CMakeContent << "\ntarget_include_directories(" << Info.ProjectName << " PRIVATE " << Info.IncludeDirectory.string() << ")\n";
    }

    return CMakeContent.str();

}