#ifndef ENGINE_HPP
#define ENGINE_HPP

class Engine {
public:
  bool is_in_game(void) {
    void** vtable = *(void***)this;

    bool (*is_in_game_fn)(void*) = (bool (*)(void*))vtable[33];

    return is_in_game_fn(this);
  }
  
};

inline static Engine* engine;

#endif
