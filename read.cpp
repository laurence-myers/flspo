#include <filesystem>
#include <map>

#include <nowide/convert.hpp>
#include <nowide/fstream.hpp>
#include <nowide/iostream.hpp>

#include "read.h"
#include "common.h"
#include "error.h"

typedef std::map<std::string, std::string> NfoData;

NfoData readNfoFile(const std::filesystem::path &filePath) {
    const std::string path { nowide::narrow(filePath.wstring()) }; // not quite right, but using `filePath` or `filePath.8string()` directly causes issues.
    nowide::cout << "Reading file: " << path << std::endl;
    nowide::ifstream file = nowide::ifstream ( path );
    NfoData data;
    if (file.is_open()) {
        std::string key;
        std::string value;
        while (std::getline(file, key, '=')) {
#if FLSPO_VERBOSE
            nowide::cout << "Key: " << key << std::endl;
#endif
            // split string into key & value
            std::getline(file, value);
#if FLSPO_VERBOSE
            nowide::cout << "Value: " << value << std::endl;
#endif
            // Store in NfoData
            data[key] = value;
        }
    }
    return data;
}

void walkDirectory(const std::filesystem::path &rootDirectory, PluginByVendorMap &pluginMap) {
    for (const auto &entry: std::filesystem::recursive_directory_iterator(rootDirectory)) {
        const auto path { entry.path() };
#if FLSPO_VERBOSE
        nowide::cout << (nowide::narrow(path.wstring())) << std::endl;
#endif
        if (entry.is_regular_file()
            && path.extension() == ".nfo"
            && path.filename() != "VerifiedIDs.nfo" // Some FL Studio thing, not a plugin.
            && !path.parent_path().u8string().ends_with(u8"New") // ignore the "New" dir, duplicates other dirs
        ) {
            // Read file as map of values.
            NfoData nfoData { readNfoFile(path) };

            // Extract the plugin details.
            const std::string vendor { nfoData["ps_file_vendorname_0"] };

            if (!vendor.empty()) {
                const auto rawPluginType { nfoData["ps_file_type_0"] };
                const int pluginType { std::stoi(nfoData["ps_file_type_0"]) };
                if (pluginType < PluginType::Effect || pluginType > PluginType::Generator) {
                    nowide::cerr << "Unrecognised plugin type: " << rawPluginType;
                    throw UnexpectedValueError();
                }

                PluginData pluginData {
                    nfoData["ps_name"],
                    entry.path(),
                    static_cast<PluginType>(pluginType)
                };
#if FLSPO_VERBOSE
                nowide::cout << pluginData << std::endl;
#endif
                pluginMap.emplace(vendor, pluginData);
            }
        }
    }
}

PluginByVendorMap walkDirectories(const std::filesystem::path &pluginDbPath) {
    PluginByVendorMap pluginMap;
    walkDirectory(pluginDbPath, pluginMap);
//    for (const auto &entry : pluginMap) {
//        std::cout << entry.first << ": " << entry.second << std::endl;
//    }
    return pluginMap;
}
