#pragma once

class AppManager;

class App {
 public:
  virtual ~App() = default;
  virtual const char* name() const = 0;
  virtual void enter() {}
  virtual void update() {}
  virtual void exit() {}

  void setManager(AppManager* manager) { manager_ = manager; }

 protected:
  AppManager* manager_ = nullptr;
};
