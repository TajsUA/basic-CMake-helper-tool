#include "ProjectScanner.h"
#include "CMakeGenerator.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fsys = std::filesystem;

int main()
{
    start:

    //asking for project root path
    fsys::path ProjectRoot;
    std::cout << "CMakeLists.txt needs to be present (even empty) in directory in order for it to be identified as a project!" << std::endl;
    std::cout << "Provide path to project root (no spaces)." << std::endl << "Example: /home/user/documents/project" << std::endl;
    std::cin >> ProjectRoot;

    //checking if provided path is valid
    fsys::path CMakeLists = ProjectRoot / "CMakeLists.txt";
    if (!fsys::exists(CMakeLists) or !fsys::is_regular_file(CMakeLists))
    {
        //handling potentialy invalid path
        std::cout << "Directory can't be identified as a project root, CMakeLists.txt not detected, are you sure '" << ProjectRoot.string();
        std::cout << "' is your project root?" << std::endl << "continue? (y/n): ";
        char ContinueChoice;
        std::cin >> ContinueChoice;
        if (ContinueChoice == 'N' || ContinueChoice == 'n')
        {
            char RestartChoice;
            std::cout << "Restart tool? (y/n): ";
            std::cin >> RestartChoice;
            if (RestartChoice == 'Y' || RestartChoice == 'y')
            {
                goto start;
            }
            return 1;
        } 
    }
    std::cout << "Path identified as project root." << std::endl;

    //scanning project
    ProjectInfo Info = ScanProject(ProjectRoot);
    
    //asking for project name and handling default option
    std::cout << "Provide project name or type '-' for default: ";
    std::cin >> Info.ProjectName;
    if (Info.ProjectName == "-")
    {
        Info.ProjectName = ProjectRoot.filename().string();
    }

    //asking for CMake version and handling default option
    std::cout << "Provide CMake version or type '-' for default (3.20): ";
    std::cin >> Info.CMakeVersion;
    if (Info.CMakeVersion == "-")
    {
        Info.CMakeVersion = "3.20";
    }

    //asking for C++ standard and handling default option
    std::cout << "Provide C++ standard or type '-' for default (17): ";
    std::cin >> Info.CppStandard;
    if (Info.CppStandard == "-")
    {
        Info.CppStandard = "17";
    }

    //proposing scan results output
    char OutputScanChoice;
    std::cout << "Output scanning results? (y/n): ";
    std::cin >> OutputScanChoice;
    if (OutputScanChoice == 'Y' || OutputScanChoice == 'y')
    {
        //outputting scan results
        std::cout << "\nScan results:" <<std::endl;
        std::cout << "Source files found: " << Info.SourceFiles.size() << std::endl;
        std::cout << "Header files found: " << Info.SourceIncludes.size() << std::endl;
        if (!Info.IncludeDirectory.empty())
        {
            std::cout << "Include directory: " << Info.IncludeDirectory.string() << std::endl;
        }
        else
        {
            std::cout << "No dedicated Include found, or not identidied." << std::endl;
        }
        
    }

    //generating CMakeLists.txt
    std::string CMakeText = GenerateCMakeLists(Info);

    //asking for outputing generated CMakeLists.txt
    char OutputCMakeListsChoice;
    std::cout << "Output generated CMakeLists.txt in console? (y/n): ";
    std::cin >> OutputCMakeListsChoice;
    if (OutputCMakeListsChoice == 'Y' || OutputCMakeListsChoice == 'y')
    {
        std::cout << "Generated CMakeLists.txt: " << std::endl << CMakeText << std::endl;
    }

    //asking for writing CMakeLists content in CMakeLists.txt
    char WriteInFileChoice;
    std::cout << "Write generated CMakeLists content at " << ProjectRoot.string() << "/CMakeLists.txt? (y/n): " << std::endl;
    std::cin >> WriteInFileChoice;
    if (WriteInFileChoice == 'Y' || WriteInFileChoice == 'y')
    {
        //rewriting CMakeLists.txt
        std::ofstream OutFile(CMakeLists);
        if (!OutFile.is_open())
        {
            std::cout << "Failed to open file for writing." << std::endl;
            return 1;
        }
        OutFile << CMakeText;
        OutFile.close();
        std::cout << "CMakeLists.txt generated successfully at: " << ProjectRoot.string() << "/CMakeLists.txt" << std::endl;
    }
    
    return 0;
}