#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <queue>
#include <atomic>

class CObserver;

using wk_observer_ptr = std::weak_ptr<CObserver>;

class CSubject {
private:
  std::vector<CObserver*> views_;
  std::queue<uint32_t> free_indics_;
  std::unordered_map<uint64_t, uint32_t> indics_;
public:
  virtual void AddObserver(CObserver&);
  virtual void RemoveObserver(CObserver&);
  virtual void Notify();
};

class CObserver {
  static std::atomic<uint64_t> cur_id_;
public:
  uint64_t id_;

  CObserver() {
    id_ = ++cur_id_;
  }
  inline uint64_t GetID(){ return id_; }
  virtual void Update(CSubject &) = 0;
};
