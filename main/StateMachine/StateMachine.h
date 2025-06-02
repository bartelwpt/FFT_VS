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
  void addStates(std::initializer_list<State *> states);

 private:
  void run();

  State *menuState, *buttonTestState, *networkTestState, *gpsTestState;

  bool m_running;
  List<State *> *m_states;
  State *m_currentState;
  int m_idProvider;
};