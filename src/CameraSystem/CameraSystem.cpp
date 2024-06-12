#include "CameraSystem.h"
#include "../EventSystem/EventSystemHelper.h"
#include "../includes/GLFW/glfw3.h"


CameraSystem::CameraSystem()
    : key_press_handler  // Key press input handler
    (
        std::make_shared<EventHandler<KeyPressedEvent>>
        (
            [this](KeyPressedEvent *e) { 
                onKeyPress(e); 
            }
        )
    )
    , key_release_handler (
        std::make_shared<EventHandler<KeyReleasedEvent>>
        (
            [this](KeyReleasedEvent* e) {
                on_key_release(e);
            }
        )
    )
    , key_mouse_moved_handler
    (
        std::make_shared<EventHandler<KeyMouseMovedEvent>>
        (
            [this](KeyMouseMovedEvent *e) {
                on_mouse_moved(e); 
            }
        )
    )
    , camera_view_space()
    , camera_position()
    , camera_direction(0, 0, -1)
    , camera_up(0, 1, 0)
    , last_x(SCR_WIDTH / 2)
    , last_y(SCR_HEIGHT / 2)
    , pitch(0)
    , yaw(0)
{
    EventSystemHelper::subscribe(EVENTTYPE_KEY_PRESSED, key_press_handler);
    EventSystemHelper::subscribe(EVENTTYPE_KEY_RELEASED, key_release_handler);
    EventSystemHelper::subscribe(EVENTTYPE_MOUSE_MOVED, key_mouse_moved_handler);
}

void CameraSystem::onKeyPress(KeyPressedEvent *event)
{
    u32 key = event->get_key_code();
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

void CameraSystem::on_key_release(KeyReleasedEvent *event)
{
    event->set_handled_flag(true);
}

void CameraSystem::on_mouse_moved(KeyMouseMovedEvent *event)
{
    f32 mouse_x = event->get_values()[0];
    f32 mouse_y = event->get_values()[1];

    f32 new_offset_x = mouse_x - last_x;
    f32 new_offset_y = -mouse_y + last_y;
    last_x = mouse_x;
    last_y = mouse_y;
    new_offset_x *= 0.1;
    new_offset_y *= 0.1;
    yaw += new_offset_x;
    pitch += new_offset_y;
    if(pitch > 89) pitch = 89;
    else if (pitch < -89) pitch = -89;
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_direction = glm::normalize(direction);
    camera_view_space = glm::lookAt(camera_position, camera_position + camera_direction, camera_up);
    event->set_handled_flag(true);
}

glm::mat4 &CameraSystem::get_camera_view_space()
{
    return camera_view_space;
}
