#include <iostream>
#include <string>
#include <memory>
#include "observer.h"

class AlarmPlatform : public CSubject {
private:
  std::vector<std::string> alarm_msgs_;
public:
  void PushAlarm(std::string msg) {
    alarm_msgs_.push_back(msg);
    Notify();
  }

  std::string GetLastestMsg() {
    return alarm_msgs_.back();
  }
};

class Supervisor : public CObserver {
private:
  int no_;
public:
  Supervisor(int no): no_(no){}
  void Update(CSubject & sub) {
    try {
      AlarmPlatform & alarm = dynamic_cast<AlarmPlatform&>(sub);
      std::cout << "NO: " << no_ << " " << alarm.GetLastestMsg() << std::endl;
    } catch(std::bad_cast) {
      std::cout << "dynamic cast err" << std::endl;
    }
  }
};

int main() {
  AlarmPlatform alarm_platform;
  Supervisor s1(1), s2(2), s3(3);
  alarm_platform.AddObserver(s1);
  alarm_platform.AddObserver(s2);
  alarm_platform.AddObserver(s3);

  alarm_platform.PushAlarm("alarm msg one");
  alarm_platform.RemoveObserver(s1);
  alarm_platform.PushAlarm("alarm msg two");
  alarm_platform.RemoveObserver(s2);
  alarm_platform.PushAlarm("alarm msg three");
}
