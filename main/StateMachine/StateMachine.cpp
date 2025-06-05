#include "StateMachine.h"

StateMachine::StateMachine() {}

void StateMachine::addState(IAppState* state) { m_states.push_back(state); }

void StateMachine::setCurrentState(IAppState* state) {
  if (m_currentState) {
    m_currentState->exit();
  }
  m_currentState = state;
  if (m_currentState) {
    m_currentState->enter();
  }
}

void StateMachine::start() { m_running = true; }

void StateMachine::stop() { m_running = false; }

bool StateMachine::running() const { return m_running; }

void StateMachine::update() {
  if (m_running && m_currentState) {
    m_currentState->update();
  }
}