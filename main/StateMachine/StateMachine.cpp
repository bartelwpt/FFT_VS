#include <StateMachine.h>

#include "ApplicationController.h"
#include "DeviceIDs.h"
static const char *TAG = "SM";
StateMachine::StateMachine() {
  m_idProvider = 0;
  m_states = new List<State *>();
  m_running = false;
  menuState = new State(AppStates::MENU_STATE, "Menu",
                        &ApplicationController::menuTest);
  buttonTestState = new State(AppStates::BUTTON_TEST_STATE, "Button Test",
                              &ApplicationController::buttonTest);
  networkTestState = new State(AppStates::NETWORK_TEST_STATE, "Network Test",
                               &ApplicationController::networkTest);
  gpsTestState = new State(AppStates::GPS_TEST_STATE, "GPS Test",
                           &ApplicationController::gpsTest);
  addStates({menuState, buttonTestState, networkTestState, gpsTestState});
}

void StateMachine::start() {
  m_running = true;
  if (m_states && m_states->getSize() > 0 && m_currentState) run();
}

int StateMachine::nextId() { return m_idProvider++; }

bool StateMachine::running() { return m_running; }

void StateMachine::setCurrentState(int stateId) {
  ESP_LOGI(TAG, "Request Setting current state to stateId: %d", stateId);
  for (int i = 0; i < m_states->getSize(); i++) {
    State *state = m_states->get(i);
    if (state->id() == stateId) {
      ESP_LOGI(TAG, "Found state with id %d", stateId);
      m_currentState = state;
    }
  }
}

void StateMachine::addStates(List<State *> &stateList) {
  m_states->addAll(stateList);
}

void StateMachine::addStates(std::initializer_list<State *> states) {
  for (auto s : states) {
    m_states->add(s);
  }
}

void StateMachine::run() {
  while (m_running) {
    if (m_currentState != nullptr) {
      m_currentState->run();
      if (m_currentState->finished()) {
        m_running = false;
        Serial.print("State Machine has completed state ");
        Serial.println(m_currentState->name());
      }
    }
  }
}
