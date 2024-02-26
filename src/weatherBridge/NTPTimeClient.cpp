
#include <ArduinoLog.h>
#include <esp_sntp.h>

#include "weatherBridge/NTPTimeClient.hpp"

#define TIME_SYNC_INTERVAL_MILLIS 1200000
#define NTP_SERVER_1 "pool.ntp.org"

static unsigned long timeLastUpdated = 0;
static long ntpTimeSyncOkFlag = false;

void NTPTimeClient::begin(WeatherBridgeSettings &settings) {
    const String& timezone = settings.getPosixTzString();
    const char *timezoneString = timezone.isEmpty() ? "UTC" : timezone.c_str();

    setInitialTime(timezoneString);

    sntp_set_sync_interval(TIME_SYNC_INTERVAL_MILLIS);
    sntp_set_time_sync_notification_cb([](struct timeval *tv) {
        timeLastUpdated = millis();
        ntpTimeSyncOkFlag = true;
        Log.traceln("NTPTimeClient: sync OK");
    });
    configTzTime(timezoneString, NTP_SERVER_1);
    Log.traceln("NTPTimeClient: sync interval set to %u", sntp_get_sync_interval());
}

bool NTPTimeClient::checkStatus() {
    if (millis() - timeLastUpdated > 2 * sntp_get_sync_interval()) {
        ntpTimeSyncOkFlag = false;
    }
    return ntpTimeSyncOkFlag;
}

void NTPTimeClient::setInitialTime(const char *timezoneString) {
    setenv("TZ", timezoneString, 1);
    tzset();

    struct tm tmstruct{0};
    tmstruct.tm_year = (2022 - 1900);
    tmstruct.tm_mon = (9 - 1);
    tmstruct.tm_mday = 15;
    tmstruct.tm_hour = 12;
    tmstruct.tm_min = 1;
    tmstruct.tm_sec = 0;

    time_t t = mktime(&tmstruct);
    timeval tv = {t, 0};
    settimeofday(&tv, NULL);

}
