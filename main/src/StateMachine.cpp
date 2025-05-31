#include <StateMachine.h>

StateMachine::StateMachine() {
  m_idProvider = 0;
  m_states = new List<State*>();
  m_running = false;
}

void StateMachine::start(){
  m_running = true;
  if (m_states && m_states->getSize() > 0 && m_currentState)
    run();
}

int StateMachine::nextId() {
  return m_idProvider++;
}

bool StateMachine::running() {
  return m_running;
}

void StateMachine::setCurrentState(int stateId) {
  for (int i = 0; i < m_states->getSize(); i++)
  {
    State * state = m_states->get(i);
    if (state->id() == stateId)
    {
      m_currentState = state;
    }
  }
}

void StateMachine::addStates(List<State*> &stateList) {
  m_states->addAll(stateList); 
}

void StateMachine::run() {
  while(m_running) {
    m_currentState->run();
    if (m_currentState->finished())
    {
      m_running = false;
      Serial.print("State Machine has completed state ");
      Serial.println(m_currentState->name());
    }    
  }
}