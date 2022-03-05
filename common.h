#ifndef FL_STUDIO_PLUGIN_ORGANIZER_COMMON_H
#define FL_STUDIO_PLUGIN_ORGANIZER_COMMON_H

#include <filesystem>
#include <map>
#include <string>

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

#endif //FL_STUDIO_PLUGIN_ORGANIZER_COMMON_H
