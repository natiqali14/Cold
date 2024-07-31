#pragma once 
#include "includes.h"
#include "opengl_backend/buffers/VertexArrayBuffer.h"
#include "opengl_backend/buffers/Vertexbuffer.h"
#include <CameraSystem.h>
#include <StaticMesh.h>
// SOME extra structs down below just for prac


namespace Cold {
    struct vertex_data {
        std::vector<std::shared_ptr<VertexArrayBuffer>> vertex_array_buffers;
        u32 tex_id;
        u32 ebo_id;
        u32 program_id;
        CameraSystem* cs;
        StaticMesh* m;
        std::vector<StaticMesh*> meshes;
    };

    inline vertex_data v_data = {};
}


#define FRAME_RATE 60
#define MICRO_SECONDS_PER_FRAME 1000000 / FRAME_RATE