#include <State.h>

State::State(int id, char * name, void (*callback)()) : m_id(id), m_name(name), m_callback(callback) {
  m_finished = false;
}

void State::run() {
  m_callback();
}

const char * State::name() {
  return m_name;
}

const int State::id() {
  return m_id;
}

bool State::finished() {
  return m_finished;
}