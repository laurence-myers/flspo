#include <filesystem>
#ifdef WIN32
#include <windows.h>
#endif

#include <nowide/args.hpp>
#include <nowide/cstdlib.hpp>
#include <nowide/iostream.hpp>
#include <nowide/convert.hpp>

#include "read.h"
#include "write.h"

enum class ExitCode : uint8_t {
    Okay,
    MissingArg,
    InvalidDirectory,
    UnhandledError
};

int main(int argc, char* argv[]) {
    nowide::args args ( argc, argv );

#ifdef WIN32
    SetConsoleCP(65001); // UTF-8 on Windows
#endif

    std::filesystem::path pluginDbPath;

    if (argc >= 2) {
        pluginDbPath = nowide::widen(argv[1]);
    } else {
        std::string homeDir { nowide::getenv("UserProfile") };
        if (homeDir.empty()) {
            nowide::cerr << "Please pass the path to your plugin database as the first arg.";
            return static_cast<int>(ExitCode::MissingArg);
        }
        // Assumes we're on Windows
        pluginDbPath = homeDir + R"(\Documents\Image-Line\FL Studio\Presets\Plugin database)";
    }

    if (!std::filesystem::exists(pluginDbPath)) {
        nowide::cerr << "Directory does not exist: " << pluginDbPath.string() << std::endl;
        return static_cast<int>(ExitCode::InvalidDirectory);
    }
    if (!std::filesystem::is_directory(pluginDbPath)) {
        nowide::cerr << "Given path is not a directory: " << pluginDbPath.string() << std::endl;
        return static_cast<int>(ExitCode::InvalidDirectory);
    }

    std::filesystem::path installedPluginDbPath = pluginDbPath / "Installed";

    if (!std::filesystem::exists(installedPluginDbPath)) {
        nowide::cerr << "Directory does not exist: " << installedPluginDbPath.string() << std::endl;
        return static_cast<int>(ExitCode::InvalidDirectory);
    }
    if (!std::filesystem::is_directory(installedPluginDbPath)) {
        nowide::cerr << "Given path is not a directory: " << installedPluginDbPath.string() << std::endl;
        return static_cast<int>(ExitCode::InvalidDirectory);
    }

    try {
        PluginByVendorMap pluginMap = walkDirectories(installedPluginDbPath);
        writeToPluginDatabase(pluginDbPath, pluginMap);
    } catch (std::exception &err) {
        nowide::cerr << err.what();
        return static_cast<int>(ExitCode::UnhandledError);
    }

    return static_cast<int>(ExitCode::Okay);
}
