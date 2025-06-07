// RendererTestView.cpp
#include "UserMenuScreen.h"

#include "DatabaseUpdateEvent.h"
#include "NetworkTopologyChangedEvent.h"
#include "SystemControllers.h"
#include "SystemIDs.h"

static const char* TAG = "UserMenuScreen";
static constexpr int dayInSec = 86400;
using namespace UI;

UserMenuScreen::UserMenuScreen() {
  rootNode = new Window(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  listView = new ListView(1, 1, SCREEN_WIDTH - 2, SCREEN_HEIGHT - 2);
  auto data = SystemControllers::instance().db().getAllData();
  std::vector<std::string> listViewEntries = setupListView(data);
  listView->setItems(listViewEntries);
  rootNode->addChild(listView);

  EventDispatcher::instance().subscribe<NetworkTopologyChangedEvent>(
      [this](const IEvent& e) { this->updateListView(); });

  EventDispatcher::instance().subscribe<DatabaseUpdateEvent>(
      [this](const IEvent& e) { this->updateListView(); });
}

void UserMenuScreen::updateListView() {
  auto data = SystemControllers::instance().db().getAllData();

  std::vector<std::string> lvEntries = setupListView(data);

  listView->setItems(lvEntries);
  listView->markDirty();
}

std::vector<std::string> UserMenuScreen::setupListView(
    SimpleMap<uint32_t, GPSData> gpsMap) {
  std::vector<std::string> result;
  for (const auto& [key, value] : gpsMap) {
    auto nameOpt = deviceIdMap.find(key);
    if (!nameOpt.has_value()) {
      continue;
    }
    std::string displayString =
        createDisplayString(value, key, nameOpt.value().c_str());
    result.push_back(displayString);
  }

  // ESP_LOGI(TAG, "ListSize: %d", result.size());
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
    const char selected = m_selectedId == id ? '>' : ' ';
    auto dist = distance(data, ownDataValue);
    auto distString = formattedDistanceString(dist);

    std::string name = deviceIdMap.find(id).value();
    const char inNetwork =
        SystemControllers::instance().mesh.isInNetwork(id) ? '+' : '-';
    char result[64];

    auto timeDiff = ownDataValue.timeDifference(data);
    auto timeString = formattedTimeString(timeDiff);

    sprintf(result, "%c %3s|(%c)|%4sm|%s", selected, name.c_str(), inNetwork,
            distString.c_str(), timeString.c_str());
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

std::string UserMenuScreen::formattedDistanceString(uint32_t distance) const {
  std::string distString = std::to_string(distance);
  unsigned int zeros = 4 - distString.length();
  distString.insert(0, zeros, '0');
  return distString;
}

std::string UserMenuScreen::formattedTimeString(
    uint32_t timeDiff /* in seconds */) const {
  const char timeChar = timeDiff > dayInSec ? 'd'
                        : timeDiff > 3600   ? 'h'
                        : timeDiff > 60     ? 'm'
                                            : 's';

  long timeToPrint = timeChar == 'd'   ? timeDiff / dayInSec
                     : timeChar == 'h' ? timeDiff / 3600
                     : timeChar == 'm' ? timeDiff / 60
                                       : timeDiff;

  std::string timeDiffString = std::to_string(timeToPrint);
  unsigned int zeros = 2 - timeDiffString.length();
  timeDiffString.insert(0, zeros, '0');
  return timeDiffString + timeChar;
}
