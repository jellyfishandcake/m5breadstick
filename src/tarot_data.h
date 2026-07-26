#pragma once

struct TarotCardReading {
  const char* name;
  const char* title;
  const char* reading;
  const char* imagePath;
};

static constexpr TarotCardReading kPlaceholderReading = {
    "The Fool",
    "A new beginning",
    "Upright: begin again with curiosity and care.",
    "/cards/major_00_fool.png"
};
