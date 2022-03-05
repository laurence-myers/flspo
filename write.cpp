#include <iostream>
#include "write.h"

void writeToPluginDatabase(const std::filesystem::path &pluginDbPath, const PluginByVendorMap &pluginByVendorMap) {
    const std::filesystem::path rootOutputPath = pluginDbPath / "By Vendor";
    for (const auto &entry : pluginByVendorMap) {
        std::cout << entry.first << " - " << entry.second.name << std::endl;
        const std::filesystem::path vendorOutputPath = rootOutputPath / entry.first;
        create_directories(vendorOutputPath);
        copy(entry.second.path, vendorOutputPath / entry.second.path.filename(), std::filesystem::copy_options::update_existing);
        const std::filesystem::path fstPath = entry.second.path.parent_path() / entry.second.path.filename().replace_extension(".fst");
        copy(fstPath, vendorOutputPath / fstPath.filename(), std::filesystem::copy_options::update_existing);
    }
}
