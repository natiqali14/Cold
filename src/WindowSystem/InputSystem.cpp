#include "InputSystem.h"
#include "../EventSystem/EventSystemHelper.h"
#include <Logger.h>
InputSystem::InputSystem()
{
}

void InputSystem::key_input_callback(int key, int scancode, int action, int mods)
{
    switch (action)
    {
    // case GLFW_REPEAT: {
    //     EventSystemHelper::queue_events(EVENTTYPE_KEY_PRESSED, new KeyPressedEvent(key));
    //     break;
    // }
    case GLFW_PRESS:
    {
        input_keys_pressed.insert(key);
        break;
    }

    case GLFW_RELEASE:
    {
        if (input_keys_pressed.count(key))
            input_keys_pressed.erase(key);
        EventSystemHelper::queue_events(EVENTTYPE_KEY_RELEASED, new KeyReleasedEvent(key));
        break;
    }
    }
}

void InputSystem::repeat_input_callbacks()
{
    for (auto saved_key : input_keys_pressed)
    {
        EventSystemHelper::queue_events(EVENTTYPE_KEY_PRESSED, new KeyPressedEvent(saved_key));
    }
}

void InputSystem::mouse_input_callback(f64 x_pos, f64 y_pos)
{
    EventSystemHelper::queue_events(EVENTTYPE_MOUSE_MOVED, new KeyMouseMovedEvent(x_pos, y_pos));
}
