# M5StickS3 — pocket "deck of apps" — build brief

A handoff brief for Claude Code. Read this, then propose a plan before writing code.

## What this is

A handheld toy/tool on an **M5Stack StickC-Plus2 / StickS3** (ESP32-S3). First build is
for the author personally. Later, near-identical firmware will be gifted to family, so
build for reuse from the start — but do **not** over-engineer the gift features now.

The device is a **menu of small apps you scroll and select**. First release ships with
exactly two things:

1. A **menu shell** — the app launcher/framework itself.
2. **Tarot** — shake to shuffle (IMU), draw a card, show art + reading.

Everything else (Heads Up, Werewolf dealer, question decks, ESP-NOW two-player) comes
later as additional apps. Do not build them yet. Do not add apps, stages, or
"helpful" features that aren't in this brief without asking.

## First decision: language — you choose, justify it

Pick **one** and explain the tradeoff briefly before proceeding:

- **Arduino / C++ with M5Unified** — the most common, best-documented path for this
  hardware. M5Unified abstracts the display/IMU/buttons/speaker. Best performance and
  asset handling. More boilerplate.
- **MicroPython via UiFlow2** — faster to iterate, friendlier, but heavier at runtime on
  a device with limited RAM and slower for image blitting.

Weigh it against: this needs to hold ~78 card images and blit them smoothly, and will
later want OTA. Recommend the one that makes those easy. State your choice and why, then
scaffold the project (folder layout, build/flash instructions, dependencies).

## Hardware facts (verify against current M5Stack docs before relying on these)

- **MCU**: ESP32-S3, 8MB flash + PSRAM.
- **Screen**: 1.14" TFT, **240×135**, ST7789. Small — design for it. At a readable font,
  a menu shows ~4 items. No submenus/hierarchy; a flat scrolling list only.
- **Buttons**: **BtnA** (front) and **BtnB** (side). Proposed mapping: BtnA = down/next,
  BtnB = select, long-press BtnB = back. Confirm actual pins from docs.
- **IMU**: 6-axis, used for shake-to-shuffle. Detect a shake gesture (accel magnitude
  over a threshold, debounced), not a single tap.
- **Speaker**: 1W, for optional card-reading audio / SFX. Tinny — keep audio optional.
- **Battery**: ~250mAh. **This is the main constraint.** Dim backlight aggressively,
  sleep the screen on inactivity, wake on button. Assume power-bank use for long sessions.
- **Storage**: use a **LittleFS** partition for card art. 78 cards at 240×135 fit
  comfortably in 8MB if pre-processed (see assets).

## Architecture — build the framework before any app

Define a tiny **App interface** and make the menu itself just another app:

- `enter()` — allocate/load assets (e.g. card art) when the app opens
- `update()` — called each loop tick; handle input, render
- `exit()` — free assets so they aren't resident while another app runs

A single **AppManager** owns the app list, tracks the active app, routes button input,
and handles switching (call `exit()` on the old, `enter()` on the new). The menu shell
is an App that lists the others and launches them on select.

This 15-minute discipline is the whole point — it makes app #3..#N trivial and keeps RAM
in check. Do not special-case tarot into the main loop.

## The two apps

### Menu shell
- Flat vertical list, ~4 visible rows, highlight the selection, show a scroll indicator
  (e.g. "2/6"). BtnA moves the highlight, BtnB launches. Wrap around at the ends.
- Keep it dead simple and fast. No settings screen yet.

### Tarot
- **Deck**: Rider–Waite–Smith. It is **public domain** (published 1909, Pamela Colman
  Smith d. 1951) so the artwork can be bundled freely. Do NOT use a modern deck — those
  are copyrighted. Source clean RWS scans; author will supply or you can note where to get them.
- **Assets**: pre-resize/convert all 78 cards to 240×135 (or the card aspect within it)
  **on the computer**, not on-device. Prefer a format the display can blit directly
  (e.g. RGB565 raw or a light PNG decode). Store on LittleFS. Provide a small script in
  the repo to batch-process a folder of source images into the on-device format.
- **Interaction**: shake to shuffle (IMU gesture) → draw 1 card (later maybe a 3-card
  spread) → display art + name → BtnB shows the reading text → back returns to draw.
- **Readings**: store as data (a JSON/struct table keyed by card), not hardcoded in
  logic. Upright only to start; reversed can come later. Keep card data separate from
  render code.
- **Optional stretch (ask before adding)**: call an LLM API for a contextual reading
  instead of canned text. Nice, but needs Wi-Fi + a key + graceful offline fallback.
  Don't build it into v1 unless asked; leave a clean seam for it.

## Cross-cutting rules

- **Assets processed off-device, always.** Never make the MCU decode a full-size image.
- **Free assets on `exit()`.** Don't hold tarot art in RAM while the menu is up.
- **Vectorise/keep the loop cheap.** Target a responsive UI; avoid blocking delays that
  make buttons feel laggy — use non-blocking timing.
- **Battery-aware from the start**: inactivity → dim → sleep; wake on button.
- **OTA**: not required for this personal build, but structure the project so OTA can be
  added later without rework (keep app code modular, keep a version constant). If it's
  cheap to include an OTA path now, propose it — the gift versions will need it.
- **Config seam**: keep a single place for per-device values (owner name, boot greeting),
  so the later gift builds differ by config, not by forked code.

## Definition of done for v1

- Flashes and boots to the menu on real hardware.
- Menu scrolls and launches tarot; back returns to menu cleanly, no leaked RAM.
- Shake shuffles; a card draws with correct art and name; reading text displays.
- Screen dims/sleeps on inactivity and wakes on a button.
- README covers: chosen language + why, how to process card art, how to build/flash,
  and how to add a new app (the interface contract).

## Explicitly out of scope for v1

Heads Up, Werewolf, question decks, ESP-NOW multiplayer, Wi-Fi provisioning portal,
family status board, voice assistant. Framework should make them easy later; don't
build them now.
