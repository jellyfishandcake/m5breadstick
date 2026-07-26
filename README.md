# m5breadstick

A reusable firmware scaffold for an M5StickS3 pocket app launcher with a tarot app.

The device uses a 1.14-inch IPS TFT display with a native 135×240 portrait resolution, so the UI and preview are tuned for that shape.

## Why this stack

I chose Arduino/C++ with M5Unified for this project.

- It is the most common and best-documented path for the ESP32-S3/M5StickS3 family.
- M5Unified simplifies display, button, IMU, and speaker access so the first build can focus on app structure rather than low-level drivers.
- The project later needs to handle many card images and smooth redraws, and C++/M5Unified is the most robust option for that.

## Project layout

- src/main.cpp — bootstraps M5Stack hardware and starts the app manager
- src/app_interface.h — the small app contract used by every screen
- src/app_manager.{h,cpp} — switches apps and manages app lifecycle
- src/menu_app.{h,cpp} — the simple launcher UI
- src/tarot_app.{h,cpp} — a tarot state machine with shake and reading flow
- tools/prepare_cards.py — placeholder asset preparation script for future card art

## Build and flash

1. Install PlatformIO.
2. From this folder, run:
   - pio run
   - pio run --target upload
3. Connect the device over USB and monitor with:
   - pio device monitor

## Web preview

You can also preview the UI in a browser before flashing the device.

1. Open [web/index.html](web/index.html) in a browser.
2. Use the on-screen buttons to test the menu and tarot flow.
3. This is useful for rapid UI iteration while keeping the embedded firmware logic separate.

## How to add a new app

1. Create a new class that inherits from App.
2. Implement name(), enter(), update(), and exit().
3. Register the app in main.cpp with appManager.registerApp(...).
4. Let the menu app launch it by calling manager_->setActiveApp(index).

## Asset preparation

The repo includes a starter script at tools/prepare_cards.py that copies a folder of source images into a target folder and writes a manifest. For the real build, this script can later be expanded into a batch pipeline that resizes and converts all 78 Rider-Waite-Smith cards into a display-friendly format for LittleFS.

## What this teaches

- App lifecycle: enter/update/exit is a useful pattern for embedded systems where RAM is tight.
- State machines: the tarot flow is easier to reason about when each screen is a state, not a pile of conditionals.
- PlatformIO: a practical build tool for embedded projects that keeps dependencies and firmware builds organized.
- Tarot basics: tarot is usually treated as a reflective, symbolic tool rather than a factual prediction system. That makes it a great fit for a simple, calm, placeholder-driven app.
- Embedded UX: even simple devices need a clear flow for screen transitions, back actions, and power-saving behavior.
