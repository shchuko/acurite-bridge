
#include <ArduinoLog.h>
#include <esp_sntp.h>

#include "weatherBridge/NTPTimeClient.hpp"

#define ONE_MINUTE_IN_MILLIS (60 * 1000)
#define TIME_SYNC_INTERVAL_MILLIS (30 * ONE_MINUTE_IN_MILLIS)
#define NTP_SERVER_1 "pool.ntp.org"
#define NTP_SERVER_2 "time.google.com"
#define NTP_SERVER_3 "time.cloudflare.com"

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
    configTzTime(timezoneString, NTP_SERVER_1, NTP_SERVER_2, NTP_SERVER_3);
    Log.traceln("NTPTimeClient: sync interval set to %u", sntp_get_sync_interval());
}

bool NTPTimeClient::checkStatus() {
    if (millis() - timeLastUpdated > sntp_get_sync_interval() + ONE_MINUTE_IN_MILLIS) {
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
    settimeofday(&tv, nullptr);

}
