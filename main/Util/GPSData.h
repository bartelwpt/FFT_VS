#pragma once
#include "cJSON.h"
struct GPSData {
  double longitude{0.0};
  double latitude{0.0};
  int hour{0};
  int minute{0};
  int second{0};
  bool fix{false};

  GPSData() {}

  GPSData(double lng, double lat, int h, int m, int s, bool f)
      : longitude(lng), latitude(lat), hour(h), minute(m), second(s), fix(f) {}

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

    if (cJSON_IsNumber(lon)) result.longitude = lon->valuedouble;
    if (cJSON_IsNumber(lat)) result.latitude = lat->valuedouble;
    if (cJSON_IsNumber(h)) result.hour = h->valueint;
    if (cJSON_IsNumber(m)) result.minute = m->valueint;
    if (cJSON_IsNumber(s)) result.second = s->valueint;
    if (cJSON_IsBool(fix)) result.fix = cJSON_IsTrue(fix);

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

    char* jsonStr =
        cJSON_PrintUnformatted(root);  // or cJSON_Print() for pretty
    cJSON_Delete(root);                // free the object tree

    ESP_LOGI("GPSData", "Serialized data before sending: %s", jsonStr);

    return jsonStr;  // must be freed by caller
  }

  long toSecond() const { return hour * 3600L + minute * 60L + second; }

  bool isNewer(const GPSData& other) const {
    long thisSec = toSecond();
    long otherSec = other.toSecond();

    // If this is more than 12 hours "earlier" than other, it probably wrapped
    if (thisSec < otherSec && (otherSec - thisSec) > 12 * 3600) return true;

    // If this is more than 12 hours "later" than other, assume other wrapped
    if (thisSec > otherSec && (thisSec - otherSec) > 12 * 3600) return false;

    return thisSec > otherSec;
  }
};