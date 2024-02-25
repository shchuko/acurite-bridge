#include "weatherBridge/FSSettingStore.hpp"

#define FS_SETTING_STORE_DIR "/kv-setting-store"
#define FS_SETTING_STORE_KEY(s) FS_SETTING_STORE_DIR "/" s ".txt"

static const String WLAN_SSID = FS_SETTING_STORE_KEY("wlan_ssid");
static const String WLAN_PASSWORD = FS_SETTING_STORE_KEY("wlan_password");
static const String POSIX_TZ_STRING = FS_SETTING_STORE_KEY("posix_tz_string");
static const String PWS_WEATHER_STATION_ID = FS_SETTING_STORE_KEY("pws_weather_station_id");
static const String PWS_WEATHER_API_KEY = FS_SETTING_STORE_KEY("pws_weather_api_key");
static const String WIND_GURU_STATION_UID = FS_SETTING_STORE_KEY("wind_guru_station_uid");
static const String WIND_GURU_STATION_PASSWORD = FS_SETTING_STORE_KEY("wind_guru_station_password");
static const String WINDY_API_KEY = FS_SETTING_STORE_KEY("windy_api_key");
static const String WINDY_STATION_ID = FS_SETTING_STORE_KEY("windy_station_id");
static const String WU_API_KEY = FS_SETTING_STORE_KEY("wu_api_key");
static const String WU_STATION_ID = FS_SETTING_STORE_KEY("wu_station_id");


FSSettingStore::FSSettingStore(FS &fs) noexcept: fs(fs) {}

WeatherBridgeSettings FSSettingStore::loadSettings() {
    if (!fs.exists(FS_SETTING_STORE_DIR)) {
        fs.mkdir(FS_SETTING_STORE_DIR);
    }

    return {
            readFile(WLAN_SSID),
            readFile(WLAN_PASSWORD),
            readFile(POSIX_TZ_STRING),
            readFile(PWS_WEATHER_STATION_ID),
            readFile(PWS_WEATHER_API_KEY),
            readFile(WIND_GURU_STATION_UID),
            readFile(WIND_GURU_STATION_PASSWORD),
            readFile(WINDY_API_KEY),
            readFile(WINDY_STATION_ID),
            readFile(WU_API_KEY),
            readFile(WU_STATION_ID)
    };
}

bool FSSettingStore::writeSettings(WeatherBridgeSettings &settings) {
    bool wlanSsidWriteRes = writeFile(WLAN_SSID, settings.getWlanSsid());
    bool wlanPasswordWriteRes = writeFile(WLAN_PASSWORD, settings.getWlanPassword());
    bool posixTzStringWriteRes = writeFile(POSIX_TZ_STRING, settings.getPosixTzString());
    bool pwsWeatherStationIdWriteRes = writeFile(PWS_WEATHER_STATION_ID, settings.getPwsWeatherStationId());
    bool pwsWeatherApiKeyWriteRes = writeFile(PWS_WEATHER_API_KEY, settings.getPwsWeatherApiKey());
    bool windGuruStationUidWriteRes = writeFile(WIND_GURU_STATION_UID, settings.getWindGuruStationUid());
    bool windGuruStationPasswordWriteRes = writeFile(WIND_GURU_STATION_PASSWORD,
                                                     settings.getWindGuruStationPassword());
    bool windyApiKeyWriteRes = writeFile(WINDY_API_KEY, settings.getWindyApiKey());
    bool windyStationIdWriteRes = writeFile(WINDY_STATION_ID, settings.getWindyStationId());
    bool wuApiKeyWriteRes = writeFile(WU_API_KEY, settings.getWuApiKey());
    bool wuStationIdWriteRes = writeFile(WU_STATION_ID, settings.getWuStationId());

    return wlanSsidWriteRes
           && wlanPasswordWriteRes
           && posixTzStringWriteRes
           && pwsWeatherStationIdWriteRes
           && pwsWeatherApiKeyWriteRes
           && windGuruStationUidWriteRes
           && windGuruStationPasswordWriteRes
           && windyApiKeyWriteRes
           && windyStationIdWriteRes
           && wuApiKeyWriteRes
           && wuStationIdWriteRes;
}

String FSSettingStore::readFile(const String &path) {
    Log.traceln("Reading file %s", path.c_str());

    String output = "";
    File file = fs.open(path);
    if (!file) {
        Log.traceln("File %s not exists", path.c_str());
        return output;
    }

    if (file.isDirectory()) {
        Log.traceln("Found directory instead of file %s", path.c_str());
        file.close();
        return output;
    }


    while (file.available()) {
        output = output + file.readString();
        break;
    }

    file.close();
    return output;
}

bool FSSettingStore::writeFile(const String &path, const String &message) {
    Log.traceln("Writing file %s", path.c_str());
    File file = fs.open(path, FILE_WRITE);
    if (!file) {
        Log.traceln("Failed to open file %s for writing", path.c_str());
        return false;
    }

    if (!file.print(message)) {
        Log.traceln("File %s content write failed", path.c_str());
        file.close();
        return false;
    }

    Log.traceln("File %s content written successfully", path.c_str());
    file.close();
    return true;
}
