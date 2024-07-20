#include "TextureSystem.h"
#include <Logger.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <filesystem>
namespace Cold {
    static TextureSystem* instance = nullptr;

    struct Texture {
        u32 width;
        u32 height;
        GLenum internal_format;
        GLenum data_type;
        u32 lod_level;
        u16 ref_count;
        Texture(u32 w, u32 h, GLenum int_form, GLenum d_type, u32 lods, u16 count)
        : width(w), height(h), internal_format(int_form), data_type(d_type), lod_level(lods), ref_count(count) {

        }
    };

    static GLenum get_gl_format(u8 channel) {
    switch (channel) {
        case 1:
            return GL_R8;
        case 2:
            return GL_RG16;
        case 3:
            return GL_RGB8;
        case 4:
            return GL_RGBA8;
        default:
            return GL_R8;
    }
}

    void TextureSystem::initiate()
    {
        instance = new TextureSystem();
        COLD_TRACE("Texture System Initiated");
    }

    void TextureSystem::shutdown()
    {
        delete instance;
        instance = nullptr;
        COLD_TRACE("Texture System Shutdown");
    }

    bool TextureSystem::delete_texture(const u32 texture_id)
    {
        // texture id not found
        if(!instance->textures.count(texture_id)) return false;
        // texture id found, ref count decrease by 1
        instance->textures[texture_id]->ref_count--;
        if(instance->textures[texture_id]->ref_count <= 0) {
            // ref count goes to 0, delete the texture
            glDeleteTextures(1, &texture_id);
            instance->textures.erase(texture_id);
            return true;
        }
        return false;
    }

    u32 TextureSystem::texture_2D_immutable_create(const std::string &path, const i32 lod_level, const GLenum internal_format)
    {
        if(instance->external_textures_ref.count(path)) {
            u32 u_id = instance->external_textures_ref.count(path);
            instance->textures[u_id]->ref_count++;
            return u_id;
        }

        std::filesystem::path cwd = std::filesystem::current_path();
        cwd = cwd / path;
        i32 width, height, channels;
        u8* image = stbi_load(cwd.c_str(), &width, &height,&channels, 0 ); 
        std::string msg = std::string("Cant load image ") + cwd.string();
        COLD_ASSERT(image != NULL, msg.c_str());
        GLenum image_format = get_gl_format(channels);

        u32 id;
        // TODO right now as we are using 4.0 version of openGL, so for imutable we use glTexImage
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, lod_level, internal_format, width, height, 0, image_format, GL_UNSIGNED_BYTE, image);
        // TODO right now only hard-coding liner filtering for textures, have to make is more customisable
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  //minification filter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  //magnification filter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  //clamp horizontal
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  //clamp vertical   

        glGenerateMipmap(GL_TEXTURE_2D);

        instance->textures.insert(
            {
                id,
                std::make_shared<Texture>(width, height, internal_format, GL_UNSIGNED_BYTE, lod_level, 1)

            }
        );
        if(!path.empty()) {
            instance->external_textures_ref.insert(
                {
                    path, id
                }
            );
        }
        stbi_image_free(image);
        return id;
    }

    u32 TextureSystem::texture_2D_mutable_create(const std::string &path, const i32 lod_level, const GLenum internal_format)
    {
       if(instance->external_textures_ref.count(path)) {
            u32 u_id = instance->external_textures_ref.count(path);
            instance->textures[u_id]->ref_count++;
            return u_id;
        }

        std::string new_path = "../../../" + path;
        i32 width, height, channels;
        u8* image = stbi_load(new_path.c_str(), &width, &height,&channels, 0 ); 
        GLenum image_format = get_gl_format(channels);
        u32 id;
        glGenTextures(GL_TEXTURE_2D, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, lod_level, internal_format, width, height, 0, image_format, GL_UNSIGNED_BYTE, image);
        // TODO right now only hard-coding liner filtering for textures, have to make is more customisable
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  //minification filter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  //magnification filter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  //clamp horizontal
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  //clamp vertical   

        glGenerateMipmap(GL_TEXTURE_2D);

        instance->textures.insert(
            {
                id,
                std::make_shared<Texture>(width, height, internal_format, GL_UNSIGNED_BYTE, lod_level, 1)

            }
        );
        if(!path.empty()) {
            instance->external_textures_ref.insert(
                {
                    path, id
                }
            );
        }
        stbi_image_free(image);
        return id;
    }
}