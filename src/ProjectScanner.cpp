#include "ProjectInfo.h"
#include "ProjectScanner.h"
#include <filesystem>

namespace fsys = std::filesystem;
ProjectInfo ScanProject(const fsys::path& ProjectRoot)
{
    ProjectInfo info;
    info.ProjectRoot = ProjectRoot;
    
    //identyfying Include folder exact name
    std::vector<std::string> CandidateNames = { "include", "inc", "headers", "Include" };
    for (const auto& name : CandidateNames)
    {
        fsys::path Candidate = ProjectRoot / name;
        if (fsys::exists(Candidate) && fsys::is_directory(Candidate))
        {
            info.IncludeDirectory = Candidate;
            break;
        }
    }

//scanning project code and headers
try
{
    for (const auto& entry : fsys::recursive_directory_iterator(info.ProjectRoot, fsys::directory_options::skip_permission_denied))
    {
                if (!entry.is_regular_file()) continue;

        // skipping unnecessary files and directories
        std::string pathStr = entry.path().string();
        if (pathStr.find("/build/") != std::string::npos ||
            pathStr.find("/.git/") != std::string::npos ||
            pathStr.find("/.vscode/") != std::string::npos)
        {
            continue;
        }

        auto ext = entry.path().extension().string();

        //scanning sources
        if (ext == ".cpp" || ext == ".cc" || ext == ".cxx" || ext == ".c")
        {
            info.SourceFiles.push_back(entry.path());
        }

        //scanning headers
        else if (ext == ".h" || ext == ".hpp" || ext == ".hxx")
        {
            info.SourceIncludes.push_back(entry.path());
        }
    }
}
catch (const fsys::filesystem_error& e)
{
    std::cout << "Warning: error while scanning directory: " << e.what() << std::endl;
}

    return info;
}
