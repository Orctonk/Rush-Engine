#ifndef __SETTINGS_H__
#define __SETTINGS_H__

namespace Rush {

class RUSH_API Settings {
private:
    using SettingMap = std::unordered_map<std::string, std::string>;
    SettingMap m_RushSettings;
    SettingMap m_UserSettings;

    Settings() { }
    ~Settings() { }

    static void SaveSettings();

    template <typename T>
    static void SetSetting(SettingMap &map, const std::string &name, T value);
    template <>
    static void SetSetting<const char *>(SettingMap &map, const std::string &name, const char *value);
    template <>
    static void SetSetting<std::string>(SettingMap &map, const std::string &name, std::string value);

    static Settings &GetInstance() {
        static Settings instance;
        return instance;
    }

public:
    static void Init() { }

    template <typename T>
    static void SetRushSetting(const std::string &name, const T value) { SetSetting(GetInstance().m_RushSettings, name, value); }
    template <typename T>
    static void SetUserSetting(const std::string &name, const T value) { SetSetting(GetInstance().m_UserSettings, name, value); }

    template <typename T>
    static T GetRushSetting(const std::string &name) {
        std::stringstream ss;
        ss << GetInstance().m_RushSettings[name];
        T val;
        ss >> val;
        return val;
    }
    template <typename T>
    static T GetUserSetting(const std::string &name) {
        std::stringstream ss;
        ss << GetInstance().m_UserSettings[name];
        T val;
        ss >> val;
        return val;
    }
};

template <>
void Settings::SetSetting<std::string>(SettingMap &map, const std::string &name, const std::string value) {
    map[name] = value;
}
template <>
void Settings::SetSetting<const char *>(SettingMap &map, const std::string &name, const char *value) {
    map[name] = value;
}
template <typename T>
void Settings::SetSetting(SettingMap &map, const std::string &name, const T value) {
    map[name] = std::to_string(value);
}

}; // namespace Rush

#endif // __SETTINGS_H__
