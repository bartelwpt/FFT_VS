#pragma once
#include <List.hpp>
#include <vector>

#include "IAppState.h"

class StateMachine {
 public:
  StateMachine();

  void addState(IAppState *state);
  void setCurrentState(IAppState *state);
  void start();
  void stop();
  void update();

  bool running() const;

 private:
  bool m_running{false};
  std::vector<IAppState *> m_states;
  IAppState *m_currentState{nullptr};
};