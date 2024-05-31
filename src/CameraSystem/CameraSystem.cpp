#include "CameraSystem.h"
#include "../EventSystem/EventSystemHelper.h"
#include "../includes/GLFW/glfw3.h"

CameraSystem::CameraSystem()
: key_press_handler (std::make_shared<EventHandler<KeyPressedEvent>>
   (EventHandler<KeyPressedEvent>([this](KeyPressedEvent* e) { onKeyPress(e); })))
, key_release_handler (std::make_shared<EventHandler<KeyReleasedEvent>>
   (EventHandler<KeyReleasedEvent>([this](KeyReleasedEvent* e) { on_key_release(e); })))
, camera_view_space()
, camera_position()
, camera_direction(0,0,-1)
, camera_up(0,1,0)
{
    EventSystemHelper::subscribe(EVENTTYPE_KEY_PRESSED, key_press_handler);
    EventSystemHelper::subscribe(EVENTTYPE_KEY_RELEASED, key_release_handler);
}

    


void CameraSystem::onKeyPress(KeyPressedEvent* event)
{
    u32 key = event->get_key_code();
    std::cout << "pressed " << key << " end \n"; 
    event->set_handled_flag(true);
    // for forward movement
    switch (key)
    {
    case GLFW_KEY_W:
        camera_position += (camera_speed * camera_direction);
        break;
    case GLFW_KEY_S:
        camera_position -= (camera_speed * camera_direction);
        break;
    case GLFW_KEY_A:
        camera_position -= (camera_speed * glm::normalize(glm::cross(camera_direction, camera_up)));
        break;
    case GLFW_KEY_D:
        camera_position += (camera_speed * glm::normalize(glm::cross(camera_direction, camera_up)));
        break;
    default:
        break;
    }
    camera_view_space = glm::lookAt(camera_position, camera_position + camera_direction, camera_up);
    

}

void CameraSystem::on_key_release(KeyReleasedEvent* event)
{
    std::cout << "released " << event->get_key_code() << " end \n"; 
    event->set_handled_flag(true);
}

glm::mat4 &CameraSystem::get_camera_view_space()
{
    return camera_view_space; 
}
