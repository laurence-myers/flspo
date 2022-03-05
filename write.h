#ifndef FL_STUDIO_PLUGIN_ORGANIZER_WRITE_H
#define FL_STUDIO_PLUGIN_ORGANIZER_WRITE_H

#include "common.h"

void writeToPluginDatabase(const std::filesystem::path& pluginDbPath, const PluginByVendorMap& pluginByVendorMap);

#endif //FL_STUDIO_PLUGIN_ORGANIZER_WRITE_H
