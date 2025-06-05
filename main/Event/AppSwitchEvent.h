#include "EventDispatcher.h"
#include "SystemIDs.h"

struct AppSwitchEvent : public IEvent {
  AppID targetApp;

  explicit AppSwitchEvent(AppID app) : targetApp(app) {}

  uint32_t typeId() const override { return EventTraits<AppSwitchEvent>::id(); }
};