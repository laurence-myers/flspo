# FL Studio Plugin Organizer

Reads your installed plugins, and adds new directories in your Plugin Database, sorting the plugins by vendor. 

![FL Studio add channel dropdown](https://www.laurencedougalmyers.net/media/images/flspo_v1.0.0_ss1.png)

![FL Studio add effect dropdown](https://www.laurencedougalmyers.net/media/images/flspo_v1.0.0_ss2.png)

## Installing

Download the latest release from the [releases page](https://github.com/laurence-myers/flspo/releases). Extract the zip somewhere.

## How to use

- In FL Studio, scan and verify all of your plugins. This will update the "Installed" plugins in your Plugin Database.
- From a command prompt, run FLSPO, e.g. `flspo.exe`.
  - On Windows, the FL Studio Plugin Database is assumed to be located at "${UserProfile}\Documents\Image-Line\FL Studio\Presets\Plugin database".
  - If your plugin database is located somewhere else, pass its directory as an argument, e.g. `flspo.exe D:\Audio\FL\PluginDB`

In FL Studio, when you go to add a new Generator or Effect, the dropdown menu will have a new folder named "By Vendor".

If you install a new plugin, add it to FL Studio by doing a "quick scan", then re-run FLSPO. 

_(This only makes sense when you use the "Tree" or "Categories" views. The "Simple" view will not look any different.)_

## Legal Notices

FL Studio Plugin Organizer is distributed under the GPLv3 (or higher) license. See [LICENSE](./LICENSE) and [COPYING](./COPYING) for details.

FL Studio is a [trademark](https://trademarks.justia.com/865/96/fl-86596929.html) of [Image Line nv](https://www.image-line.com/).
