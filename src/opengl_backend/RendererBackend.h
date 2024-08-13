#pragma once
#include <includes.h>
#include <memory>
#include <glm/glm.hpp>
namespace Cold {
    struct FramePacket {
        f32 frame_rate;
        f32 screen_width;
        f32 screen_height;
    };

    using FramePacketSPtr = std::shared_ptr<FramePacket>;

    struct GlobalUniformObject {
        alignas(64) glm::mat4 projection {1.0};
        alignas(64) glm::mat4 view_model {1.0};
        alignas(16) glm::vec4 ambient_color {0.2,0.2,0.2,1.0};
        alignas(16) glm::vec4 directional_light_color {0.8,0.8,0.8,1.0};

    };

    class RendererBackend {
    public:
        static void initiate();
        static void shut_down();
        static void on_frame_render();
        static void set_frame_packet(FramePacketSPtr frame_packet);
        static void set_open_gl_settings();

        static void load_data(); // TODO should not be here, make a scene class or something similar

        private:
            GlobalUniformObject* global_uniform_object;

            ~RendererBackend();
            void initialise_default_shader(); // TODO SHOULD BE MOVED FROM HERE
    };
}