#include <filesystem>
#include <fstream>
#include <map>
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
            // split string into key & value
            std::getline(file, value);
            // Store in NfoData
            data[key] = value;
        }
    }
    return data;
}

void walkDirectory(const std::filesystem::path &rootDirectory, PluginByVendorMap &pluginMap) {
    for (const auto &entry: std::filesystem::recursive_directory_iterator(rootDirectory)) {
        if (entry.is_regular_file()
            && entry.path().extension() == ".nfo"
            && entry.path().filename() != "VerifiedIDs.nfo" // Some FL Studio thing, not a plugin.
            && !entry.path().parent_path().string().ends_with("New") // ignore the "New" dir, duplicates other dirs
        ) {
//            std::cout << entry.path() << std::endl;
            // Read file as map of values.
            NfoData nfoData { readNfoFile(entry.path()) };

            // Extract the plugin details.
            const std::string vendor { nfoData["ps_file_vendorname_0"] };

            if (!vendor.empty()) {
                std::string tip { nfoData["tip"] };

                const int pluginType { std::stoi(nfoData["ps_file_type_0"]) };
                if (pluginType < PluginType::Effect || pluginType > PluginType::Generator) {
                    throw UnexpectedValueError();
                }

                PluginData pluginData {
                    nfoData["ps_name"],
                    entry.path(),
                    static_cast<PluginType>(pluginType),
                    tip.replace(tip.find('/'), 1, "-") // make this filename safe
                };
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
