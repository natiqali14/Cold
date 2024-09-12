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

private: 
    class GWindow* current_window;
// functions
    explicit InputSystem();
    void key_input_callback(int key, int scancode, int action, int mods);
    void repeat_input_callbacks();
    void mouse_input_callback(f64 x_pos, f64 y_pos);
    void set_current_window(GWindow* window);
    // this function is to pocess inputs  on release directly rather then passing them to event system
    // these should be event that are very urgent. Like showing mouse cursor on escape
    void direct_input_release_process(i32 key);
    ~InputSystem() = default;


private:
    
    std::unordered_set<i16> input_keys_pressed;
    // deleted functions
    InputSystem(const InputSystem&) = delete;
    InputSystem& operator=(const InputSystem&) = delete;
};