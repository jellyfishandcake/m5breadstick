#pragma once

#include <cstddef>

#include "app_interface.h"

class MenuApp : public App {
 public:
  const char* name() const override { return "Menu"; }
  void enter() override;
  void update() override;
  void exit() override;

 private:
  size_t selectedIndex_ = 0;
  size_t launchIndex_ = 1;
};
