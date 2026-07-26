#pragma once

#include <array>
#include "app_interface.h"

class AppManager {
 public:
  static constexpr size_t kMaxApps = 8;

  void registerApp(App* app);
  void setActiveApp(size_t index);
  void goBack();
  void update();

  size_t appCount() const { return appCount_; }
  App* currentApp() const { return currentApp_; }

 private:
  std::array<App*, kMaxApps> apps_{{nullptr}};
  size_t appCount_ = 0;
  size_t activeIndex_ = 0;
  App* currentApp_ = nullptr;
  uint32_t lastInputMs_ = 0;
  bool sleeping_ = false;

  void switchTo(size_t index);
  void handlePowerState();
};
