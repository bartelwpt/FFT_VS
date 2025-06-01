#pragma once
#include <State.h>

#include <List.hpp>

class StateMachine {
 public:
  StateMachine();
  void start();
  int nextId();
  bool running();
  void setCurrentState(int stateId);
  void addStates(List<State *> &stateList);

 private:
  void run();

  bool m_running;
  List<State *> *m_states;
  State *m_currentState;
  int m_idProvider;
};