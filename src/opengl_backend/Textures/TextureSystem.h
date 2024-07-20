#pragma once

#include "../../includes.h"
#include "../includes/glad/glad.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>
#include <memory>
namespace Cold {
    struct Texture;
    using TextureSPtr = std::shared_ptr<Texture>;
    class TextureSystem {
    public:
        static void initiate();
        static void shutdown();
        static bool delete_texture(const u32 texture_id);
        static u32 texture_2D_immutable_create(const std::string& path, const i32 lod_level, const GLenum internal_format);
        static u32 texture_2D_mutable_create(const std::string& path, const i32 lod_level, const GLenum internal_format);
    private:
        std::unordered_map<u32,TextureSPtr> textures;
        std::unordered_map<std::string, u32> external_textures_ref;
    };
}