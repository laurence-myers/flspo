#include <nowide/convert.hpp>
#include <nowide/iostream.hpp>

#include "write.h"

void writeToPluginDatabase(const std::filesystem::path &pluginDbPath, const PluginByVendorMap &pluginByVendorMap) {
    const std::filesystem::path effectsOutputPath = pluginDbPath / "Effects" / "By Vendor";
    const std::filesystem::path generatorsOutputPath = pluginDbPath / "Generators" / "By Vendor";

    if (std::filesystem::exists(effectsOutputPath)) {
        nowide::cout << "Removing existing effects directory: " << nowide::narrow(effectsOutputPath.wstring()) << std::endl;
        remove_all(effectsOutputPath);
    }

    if (std::filesystem::exists(generatorsOutputPath)) {
        nowide::cout << "Removing existing generators directory: " << nowide::narrow(generatorsOutputPath.wstring()) << std::endl;
        remove_all(generatorsOutputPath);
    }

    for (const auto &entry : pluginByVendorMap) {
        nowide::cout << "Writing: " << entry.first << " - " << entry.second.name << std::endl;
        const std::filesystem::path vendorOutputPath = (entry.second.pluginType == PluginType::Effect ? effectsOutputPath : generatorsOutputPath) / entry.first;
        create_directories(vendorOutputPath);

        // Copy the .nfo
        copy(entry.second.path, vendorOutputPath / entry.second.path.filename(), std::filesystem::copy_options::update_existing);

        // Copy the .fst
        const std::filesystem::path fstPath = entry.second.path.parent_path() / entry.second.path.filename().replace_extension(".fst");
        copy(fstPath, vendorOutputPath / fstPath.filename(), std::filesystem::copy_options::update_existing);
    }
}
