#include "CameraSystem.h"
#include "../EventSystem/EventSystemHelper.h"
#include "../includes/GLFW/glfw3.h"
#include <Logger.h>

CameraSystem::CameraSystem()
    : key_press_handler (BIND_EVENT_FUNCTION(KeyPressedEvent, &CameraSystem::onKeyPress, this))
    , key_release_handler (BIND_EVENT_FUNCTION(KeyReleasedEvent, &CameraSystem::on_key_release, this))
    , key_mouse_moved_handler (BIND_EVENT_FUNCTION(KeyMouseMovedEvent, &CameraSystem::on_mouse_moved, this))
    , event_cursor_activated  (BIND_EVENT_FUNCTION(EventCursorActivated, &CameraSystem::on_cursor_activated, this))
    , camera_view_space()
    , camera_position()
    , camera_direction(0, 0, -1)
    , camera_up(0, 1, 0)
    , last_x(0)
    , last_y(0)
    , pitch(0)
    , yaw(0)
{
    // for key press events
    EventSystemHelper::subscribe(EVENTTYPE_KEY_PRESSED, 
    key_press_handler);
    // for key release events
    EventSystemHelper::subscribe(EVENTTYPE_KEY_RELEASED, 
    key_release_handler);
    // for move move event
    EventSystemHelper::subscribe(EVENTTYPE_MOUSE_MOVED, 
    key_mouse_moved_handler);

    // for cursor
    EventSystemHelper::subscribe(EVENTTYPE_CURSOR_ACTIVATED,
    event_cursor_activated);

    auto e1 = new KeyMouseMovedEvent(0,0);
    on_mouse_moved(e1);
    auto e2 = new KeyMouseMovedEvent(1, 1);
    on_mouse_moved(e2);
    delete e1;
    delete e2;

}

CameraSystem::~CameraSystem()
{
    EventSystemHelper::unsubscribe(EVENTTYPE_KEY_PRESSED,key_press_handler);
    EventSystemHelper::unsubscribe(EVENTTYPE_KEY_RELEASED, key_release_handler);
    EventSystemHelper::unsubscribe(EVENTTYPE_MOUSE_MOVED, key_mouse_moved_handler);
    EventSystemHelper::unsubscribe(EVENTTYPE_CURSOR_ACTIVATED, event_cursor_activated);
    delete key_press_handler;
    delete key_release_handler;
    delete key_mouse_moved_handler;
    delete event_cursor_activated;
}

void CameraSystem::onKeyPress(KeyPressedEvent *event)
{
    if (!should_move)  {
        event->set_handled_flag(true);
        return;
    }
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
    case GLFW_KEY_Q:
        camera_position += (camera_speed * camera_up);
        break;
    case GLFW_KEY_E:
        camera_position -= (camera_speed * camera_up);
        break;
    case GLFW_KEY_Z: { // rotation on left side
        yaw -= camera_speed * rotation_factor;
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camera_direction = glm::normalize(direction);
        camera_view_space = glm::lookAt(camera_position, camera_position + camera_direction, camera_up);
    }
        
        break;
    case GLFW_KEY_C: { // rotation on right side
        yaw += camera_speed * rotation_factor;
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camera_direction = glm::normalize(direction);
        camera_view_space = glm::lookAt(camera_position, camera_position + camera_direction, camera_up);
    }
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
    if (!should_move) {
        event->set_handled_flag(true);
        return;
    }
    f32 mouse_x = event->get_values()[0];
    f32 mouse_y = event->get_values()[1];
   
    f32 new_offset_x = mouse_x - last_x;
    f32 new_offset_y = mouse_y - last_y;
    if (on_first_move) {
        new_offset_x = 0;
        new_offset_y = 0;
        last_x = mouse_x;
        last_y = mouse_y;
        on_first_move = false;
        event->set_handled_flag(true);
        return;
    }

    new_offset_y *= -1; // inverting
    last_x = mouse_x; 
    last_y = mouse_y;
    new_offset_x *= 0.3;
    new_offset_y *= 0.3;
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
    on_first_move = false;
    event->set_handled_flag(true);
}

glm::mat4 &CameraSystem::get_camera_view_space()
{
    return camera_view_space;
}

glm::vec3 &CameraSystem::get_camera_position()
{
    return camera_position;
}

void CameraSystem::on_cursor_activated(EventCursorActivated *event)
{
    should_move = event->get_status();
    event->set_handled_flag(true);
    on_first_move = true;
}
