#pragma once
#include <includes.h>
#include <memory>
#include <glm/glm.hpp>
#include <StaticMesh.h>
#include <vector>
#include <Events.h>
#include <EventHandler.h>
#include <EventSystemHelper.h>
#include <GeometrySystem.h>

namespace Cold {
    struct FramePacket {
        f32 frame_rate;
        f32 screen_width;
        f32 screen_height;
    };

    using FramePacketSPtr = std::shared_ptr<FramePacket>;

    struct GlobalUniformObject 
    {
        alignas(64) glm::mat4 projection;
        alignas(64) glm::mat4 view_model {1.0};
        alignas(16) glm::vec3 camera_position {1.0};
        alignas(16) glm::vec3 ambient_color {0.2,0.2,0.2};
        alignas(16) glm::vec3 directional_light_color {01.0,01.0,0.9};
        alignas(16) glm::vec3 directional_light_vector {-0.7, 0.7, 0.7};
        GlobalUniformObject() {
            projection = glm::perspective(45.0f, 800.f / 600.f, 0.1f, 1000.f);
        }
    };

    class RendererBackend {
    public:
        RendererBackend();
        ~RendererBackend();
         void initiate();
         void shut_down();
         void on_frame_render();
         void set_frame_packet(FramePacketSPtr frame_packet);
         void set_open_gl_settings();
         void on_camera_props_change(const glm::mat4& new_view_model, const glm::vec3 new_camera_position);
         void load_data(); // TODO should not be here, make a scene class or something similar

        ////////////////////////////////////  API CALLS  ////////////////////////////////////

        /// 1. For Static Mesh.

        i32 create_static_mesh(const std::string& path, glm::vec3 translation , glm::vec3 scale);
        void delete_static_mesh(i32 static_mesh_id);

        ////////////////////////////////////  API CALLS  ////////////////////////////////////

    private:
        GlobalUniformObject* global_uniform_object;
        std::unordered_map<std::string , StaticMesh*> meshes;
        GeometrySystemSPtr geometry_system;
        void initialise_default_shader(); // TODO SHOULD BE MOVED FROM HERE

        // TODO move to some place else here just for testing
        EventHandler<KeyPressedEvent>* key_press_handler;

        void on_key_press_event(KeyPressedEvent* event);

        void pass_camera_props_to_gpu();
    };
}
