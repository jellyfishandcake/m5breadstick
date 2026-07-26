#include "app_manager.h"
#include <M5Unified.h>

void AppManager::registerApp(App* app) {
  if (appCount_ < kMaxApps) {
    apps_[appCount_++] = app;
    app->setManager(this);
  }
}

void AppManager::setActiveApp(size_t index) {
  if (index < appCount_) {
    switchTo(index);
  }
}

void AppManager::update() {
  handlePowerState();
  if (currentApp_ != nullptr) {
    currentApp_->update();
  }
}

void AppManager::goBack() {
  if (currentApp_ != nullptr && activeIndex_ != 0) {
    switchTo(0);
  }
}

void AppManager::switchTo(size_t index) {
  if (currentApp_ != nullptr) {
    currentApp_->exit();
  }

  activeIndex_ = index;
  currentApp_ = apps_[index];
  if (currentApp_ != nullptr) {
    currentApp_->enter();
  }
}

void AppManager::handlePowerState() {
  if (M5.BtnA.wasPressed() || M5.BtnB.wasPressed()) {
    sleeping_ = false;
    M5.Display.setBrightness(255);
    lastInputMs_ = millis();
    return;
  }

  if (sleeping_) {
    return;
  }

  if (millis() - lastInputMs_ > 15000) {
    sleeping_ = true;
    M5.Display.setBrightness(30);
  }
}
