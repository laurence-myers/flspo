#ifndef FL_STUDIO_PLUGIN_ORGANIZER_FILES_H
#define FL_STUDIO_PLUGIN_ORGANIZER_FILES_H

#include <map>
#include <string>

struct PluginData {
    const std::string name;
    const std::filesystem::path path;
    const std::string category;
    const std::string type;
};

typedef std::multimap<std::string, PluginData> PluginByVendorMap;

PluginByVendorMap walkDirectories(const std::string& pluginDbPath);

#endif //FL_STUDIO_PLUGIN_ORGANIZER_FILES_H
