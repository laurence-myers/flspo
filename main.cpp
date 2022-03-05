#include <filesystem>
#include <fstream>
#include <iostream>

#include "read.h"

enum class ExitCode : uint8_t {
    Okay,
    MissingArg,
    InvalidDirectory
};

int main(int argc, char* argv[]) {
    std::string pluginDbPath;

    if (argc >= 2) {
        pluginDbPath = argv[1];
    } else {
        std::string homeDir = getenv("UserProfile");
        if (homeDir.empty()) {
            std::cerr << "Please pass the path to your plugin database as the first arg.";
            return static_cast<int>(ExitCode::MissingArg);
        }
        // Assumes we're on Windows
        pluginDbPath = homeDir + R"(\Documents\Image-Line\FL Studio\Presets\Plugin database)";
    }

    if (!std::filesystem::exists(pluginDbPath)) {
        std::cerr << "Directory does not exist: " << pluginDbPath << std::endl;
        return static_cast<int>(ExitCode::InvalidDirectory);
    }
    if (!std::filesystem::is_directory(pluginDbPath)) {
        std::cerr << "Given path is not a directory: " << pluginDbPath << std::endl;
        return static_cast<int>(ExitCode::InvalidDirectory);
    }

    pluginDbPath += "\\Installed";

    if (!std::filesystem::exists(pluginDbPath)) {
        std::cerr << "Directory does not exist: " << pluginDbPath << std::endl;
        return static_cast<int>(ExitCode::InvalidDirectory);
    }
    if (!std::filesystem::is_directory(pluginDbPath)) {
        std::cerr << "Given path is not a directory: " << pluginDbPath << std::endl;
        return static_cast<int>(ExitCode::InvalidDirectory);
    }

    PluginByVendorMap pluginMap = walkDirectories(pluginDbPath);
}
