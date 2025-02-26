#pragma once

#include "../../includes.h"
#include "../includes/glad/glad.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>
#include <memory>
namespace Cold {
    struct TextureProps {
        GLenum internal_format;
        bool should_generate_mipmap;
        GLenum minification_filter;
        GLenum magnification_fillter;
        GLenum wrap_x_axis;
        GLenum wrap_y_axis;
        GLenum image_data_type;

    };
    struct Texture;
    using TextureSPtr = std::shared_ptr<Texture>;
    using TextureId = u32;
    class TextureSystem {
    public:
        static void initiate();
        static void shutdown();
        static bool delete_texture(const u32 texture_id);
        static TextureId texture_2D_immutable_create(const std::string& path, const TextureProps& props);
        static TextureId texture_2D_mutable_create(const std::string& path, const TextureProps& props);
    private:
        std::unordered_map<TextureId,TextureSPtr> textures;
        std::unordered_map<std::string, TextureId> external_textures_ref;

        std::mutex mtx;
    };
}