#pragma once

#include <array>
#include <cstdint>
#include <functional>

// --------------------------------------
// Base Event Interface
// --------------------------------------
struct IEvent {
  virtual ~IEvent() = default;
  virtual uint32_t typeId() const = 0;
};

// --------------------------------------
// Unique Type ID using static address
// --------------------------------------
template <typename T>
struct EventTraits {
  static uint32_t id() {
    static const char unique_per_type = 0;
    return reinterpret_cast<uintptr_t>(&unique_per_type);
  }
};

// --------------------------------------
// Simple Static Event Dispatcher
// --------------------------------------
class EventDispatcher {
 public:
  using CallbackFunc = std::function<void(const IEvent&)>;

  static constexpr int MaxTypes = 16;
  static constexpr int MaxListenersPerType = 8;

  static EventDispatcher& instance() {
    static EventDispatcher instance;
    return instance;
  }

  template <typename EventType>
  void subscribe(CallbackFunc callback) {
    uint32_t id = EventTraits<EventType>::id();
    for (int i = 0; i < MaxTypes; ++i) {
      if (typeIds[i] == 0 || typeIds[i] == id) {
        typeIds[i] = id;
        callbacks[i][counts[i]++] = callback;
        return;
      }
    }
  }

  void dispatch(const IEvent& event) const {
    uint32_t id = event.typeId();
    for (int i = 0; i < MaxTypes; ++i) {
      if (typeIds[i] == id) {
        for (int j = 0; j < counts[i]; ++j) {
          callbacks[i][j](event);
        }
        break;
      }
    }
  }

 private:
  EventDispatcher() {
    typeIds.fill(0);
    counts.fill(0);
  }

  std::array<uint32_t, MaxTypes> typeIds;
  std::array<std::array<CallbackFunc, MaxListenersPerType>, MaxTypes> callbacks;
  std::array<int, MaxTypes> counts;
};