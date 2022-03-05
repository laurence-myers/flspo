#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>

struct PluginData {
    const std::string name;
    const std::filesystem::path path;
    const std::string category;
    const std::string type;

    friend std::ostream &operator<<(std::ostream &os, const PluginData &data) {
        os << "name: " << data.name << " path: " << data.path << " category: " << data.category << " type: "
           << data.type;
        return os;
    }
};

typedef std::multimap<std::string, PluginData> PluginByVendorMap;

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

void walkDirectory(const std::string &rootDirectory, PluginByVendorMap &pluginMap) {
    for (const auto &entry: std::filesystem::recursive_directory_iterator(rootDirectory)) {
        if (entry.is_regular_file()
            && entry.path().extension() == ".nfo"
            && entry.path().filename() != "VerifiedIDs.nfo" // Some FL Studio thing, not a plugin.
            && !entry.path().parent_path().string().ends_with("New") // ignore the "New" dir, duplicates other dirs
        ) {
//            std::cout << entry.path() << std::endl;
            // Read file as map of values.
            NfoData nfoData { readNfoFile(entry.path()) };
            // Check if we've seen this plugin before.
            // If not, store its details (vendor, name, type)
            std::string vendor = nfoData["ps_file_vendorname_0"];
            std::string tip = nfoData["tip"];
            if (!vendor.empty()) {
                PluginData pluginData {
                    nfoData["ps_name"],
                    entry.path(),
                    nfoData["ps_file_category_0"], // Might also want to look at "ps_file_category_1", which sometimes has "Instrument|Synth"
                    tip.replace(tip.find('/'), 1, "-") // make this filename safe
                };
                pluginMap.emplace(vendor, pluginData);
            }
        }
    }
}

PluginByVendorMap walkDirectories(const std::string &pluginDbPath) {
    PluginByVendorMap pluginMap;
    walkDirectory(pluginDbPath, pluginMap);
    for (const auto &entry : pluginMap) {
        std::cout << entry.first << ": " << entry.second << std::endl;
    }
    return pluginMap;
}
