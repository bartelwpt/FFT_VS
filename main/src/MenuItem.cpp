  #include <MenuItem.h>
  
  MenuItem::MenuItem(char * name, void (*callback)()) : m_name(name), m_onEnteredCallback(callback)
  {}

  const char * MenuItem::name() {
    return m_name;
  }

  bool MenuItem::selected() const
  {
    return m_selected;
  }

  void MenuItem::select() 
  {
    m_selected = true;
  }

  void MenuItem::deselect() 
  {
    m_selected = false;
  }

  void MenuItem::submit() 
  {
    m_onEnteredCallback();
  }

