#pragma once

#include "../includes.h"
#include "../EventSystem/Events.h"
#include "../EventSystem/EventHandler.h"
#include "../comman_data.h"
class CameraSystem {
public:
    CameraSystem();
    ~CameraSystem() = default; // for now
    void onKeyPress(KeyPressedEvent* event);
    void on_key_release(KeyReleasedEvent* event);
    glm::mat4& get_camera_view_space();

private:
    std::shared_ptr<EventHandler<KeyPressedEvent>> key_press_handler;
    std::shared_ptr<EventHandler<KeyReleasedEvent>> key_release_handler;

    f32 camera_speed = 0.5f;
    
    glm::mat4 camera_view_space;
    glm::vec3 camera_position;
    glm::vec3 camera_direction;
    glm::vec3 camera_up;

};