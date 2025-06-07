// UserMenuScreen.h
#pragma once

#include "ListView.h"
#include "Text.h"
#include "UIScreen.h"
namespace UI {

class UserMenuScreen : public UIScreen {
 public:
  UserMenuScreen();
  ~UserMenuScreen() override = default;

  Window* root() const override;
  std::string createDisplayString(const GPSData& data, uint32_t id,
                                  const char* name) const;

  uint32_t distance(const GPSData& a, const GPSData& b) const;
  std::vector<std::string> setupListView(SimpleMap<uint32_t, GPSData> map);
  void updateListView();

 private:
  std::string formattedDistanceString(uint32_t distance) const;
  std::string formattedTimeString(uint32_t diff) const;
  ListView* listView;
  uint32_t m_selectedId{SystemControllers::instance().mesh.deviceId()};
};

}  // namespace UI