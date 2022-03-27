#ifndef FL_STUDIO_PLUGIN_ORGANIZER_COMMON_H
#define FL_STUDIO_PLUGIN_ORGANIZER_COMMON_H

#include <filesystem>
#include <map>
#include <string>

#include <nowide/convert.hpp>

#define FLSPO_VERBOSE 1

enum PluginType {
    Effect = 1,
    Generator = 2
};

struct PluginData {
    const std::string name;
    const std::filesystem::path path;
    const PluginType pluginType;

    friend std::ostream &operator<<(std::ostream &os, const PluginData &data) {
        os << "name: " << data.name << " path: " << nowide::narrow(data.path.wstring()) << " pluginType: " << data.pluginType;
        return os;
    }
};

typedef std::multimap<std::string, PluginData> PluginByVendorMap;

//typedef std::basic_string<char8_t> Utf8String; // std::u8string isn't recognised by CLion :(

#endif //FL_STUDIO_PLUGIN_ORGANIZER_COMMON_H
