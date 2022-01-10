#include "Settings.h"
#include <unordered_map>

namespace Rush {

void Settings::SaveSettings() {
}

template <>
void Settings::SetSetting<std::string>(SettingMap &map, const std::string &name, const std::string value) {
    map[name] = value;
}
template <>
void Settings::SetSetting<const char *>(SettingMap &map, const std::string &name, const char *value) {
    map[name] = value;
}

}; // namespace Rush