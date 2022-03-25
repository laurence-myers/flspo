#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>

#include "read.h"
#include "common.h"
#include "error.h"

typedef std::map<std::string, std::string> NfoData;

NfoData readNfoFile(const std::filesystem::path &filePath) {
    std::ifstream file { filePath };
    NfoData data;
    if (file.is_open()) {
        std::string key;
        std::string value;
        while (std::getline(file, key, '=')) {
#if _DEBUG
            std::cout << "Key: " << key << std::endl;
#endif
            // split string into key & value
            std::getline(file, value);
#if _DEBUG
            std::cout << "Value: " << value << std::endl;
#endif
            // Store in NfoData
            data[key] = value;
        }
    }
    return data;
}

void walkDirectory(const std::filesystem::path &rootDirectory, PluginByVendorMap &pluginMap) {
    for (const auto &entry: std::filesystem::recursive_directory_iterator(rootDirectory)) {
#if _DEBUG
        std::cout << entry.path() << std::endl;
#endif
        if (entry.is_regular_file()
            && entry.path().extension() == ".nfo"
            && entry.path().filename() != "VerifiedIDs.nfo" // Some FL Studio thing, not a plugin.
            && !entry.path().parent_path().string().ends_with("New") // ignore the "New" dir, duplicates other dirs
        ) {
            // Read file as map of values.
            NfoData nfoData { readNfoFile(entry.path()) };

            // Extract the plugin details.
            const std::string vendor { nfoData["ps_file_vendorname_0"] };

            if (!vendor.empty()) {
                const int pluginType { std::stoi(nfoData["ps_file_type_0"]) };
                if (pluginType < PluginType::Effect || pluginType > PluginType::Generator) {
                    throw UnexpectedValueError();
                }

                PluginData pluginData {
                    nfoData["ps_name"],
                    entry.path(),
                    static_cast<PluginType>(pluginType)
                };
#if _DEBUG
                std::cout << pluginData << std::endl;
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
