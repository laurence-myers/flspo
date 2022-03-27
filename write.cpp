#include <nowide/iostream.hpp>

#include "write.h"

void writeToPluginDatabase(const std::filesystem::path &pluginDbPath, const PluginByVendorMap &pluginByVendorMap) {
    const std::filesystem::path effectsOutputPath = pluginDbPath / "Effects" / "By Vendor";
    const std::filesystem::path generatorsOutputPath = pluginDbPath / "Generators" / "By Vendor";

    for (const auto &entry : pluginByVendorMap) {
        nowide::cout << entry.first << " - " << entry.second.name << std::endl;
        const std::filesystem::path vendorOutputPath = (entry.second.pluginType == PluginType::Effect ? effectsOutputPath : generatorsOutputPath) / entry.first;
        create_directories(vendorOutputPath);

        // Copy the .nfo
        copy(entry.second.path, vendorOutputPath / entry.second.path.filename(), std::filesystem::copy_options::update_existing);

        // Copy the .fst
        const std::filesystem::path fstPath = entry.second.path.parent_path() / entry.second.path.filename().replace_extension(".fst");
        copy(fstPath, vendorOutputPath / fstPath.filename(), std::filesystem::copy_options::update_existing);
    }
}
