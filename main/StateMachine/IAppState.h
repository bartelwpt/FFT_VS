#pragma once
class IAppState {
 public:
  virtual void enter() = 0;   // Called when the state starts
  virtual void update() = 0;  // Called every loop cycle
  virtual void exit() = 0;    // Optional, when leaving the state
  virtual ~IAppState() = default;
};