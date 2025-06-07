#include "EventDispatcher.h"
#include "GPSData.h"

struct NetworkTopologyChangedEvent : public IEvent {
  explicit NetworkTopologyChangedEvent() {}

  uint32_t typeId() const override {
    return EventTraits<NetworkTopologyChangedEvent>::id();
  }
};