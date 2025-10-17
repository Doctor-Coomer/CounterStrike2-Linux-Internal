#ifndef CONVAR_HPP
#define CONVAR_HPP

class Convar {
public:
  template <typename T>
  T get_value(void) {
    return *(T*)(this + 0x50);
  }
  
  template <typename T>
  void set_value(T value) {
    *(T*)(this + 0x50) = value;
  }  
};

#endif
