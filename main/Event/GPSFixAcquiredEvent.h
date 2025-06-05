#include "EventDispatcher.h"
#include "GPSData.h"

struct GPSFixAcquiredEvent : public IEvent {
  GPSData data;

  explicit GPSFixAcquiredEvent(const GPSData& d) : data(d) {}

  uint32_t typeId() const override {
    return EventTraits<GPSFixAcquiredEvent>::id();
  }
};