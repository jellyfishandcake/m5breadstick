#include <Arduino.h>
#include <M5Unified.h>
#include "app_manager.h"
#include "config.h"
#include "menu_app.h"
#include "tarot_app.h"

AppManager appManager;
MenuApp menuApp;
TarotApp tarotApp;

void setup() {
  auto cfg = M5.config();
  cfg.internal_imu = true;
  cfg.external_spk = true;
  M5.begin(cfg);

  M5.Display.setRotation(1);
  M5.Display.setTextWrap(false);
  M5.Display.setBrightness(255);
  M5.Display.setTextDatum(MC_DATUM);

  M5.Display.println(config::kBootGreeting);

  appManager.registerApp(&menuApp);
  appManager.registerApp(&tarotApp);
  appManager.setActiveApp(0);
}

void loop() {
  M5.update();
  appManager.update();
}
