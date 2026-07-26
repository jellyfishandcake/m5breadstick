#include "menu_app.h"
#include <M5Unified.h>
#include "app_manager.h"

void MenuApp::enter() {
  M5.Display.clear(TFT_BLACK);
  M5.Display.setTextColor(TFT_WHITE);
  M5.Display.setTextSize(1);

  constexpr int16_t kScreenWidth = 135;
  constexpr int16_t kScreenHeight = 240;
  constexpr int16_t kMargin = 10;

  M5.Display.setCursor(kMargin, 8);
  M5.Display.println("Breadstick");
  M5.Display.setCursor(kMargin, 24);
  M5.Display.println("Pocket Apps");

  M5.Display.fillRoundRect(kMargin, 44, kScreenWidth - (kMargin * 2), 44, 8, TFT_DARKCYAN);
  M5.Display.setTextColor(TFT_WHITE);
  M5.Display.setCursor(18, 58);
  M5.Display.println(selectedIndex_ == 0 ? "Tarot" : "Soon");

  M5.Display.setTextColor(TFT_LIGHTGREY);
  M5.Display.setCursor(kMargin, 106);
  M5.Display.println("A tiny launcher");
  M5.Display.setCursor(kMargin, 122);
  M5.Display.println("for a pocket screen");
}

void MenuApp::update() {
  if (M5.BtnA.wasPressed()) {
    selectedIndex_ = (selectedIndex_ + 1) % 2;
  }

  if (M5.BtnB.wasPressed()) {
    if (selectedIndex_ == 0) {
      manager_->setActiveApp(1);
    }
  }

  if (M5.BtnA.wasPressed() && M5.BtnB.wasPressed()) {
    manager_->goBack();
  }

  constexpr int16_t kScreenWidth = 135;
  constexpr int16_t kScreenHeight = 240;
  constexpr int16_t kMargin = 10;

  M5.Display.fillRect(0, 40, kScreenWidth, kScreenHeight - 40, TFT_BLACK);
  M5.Display.setCursor(kMargin, 42);
  M5.Display.setTextColor(TFT_WHITE);
  M5.Display.println("1. Tarot");
  M5.Display.println("2. Soon");

  M5.Display.setCursor(kMargin, 96);
  M5.Display.setTextColor(TFT_DARKCYAN);
  M5.Display.printf("Focus: %s", selectedIndex_ == 0 ? "Tarot" : "Coming soon");
}

void MenuApp::exit() {
  M5.Display.clear(TFT_BLACK);
}
