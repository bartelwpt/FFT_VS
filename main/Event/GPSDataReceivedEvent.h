#pragma once
#include "EventDispatcher.h"
#include "GPSData.h"

struct GPSDataReceivedEvent : public IEvent {
  uint32_t deviceId;
  GPSData data;

  explicit GPSDataReceivedEvent(uint32_t id, const GPSData& d)
      : deviceId(id), data(d) {}

  uint32_t typeId() const override {
    return EventTraits<GPSDataReceivedEvent>::id();
  }
};