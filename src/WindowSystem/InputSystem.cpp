#include "InputSystem.h"
#include "../EventSystem/EventSystemHelper.h"
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
        case GLFW_PRESS: {
            input_keys_pressed.insert(key);
            break;
        }
            
        case GLFW_RELEASE: {
            if(input_keys_pressed.count(key)) input_keys_pressed.erase(key);
            std::cout << "Key is released " << "\n";
            EventSystemHelper::queue_events(EVENTTYPE_KEY_RELEASED, new KeyReleasedEvent(key));
            break;
        }
        
    }

    
}

void InputSystem::repeat_input_callbacks()
{
    for(auto saved_key : input_keys_pressed) {
        std::cout << "pressed set size " << input_keys_pressed.size() << "\n";
        EventSystemHelper::queue_events(EVENTTYPE_KEY_PRESSED, new KeyPressedEvent(saved_key));
    }
}
