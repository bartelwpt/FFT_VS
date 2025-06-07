#pragma once
#include <optional>  // C++17
#include <utility>
#include <vector>

#include "esp_log.h"

template <typename Key, typename Value>
class SimpleMap {
 public:
  SimpleMap() = default;

  // Constructor to initialize from list
  SimpleMap(std::initializer_list<std::pair<const Key, Value>> init) {
    for (const auto& kv : init) {
      insert(kv.first, kv.second);
    }
  }

  // Insert or update key-value pair
  void insert(const Key& key, const Value& value) {
    for (auto& kv : data_) {
      if (kv.first == key) {
        kv.second = value;
        return;
      }
    }
    data_.emplace_back(key, value);
  }

  // Find value by key; returns std::optional<Value>
  std::optional<Value> find(const Key& key) const {
    for (const auto& kv : data_) {
      if (static_cast<uint32_t>(kv.first) == static_cast<uint32_t>(key)) {
        return kv.second;
      }
    }
    return std::nullopt;
  }

  // Remove key if exists; returns true if removed
  bool erase(const Key& key) {
    for (auto it = data_.begin(); it != data_.end(); ++it) {
      if (it->first == key) {
        data_.erase(it);
        return true;
      }
    }
    return false;
  }

  bool contains(const Key& key) const { return find(key).has_value(); }

  size_t size() const { return data_.size(); }

  void clear() { data_.clear(); }

  auto begin() { return data_.begin(); }
  auto end() { return data_.end(); }

  auto begin() const { return data_.begin(); }
  auto end() const { return data_.end(); }

  auto cbegin() const { return data_.cbegin(); }
  auto cend() const { return data_.cend(); }

 private:
  std::vector<std::pair<Key, Value>> data_;
};