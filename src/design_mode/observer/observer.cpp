#include "observer.h"

std::atomic<uint64_t> CObserver::cur_id_{0};

void CSubject::AddObserver(CObserver & ob) {
  uint64_t observer_id = ob.GetID();
  if(free_indics_.size() > 0) {
    uint32_t index = free_indics_.front();
    free_indics_.pop();
    indics_[observer_id] = index;
    views_[index] = &ob;
  } else {
    indics_[observer_id] = views_.size();
    views_.push_back(&ob);
  }
}

void CSubject::RemoveObserver(CObserver & ob) {
  uint64_t observer_id = ob.GetID();
  if(!indics_.count(observer_id)) {
    return;
  }
  int index = indics_[observer_id];
  indics_.erase(observer_id);
  views_[index] = nullptr;
  free_indics_.push(index);
}

void CSubject::Notify() {
  for(auto & ob : views_) {
    if(ob) {
      ob->Update(*this);
    }
  }
}
