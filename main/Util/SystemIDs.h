#pragma once
#include <inttypes.h>

#include "SimpleMap.h"
const SimpleMap<uint32_t, const char *> deviceIdMap = {{997474133, "YBA"},
                                                       {530658649, "JNI"}};

enum class AppID {
  MENU = 0,
  GPS,
  BUTTON_TEST,
  NETWORK_TEST,
  RENDER_TEST,
  USER_MENU
};
