#include "TextureSystem.h"
#include <Logger.h>
namespace Cold {
    static TextureSystem* instance = nullptr;

    struct Texture {
        u32 unique_id;
        u32 width;
        u32 height;
        GLenum internal_format;
        GLenum data_type;
        u32 lod_level;
        u16 ref_count;
    };

    void TextureSystem::initiate()
    {
        instance = new TextureSystem();
        COLD_TRACE("Texture System Initiated");
    }

    void TextureSystem::shutdown()
    {
        delete instance;
        instance = nullptr;
        COLD_TRACE("Texture System Deleted");
    }

    bool TextureSystem::delete_texture(const u32 texture_id)
    {
    
        return false;
    }

    u32 TextureSystem::texture_2D_immutable_create(const std::string &path, const i32 lod_level, const GLenum internal_format)
    {
        return u32();
    }

    u32 TextureSystem::texture_2D_mutable_create(const std::string &path, const i32 lod_level, const GLenum internal_format)
    {
        return u32();
    }
}