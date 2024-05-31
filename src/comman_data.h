#include "includes.h"
#pragma once
// SOME extra structs down below just for prac
struct buffer_data {
    u32 vbo;
    u32 vao;
    u32 ebo;
};

struct CameraData {
    glm::vec3 position = glm::vec3(0,0,0);
};



#define FRAME_RATE 60
#define MICRO_SECONDS_PER_FRAME 1000000 / FRAME_RATE