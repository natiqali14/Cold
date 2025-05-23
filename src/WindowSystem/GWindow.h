#pragma once
#include "../includes.h"
#include "../includes/GLFW/glfw3.h"
#include <EventSystemHelper.h>

class InputSystem;
class GWindow {
    GLFWwindow* window;
    u16 width;
    u16 height;
    std::string window_name;
    InputSystem* input_system;
public:
    GWindow(u16 w, u16 h, std::string w_name);
    void make_context_current();
    bool should_close_window();
    GLFWwindow* get_window_ptr();
    void key_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void poll_input_events();
    void should_hide_cursor(bool should_hide);
    ~GWindow();
private:
    void mouse_input_callback(GLFWwindow* window, f64 x_pos, f64 y_pos);
};