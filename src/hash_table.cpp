#include "hash_table.hpp"

#include <stdexcept>

namespace itis {

  int HashTable::hash(int key) const {
    return utils::hash(key, static_cast<int>(buckets_.size()));
  }

  HashTable::HashTable(int capacity, double load_factor) : load_factor_{load_factor}{
    if (capacity <= 0) {
      throw std::logic_error("hash table capacity must be greater than zero");
    }

    if (load_factor <= 0.0 || load_factor > 1.0) {
      throw std::logic_error("hash table load factor must be in range [0...1]");
    }
    buckets_.resize(capacity);
    // Tip: allocate hash-table buckets
  }

  std::optional<std::string> HashTable::Search(int key) const {
      int index = hash(key);
      std::list<std::pair<int, std::string>> list_ = buckets_[index];
      for (std::pair<int, std::string> single_pair : list_){
          if (single_pair.first == key) return single_pair.second;
      }
    // Tip: compute hash code (index) and use linear search
    return std::nullopt;
  }

  void HashTable::Put(int key, const std::string &value) {
      int index = hash(key);
      for (auto single_pair: buckets_[index]){
          if (single_pair.first == key){
              single_pair.second = value;
              return;
          }
      }
      buckets_[index].push_back(std::pair(key,value));
      num_keys_++;
    // Tip 1: compute hash code (index) to determine which bucket to use
    // Tip 2: consider the case when the key exists (read the docs in the header file)

    if (static_cast<double>(num_keys_) / buckets_.size() >= load_factor_) {
      auto new_bucket = new std::vector<Bucket>[buckets_.size()+kGrowthCoefficient];
      for (auto bucket:buckets_){
          for (auto single_pair : bucket){
              auto new_index = hash(single_pair.first);
              new_bucket[new_index][0].push_back(single_pair);
          }
      }

      buckets_.clear();
      for (auto bucket: buckets_){
          buckets_.push_back(bucket);
      }
      return;
    }
  }

  std::optional<std::string> HashTable::Remove(int key) {
      int index = hash(key);
      auto bucket = buckets_[index];

      std::string v;
      auto key_ = INT32_MAX;
      for (auto pair : bucket){
          if (pair.first == key){
              v = pair.second;
              key_ = pair.first;
              break;
          }
      }
      if (key_ == INT32_MAX) {
          buckets_[index].remove(std::pair(key_, v));
          num_keys_ --;
          return v;
      }
    // Tip 1: compute hash code (index) to determine which bucket to use
    // TIp 2: find the key-value pair to remove and make a copy of value to return
    return std::nullopt;
  }

  bool HashTable::ContainsKey(int key) const {
    // Note: uses Search(key) which is not initially implemented
    return Search(key).has_value();
  }

  bool HashTable::empty() const {
    return size() == 0;
  }

  int HashTable::size() const {
    return num_keys_;
  }

  int HashTable::capacity() const {
    return static_cast<int>(buckets_.size());
  }

  double HashTable::load_factor() const {
    return load_factor_;
  }

  std::unordered_set<int> HashTable::keys() const {
    std::unordered_set<int> keys(num_keys_);
    for (const auto &bucket : buckets_) {
      for (const auto &[key, _] : bucket) {
        keys.insert(key);
      }
    }
    return keys;
  }

  std::vector<std::string> HashTable::values() const {
    std::vector<std::string> values;
    for (const auto &bucket : buckets_) {
      for (const auto &[_, value] : bucket) {
        values.push_back(value);
      }
    }
    return values;
  }

}  // namespace itis