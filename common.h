#ifndef FL_STUDIO_PLUGIN_ORGANIZER_COMMON_H
#define FL_STUDIO_PLUGIN_ORGANIZER_COMMON_H

#include <filesystem>
#include <map>
#include <string>

enum PluginType {
    Effect = 1,
    Generator = 2
};

struct PluginData {
    const std::string name;
    const std::filesystem::path path;
    const PluginType pluginType;
    const std::string summary;

    friend std::ostream &operator<<(std::ostream &os, const PluginData &data) {
        os << "name: " << data.name << " path: " << data.path << " pluginType: " << data.pluginType << " summary: "
           << data.summary;
        return os;
    }
};

typedef std::multimap<std::string, PluginData> PluginByVendorMap;

#endif //FL_STUDIO_PLUGIN_ORGANIZER_COMMON_H
