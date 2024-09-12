#pragma once

#include "../includes.h"
#include "../EventSystem/Events.h"
#include "../EventSystem/EventHandler.h"
class CameraSystem
{
public:
    CameraSystem();
    ~CameraSystem(); // for now
    void onKeyPress(KeyPressedEvent *event);
    void on_key_release(KeyReleasedEvent *event);
    void on_mouse_moved(KeyMouseMovedEvent *event);
    glm::mat4 &get_camera_view_space();
    glm::vec3 &get_camera_position();

private:
    EventHandler<KeyPressedEvent> *key_press_handler;
    EventHandler<KeyReleasedEvent> *key_release_handler;
    EventHandler<KeyMouseMovedEvent> *key_mouse_moved_handler;
    EventHandler<EventCursorActivated> *event_cursor_activated;
    f32 camera_speed = 0.5f;
    f32 rotation_factor = 3.0f;

    glm::mat4 camera_view_space;
    glm::vec3 camera_position;
    glm::vec3 camera_direction;
    glm::vec3 camera_up;

    // for angles
    f32 last_x;
    f32 last_y;
    bool on_first_move {true};

    f32 pitch;
    f32 yaw;
    bool should_move {true};


    // functions

    void on_cursor_activated(EventCursorActivated* event);

};