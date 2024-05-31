#pragma once

#include "../includes.h"
#include "../includes/GLFW/glfw3.h"
struct InputData {
    u32 last_key_pressed = 0;
    bool b_is_pressed = false;
};

class InputSystem {
    friend class GWindow;
public:

private:  // functions
    explicit InputSystem();
    void key_input_callback(int key, int scancode, int action, int mods);
    void repeat_input_callbacks();
    ~InputSystem() = default;


private:
    
    std::unordered_set<i16> input_keys_pressed;
    // deleted functions
    InputSystem(const InputSystem&) = delete;
    InputSystem& operator=(const InputSystem&) = delete;
};