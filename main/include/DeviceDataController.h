#pragma once

#include <List.hpp>
#include <map>
#include <string>

#include "DeviceIDs.h"
#include "cJSON.h"

struct DeviceData {
  long longitude;
  long latitude;
  int hour;
  int minute;
  int second;

  static DeviceData deserialize(const char* jsonStr) {
    DeviceData result = {};

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

    if (cJSON_IsNumber(lon)) result.longitude = lon->valuedouble;
    if (cJSON_IsNumber(lat)) result.latitude = lat->valuedouble;
    if (cJSON_IsNumber(h)) result.hour = h->valueint;
    if (cJSON_IsNumber(m)) result.minute = m->valueint;
    if (cJSON_IsNumber(s)) result.second = s->valueint;

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

    char* jsonStr =
        cJSON_PrintUnformatted(root);  // or cJSON_Print() for pretty
    cJSON_Delete(root);                // free the object tree

    return jsonStr;  // must be freed by caller
  }

  long toSecond() const { return hour * 3600L + minute * 60L + second; }

  bool isNewer(const DeviceData& other) const {
    long thisSec = toSecond();
    long otherSec = other.toSecond();

    // If this is more than 12 hours "earlier" than other, it probably wrapped
    if (thisSec < otherSec && (otherSec - thisSec) > 12 * 3600) return true;

    // If this is more than 12 hours "later" than other, assume other wrapped
    if (thisSec > otherSec && (thisSec - otherSec) > 12 * 3600) return false;

    return thisSec > otherSec;
  }
};

class DeviceDataController {
 public:
  DeviceDataController();
  void addDeviceData(uint32_t userId, const DeviceData& DeviceData);
  const char* updateMessage() const;
  void updateData(long lon, long lat, int h, int m, int s);
  void setDeviceId(uint32_t id);

 private:
  std::map<uint32_t, DeviceData> m_data;
  std::map<uint32_t, std::string> m_usernames;
  DeviceData m_deviceData;
  uint32_t m_deviceId;
};