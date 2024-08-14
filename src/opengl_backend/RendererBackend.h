#pragma once
#include <includes.h>
#include <memory>
#include <glm/glm.hpp>
#include <StaticMesh.h>
#include <vector>
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
        alignas(16) glm::vec3 ambient_color {0.2,0.2,0.2};
        alignas(16) glm::vec3 directional_light_color {01.0,01.0,0.9};
        alignas(16) glm::vec3 directional_light_vector {-0.7, 0.7, 0.7};
        GlobalUniformObject() {
            projection = glm::perspective(45.0f, 800.f / 600.f, 0.1f, 1000.f);
        }
    };

    class RendererBackend {
    public:
        static void initiate();
        static void shut_down();
        static void on_frame_render();
        static void set_frame_packet(FramePacketSPtr frame_packet);
        static void set_open_gl_settings();
        static void on_view_model_change(const glm::mat4& new_view_model);
        static void load_data(); // TODO should not be here, make a scene class or something similar

        private:
            GlobalUniformObject* global_uniform_object;
            std::unordered_map<std::string , StaticMesh*> meshes;
            ~RendererBackend();
            void initialise_default_shader(); // TODO SHOULD BE MOVED FROM HERE
    };
}