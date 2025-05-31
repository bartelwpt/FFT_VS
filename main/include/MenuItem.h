#pragma once
class MenuItem {

public:
  MenuItem(char * name, void (*callback)());

  const char * name();
  bool selected() const;
  void select();
  void deselect();
  void submit();

private:
  char * m_name;
  bool m_selected{false};
  void (*m_onEnteredCallback)(void);
};