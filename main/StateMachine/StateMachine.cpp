#include "StateMachine.h"

#include "esp_log.h"
static const char* TAG = "StatemMachine";
StateMachine::StateMachine() {}

void StateMachine::addState(IAppState* state) { m_states.push_back(state); }

void StateMachine::setCurrentState(IAppState* state) {
  ESP_LOGI(TAG, "Set current state requested");
  if (m_currentState) {
    ESP_LOGI(TAG, "Exiting running state ...");
    m_currentState->exit();
    stop();
  }
  m_currentState = state;
  if (m_currentState) {
    ESP_LOGI(TAG, "Entering new state");
    m_currentState->enter();
    start();
  }
}

void StateMachine::start() { m_running = true; }

void StateMachine::stop() { m_running = false; }

bool StateMachine::running() const { return m_running; }

void StateMachine::update() {
  if (m_running && m_currentState) {
    ESP_LOGI(TAG, "update");
    m_currentState->update();
  }
}