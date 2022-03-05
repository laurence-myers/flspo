#ifndef FL_STUDIO_PLUGIN_ORGANIZER_READ_H
#define FL_STUDIO_PLUGIN_ORGANIZER_READ_H

#include <filesystem>
#include "common.h"

PluginByVendorMap walkDirectories(const std::filesystem::path& pluginDbPath);

#endif //FL_STUDIO_PLUGIN_ORGANIZER_READ_H
