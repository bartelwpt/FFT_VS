#pragma once
#include "cJSON.h"
#include "esp_log.h"
struct GPSData {
  double longitude{0.0};
  double latitude{0.0};
  int hour{0};
  int minute{0};
  int second{0};
  int day{1};
  int month{1};
  int year{1970};
  bool fix{false};

  GPSData() {}

  GPSData(double lng, double lat, int h, int m, int s, int dd, int mm, int yyyy,
          bool f)
      : longitude(lng),
        latitude(lat),
        hour(h),
        minute(m),
        second(s),
        day(dd),
        month(mm),
        year(yyyy),
        fix(f) {}

  static GPSData deserialize(const char* jsonStr) {
    GPSData result = {};

    cJSON* root = cJSON_Parse(jsonStr);
    if (!root) {
      // Invalid JSON
      return result;
    }

    cJSON* lon = cJSON_GetObjectItemCaseSensitive(root, "longitude");
    cJSON* lat = cJSON_GetObjectItemCaseSensitive(root, "latitude");
    cJSON* h = cJSON_GetObjectItemCaseSensitive(root, "hour");
    cJSON* m = cJSON_GetObjectItemCaseSensitive(root, "minute");
    cJSON* s = cJSON_GetObjectItemCaseSensitive(root, "second");
    cJSON* fix = cJSON_GetObjectItemCaseSensitive(root, "fix");
    cJSON* dd = cJSON_GetObjectItemCaseSensitive(root, "day");
    cJSON* mm = cJSON_GetObjectItemCaseSensitive(root, "month");
    cJSON* yyyy = cJSON_GetObjectItemCaseSensitive(root, "year");

    if (cJSON_IsNumber(lon)) result.longitude = lon->valuedouble;
    if (cJSON_IsNumber(lat)) result.latitude = lat->valuedouble;
    if (cJSON_IsNumber(h)) result.hour = h->valueint;
    if (cJSON_IsNumber(m)) result.minute = m->valueint;
    if (cJSON_IsNumber(s)) result.second = s->valueint;
    if (cJSON_IsBool(fix)) result.fix = cJSON_IsTrue(fix);
    if (cJSON_IsNumber(dd)) result.day = dd->valueint;
    if (cJSON_IsNumber(mm)) result.month = mm->valueint;
    if (cJSON_IsNumber(yyyy)) result.year = yyyy->valueint;

    cJSON_Delete(root);
    return result;
  }

  char* serialize() const {
    cJSON* root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "longitude", longitude);
    cJSON_AddNumberToObject(root, "latitude", latitude);
    cJSON_AddNumberToObject(root, "hour", hour);
    cJSON_AddNumberToObject(root, "minute", minute);
    cJSON_AddNumberToObject(root, "second", second);
    cJSON_AddBoolToObject(root, "fix", fix);
    cJSON_AddNumberToObject(root, "day", day);
    cJSON_AddNumberToObject(root, "month", month);
    cJSON_AddNumberToObject(root, "year", year);
    char* jsonStr =
        cJSON_PrintUnformatted(root);  // or cJSON_Print() for pretty
    cJSON_Delete(root);                // free the object tree

    // ESP_LOGI("GPSData", "Serialized data before sending: %s", jsonStr);

    return jsonStr;  // must be freed by caller
  }

  long toSecond() const { return hour * 3600L + minute * 60L + second; }

  bool isNewer(const GPSData& other) const {
    if (year != other.year) return year > other.year;
    if (month != other.month) return month > other.month;
    if (day != other.day) return day > other.day;

    long thisSec = toSecond();
    long otherSec = other.toSecond();

    return thisSec > otherSec;
  }

  long timeDifference(const GPSData& other) const {
    // Convert both dates + times into total seconds from a reference point
    // We'll use a simple "days since epoch" * 86400 + seconds in day approach

    auto daysFromEpoch = [](int y, int m, int d) -> long {
      // Count days since 1970-01-01 (Unix epoch)
      // Simplified calculation ignoring leap seconds, but includes leap years

      // Number of days in each month (non-leap year)
      static const int daysInMonth[] = {31, 28, 31, 30, 31, 30,
                                        31, 31, 30, 31, 30, 31};

      long days = 0;

      // Years
      for (int year = 1970; year < y; ++year) {
        days += 365;
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
          days += 1;  // leap year
        }
      }

      // Months
      for (int month = 1; month < m; ++month) {
        days += daysInMonth[month - 1];
        // Add leap day if february of leap year
        if (month == 2 && ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0))) {
          days += 1;
        }
      }

      // Days
      days += d - 1;  // subtract 1 because day 1 means 0 days offset

      return days;
    };

    long thisTotalSeconds =
        daysFromEpoch(year, month, day) * 86400L + toSecond();
    long otherTotalSeconds =
        daysFromEpoch(other.year, other.month, other.day) * 86400L +
        other.toSecond();

    return thisTotalSeconds - otherTotalSeconds;
  }
};