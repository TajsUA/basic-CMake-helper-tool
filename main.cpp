#include <iostream>
#include <filesystem>
#include <vector>
#include <sstream>
#include <fstream>
namespace fsys = std::filesystem;

int main()
{
   start:

   //asking for project root path
   fsys::path InputPath;
   std::cout << "provide path to project root (no spaces)" << std::endl << "example: /home/user/documents/project" << std::endl;
   std::cin >> InputPath;

   //scanning CmakeLists.txt
   fsys::path ProjectLists = InputPath / "CMakeLists.txt";
   if (fsys::exists(ProjectLists))
   {
      std::cout << "CMakeLists.txt found" << std::endl;
   }
   else
   {
      //invalid path handling
      std::cout << "path can't be identified as project root without CMakeLists.txt being present" << std::endl << "restart tool? (y/n): ";
      char restart;
      std::cin >> restart;
      if (restart == 'Y' or restart == 'y')
      {
         goto start;
      }
      return 1;
   }
   
   //asking for Cmake version
   std::string InputCMakeVersion;
   std::cout << "provide Cmake version (type '-' for default (3.16))" << std::endl;
   std::cin >> InputCMakeVersion;
   if (InputCMakeVersion == "-")
   {
      InputCMakeVersion = "3.20";
   }

   //asking for C++ standard
   std::string InputCppStandard;
   std::cout << "provide C++ standard (type '-' for default (17))" << std::endl;
   std::cin >> InputCppStandard;
   if (InputCppStandard == "-")
   {
      InputCppStandard = "17";
   }
   
   //scanning source
   std::vector<fsys::path> SourceFiles;
   fsys::path ProjectSource = InputPath / "src";
   if (fsys::exists(ProjectSource))
   {
      std::cout << "source found" << std::endl;

      //looking for c++ files
      for (const auto& entry : fsys::recursive_directory_iterator(ProjectSource))
      {
        if (entry.is_regular_file())
         {
            auto ext = entry.path().extension().string();
            if (ext == ".cpp" || ext == ".cc" || ext == ".cxx")
            {
               SourceFiles.push_back(entry.path());
               std::cout << "found: " << entry.path().filename().string() << std::endl;
            }
         }
      }
   }
   else
   {
      std::cout << "source not found" << std::endl;
   }

   //scanning include
   std::vector<fsys::path> HeaderFiles;
   fsys::path ProjectInclude = InputPath / "include";
   if (fsys::exists(ProjectInclude))
   {
      std::cout << "include found" << std::endl;

      //looking for header files
      for (const auto& entry : fsys::recursive_directory_iterator(ProjectInclude))
      {
        if (entry.is_regular_file())
         {
            auto ext = entry.path().extension().string();
            if (ext == ".h" || ext == ".hpp" || ext == ".hxx")
            {
               HeaderFiles.push_back(entry.path());
               std::cout << "found: " << entry.path().filename().string() << std::endl;
            }
         }
      }
   }
   else
   {
      std::cout << "include not found" << std::endl;
   }

   //preventing program from writing empty project name if user writes path with "/" at the end
   std::string ProjectName = InputPath.filename().string();
   if (ProjectName.empty())
   {
      ProjectName = InputPath.parent_path().filename().string();
   }

   //assembling CMake content
   std::stringstream CMakeContent;
   CMakeContent << "cmake_minimum_required(VERSION " << InputCMakeVersion << ")\n";
   CMakeContent << "project(" << ProjectName << ")\n\n";

   CMakeContent << "set(CMAKE_CXX_STANDARD " << InputCppStandard << ")\n";
   CMakeContent << "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n\n";

   CMakeContent << "add_executable(" << ProjectName;
   for (const auto& src : SourceFiles)
   { 
      fsys::path relativePath = fsys::relative(src, InputPath);
      CMakeContent << " " << relativePath.string();
   }
   CMakeContent << ")\n";

   if (fsys::exists(ProjectInclude))
   {
      CMakeContent << "\ntarget_include_directories(" << ProjectName << " PRIVATE include)\n";
   }

   //rewriting CMakeLists.txt
   std::ofstream outFile(InputPath / "CMakeLists.txt");
   outFile << CMakeContent.str();   
   outFile.close();
}