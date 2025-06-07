// RendererTestView.cpp
#include "UserMenuScreen.h"

#include "DatabaseUpdateEvent.h"
#include "NetworkTopologyChangedEvent.h"
#include "SystemControllers.h"
#include "SystemIDs.h"

static const char* TAG = "UserMenuScreen";

using namespace UI;

UserMenuScreen::UserMenuScreen() {
  rootNode = new Window(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  listView = new ListView(1, 1, SCREEN_WIDTH - 2, SCREEN_HEIGHT - 2);
  auto data = SystemControllers::instance().db().getAllData();
  std::vector<std::string> listViewEntries = setupListView(data);
  listView->setItems(listViewEntries);
  rootNode->addChild(listView);

  EventDispatcher::instance().subscribe<NetworkTopologyChangedEvent>(
      [this](const IEvent& e) { this->updateListView(e); });

  EventDispatcher::instance().subscribe<DatabaseUpdateEvent>(
      [this](const IEvent& e) { this->updateListView(e); });
}

void UserMenuScreen::updateListView(const IEvent& e) {
  ESP_LOGI(TAG, "Received NetworkTopologyChangedEvent");
  auto data = SystemControllers::instance().db().getAllData();
  std::vector<std::string> lvEntries = setupListView(data);
  listView->setItems(lvEntries);
}

std::vector<std::string> UserMenuScreen::setupListView(
    SimpleMap<uint32_t, GPSData> gpsMap) {
  std::vector<std::string> result;
  for (const auto& [key, value] : gpsMap) {
    std::string displayString =
        createDisplayString(value, key, deviceIdMap.find(key).value());
    result.push_back(displayString);
  }
  return result;
}

Window* UserMenuScreen::root() const { return rootNode; }

std::string UserMenuScreen::createDisplayString(const GPSData& data,
                                                uint32_t id,
                                                const char* name) const {
  GPSData ownDataValue;
  if (SystemControllers::instance().db().getDeviceData(
          SystemControllers::instance().mesh.deviceId(), &ownDataValue)) {
    // use ownDataValue safely here
    std::string selected = m_selectedId == id ? ">" : " ";
    auto distanceInMeter = distance(data, ownDataValue);
    std::string name = deviceIdMap.find(id).value();
    std::string inNetwork =
        SystemControllers::instance().mesh.isInNetwork(id) ? "+" : "-";
    char result[32];
    sprintf(result, "%s%s %s %4" PRIu32 "m %1dd%2dh%2dm", selected.c_str(),
            name.c_str(), inNetwork.c_str(), distanceInMeter, data.hour,
            data.minute, data.second);
    return result;
  } else {
    return "";
  }
}

inline double degToRad(double deg) { return deg * M_PI / 180.0; }

uint32_t UserMenuScreen::distance(const GPSData& a, const GPSData& b) const {
  // Convert lat/lon from degrees to radians
  double lat1 = degToRad(a.latitude);
  double lon1 = degToRad(a.longitude);
  double lat2 = degToRad(b.latitude);
  double lon2 = degToRad(b.longitude);

  double deltaLat = lat2 - lat1;
  double deltaLon = lon2 - lon1;

  double sinLat = sin(deltaLat / 2);
  double sinLon = sin(deltaLon / 2);

  double h = sinLat * sinLat + cos(lat1) * cos(lat2) * sinLon * sinLon;

  double c = 2 * atan2(sqrt(h), sqrt(1 - h));
  constexpr double earthRadiusMeters = 6371000.0;

  double dist = earthRadiusMeters * c;
  return static_cast<uint32_t>(dist);
}
