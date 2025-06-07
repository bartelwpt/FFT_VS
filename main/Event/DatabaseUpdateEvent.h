#include "EventDispatcher.h"
#include "GPSData.h"

struct DatabaseUpdateEvent : public IEvent {
  explicit DatabaseUpdateEvent() {}

  uint32_t typeId() const override {
    return EventTraits<DatabaseUpdateEvent>::id();
  }
};