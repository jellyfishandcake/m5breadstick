#include "tarot_app.h"
#include <M5Unified.h>
#include <LittleFS.h>
#include <math.h>
#include "app_manager.h"
#include "tarot_data.h"

void TarotApp::drawAssetCard(const char* imagePath, const char* title, const char* subtitle) {
  constexpr int16_t kScreenWidth = 135;
  constexpr int16_t kMargin = 10;
  constexpr int16_t kCardWidth = 115;
  constexpr int16_t kCardHeight = 120;
  constexpr int16_t kImageX = 12;
  constexpr int16_t kImageY = 56;

  M5.Display.fillRoundRect(kMargin, 48, kScreenWidth - (kMargin * 2), 150, 10, TFT_DARKCYAN);
  M5.Display.drawRoundRect(kMargin, 48, kScreenWidth - (kMargin * 2), 150, 10, TFT_WHITE);

  if (LittleFS.begin(false)) {
    if (LittleFS.exists(imagePath)) {
      M5.Display.drawPngFile(imagePath, kImageX, kImageY);
    }
    LittleFS.end();
  }

  M5.Display.setTextColor(TFT_WHITE);
  M5.Display.setCursor(12, 54);
  M5.Display.println(title);
  M5.Display.setCursor(12, 70);
  M5.Display.println(subtitle);
}

void TarotApp::enter() {
  state_ = ScreenState::kShuffle;
  hasShuffled_ = false;
  readingVisible_ = false;
  M5.Display.clear(TFT_BLACK);
  M5.Display.setTextColor(TFT_WHITE);
  M5.Display.setCursor(8, 8);
  M5.Display.println("Tarot");
  M5.Display.setCursor(8, 24);
  M5.Display.println("A small, calm draw");
}

void TarotApp::update() {
  if (M5.BtnB.wasPressed()) {
    if (state_ == ScreenState::kCard) {
      readingVisible_ = true;
      state_ = ScreenState::kReading;
    } else if (state_ == ScreenState::kReading) {
      state_ = ScreenState::kShuffle;
      readingVisible_ = false;
      hasShuffled_ = false;
    } else {
      state_ = ScreenState::kCard;
      hasShuffled_ = true;
    }
  }

  if (M5.BtnA.wasPressed()) {
    manager_->goBack();
  }

  if (state_ == ScreenState::kShuffle) {
    const auto imuUpdate = M5.Imu.update();
    if (imuUpdate) {
      const auto imuData = M5.Imu.getImuData();
      const float magnitude = sqrtf(imuData.accel.x * imuData.accel.x +
                                    imuData.accel.y * imuData.accel.y +
                                    imuData.accel.z * imuData.accel.z);
      if (magnitude > 1.8f) {
        state_ = ScreenState::kCard;
        hasShuffled_ = true;
        lastShakeMs_ = millis();
      }
    }
  }

  constexpr int16_t kScreenWidth = 135;
  constexpr int16_t kScreenHeight = 240;
  constexpr int16_t kMargin = 10;

  M5.Display.fillRect(0, 44, kScreenWidth, kScreenHeight - 44, TFT_BLACK);

  if (state_ == ScreenState::kCard) {
    drawAssetCard(kPlaceholderReading.imagePath, kPlaceholderReading.name, kPlaceholderReading.title);
    M5.Display.setCursor(14, 208);
    M5.Display.setTextColor(TFT_LIGHTGREY);
    M5.Display.println("BtnB: reveal the reading");
  } else if (state_ == ScreenState::kReading) {
    drawAssetCard(kPlaceholderReading.imagePath, "Reading", kPlaceholderReading.reading);
    M5.Display.setCursor(14, 208);
    M5.Display.setTextColor(TFT_LIGHTGREY);
    M5.Display.println("BtnB: return to shuffle");
  } else {
    M5.Display.fillRoundRect(kMargin, 50, kScreenWidth - (kMargin * 2), 82, 10, TFT_DARKCYAN);
    M5.Display.setCursor(18, 58);
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.println("Shake gently");
    M5.Display.setCursor(18, 78);
    M5.Display.println("to draw a card");
    M5.Display.setCursor(14, 144);
    M5.Display.setTextColor(TFT_LIGHTGREY);
    M5.Display.println("BtnB: use placeholder draw");
  }
}

void TarotApp::exit() {
  M5.Display.clear(TFT_BLACK);
}
