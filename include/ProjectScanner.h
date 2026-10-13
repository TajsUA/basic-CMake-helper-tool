#pragma once
#include "ProjectInfo.h"
#include <filesystem>

namespace fsys = std::filesystem;

ProjectInfo ScanProject(const fsys::path& ProjectRoot);
