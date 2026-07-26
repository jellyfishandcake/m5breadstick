#pragma once

#include <cstdint>

#include "app_interface.h"

class TarotApp : public App {
 public:
  const char* name() const override { return "Tarot"; }
  void enter() override;
  void update() override;
  void exit() override;

 private:
  enum class ScreenState { kShuffle, kCard, kReading } state_ = ScreenState::kShuffle;
  bool hasShuffled_ = false;
  bool readingVisible_ = false;
  uint32_t lastShakeMs_ = 0;
  void drawAssetCard(const char* imagePath, const char* title, const char* subtitle);
};
