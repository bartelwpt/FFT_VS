#include <optional>  // C++17
#include <utility>
#include <vector>

template <typename Key, typename Value>
class SimpleMap {
 public:
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
      if (kv.first == key) {
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

 private:
  std::vector<std::pair<Key, Value>> data_;
};