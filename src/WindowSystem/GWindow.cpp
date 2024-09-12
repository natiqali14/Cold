#include "GWindow.h"
#include "WindowSystemUtility.h"
#include "InputSystem.h"

GWindow::GWindow(u16 w, u16 h, std::string w_name)
: width(w)
, height(h)
, window_name(std::move(w_name))
, input_system(new InputSystem)

{
    // window creation
    window = glfwCreateWindow(width, height, window_name.c_str(), nullptr, nullptr);
    // error on failure
    if(window == NULL) throw std::runtime_error("GLFW window creation failed");
    // for now when create a new GLFW window, this window will be current context for opneGL.
    glfwMakeContextCurrent(window); 
    // frame buffer size callback
    glfwSetFramebufferSizeCallback(window, &WindowSystemUtility::frame_buffer_size_callback);

    // For now following settings are for all windows
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE,1);
    glfwSetWindowUserPointer(window, this);
    auto lambda_ftn = [](GLFWwindow *w, int key, int scancode, int action, int mods) {
        GWindow* this_window = static_cast<GWindow*> (glfwGetWindowUserPointer(w));
        this_window->key_input_callback(w,key,scancode,action,mods);
    };

    auto mouse_input_ftn_ptr = [](GLFWwindow* w, f64 x_pos, f64 y_pos) {
        GWindow* my_window = static_cast<GWindow*> (glfwGetWindowUserPointer(w));
        my_window->mouse_input_callback(w,x_pos,y_pos);
    };

    glfwSetKeyCallback(window, lambda_ftn);
    glfwSetCursorPosCallback(window, mouse_input_ftn_ptr);
    input_system->set_current_window(this);

}

void GWindow::make_context_current()
{
    glfwMakeContextCurrent(window);
}

bool GWindow::should_close_window()
{
    return glfwWindowShouldClose(window);
}

GLFWwindow *GWindow::get_window_ptr()
{
    return window;
}

void GWindow::key_input_callback(GLFWwindow *w, int key, int scancode, int action, int mods)
{
    input_system->key_input_callback(key, scancode, action, mods);
}

void GWindow::poll_input_events()
{
    glfwPollEvents();
    input_system->repeat_input_callbacks();
}

void GWindow::should_hide_cursor(bool should_hide)
{
    EventSystemHelper::queue_events(EVENTTYPE_CURSOR_ACTIVATED, new EventCursorActivated(should_hide));
    u32 value = should_hide ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
    glfwSetInputMode(window, GLFW_CURSOR, value);
}

GWindow::~GWindow()
{
    glfwSetWindowShouldClose(window, 1);
    glfwDestroyWindow(window);
}

void GWindow::mouse_input_callback(GLFWwindow *window, f64 x_pos, f64 y_pos)
{
    input_system->mouse_input_callback(x_pos, y_pos);
}
