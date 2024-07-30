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
        GLenum data_type;
        u16 ref_count;
        TextureProps tex_props;
        Texture(u32 w, u32 h, u16 count, TextureProps props)
        : width(w), height(h), ref_count(count), tex_props(props) {

        }
    };

    static GLenum get_gl_format(u8 channel) {
    switch (channel) {
        case 1:
            return GL_R;
        case 2:
            return GL_RG;
        case 3:
            return GL_RGB;
        case 4:
            return GL_RGBA;
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
        for(auto& pair : instance->textures) {
            glDeleteTextures(1, &pair.first);
        }
        delete instance;
        instance = nullptr;
        COLD_TRACE("Texture System Shutdown");
    }

    bool TextureSystem::delete_texture(const TextureId texture_id)
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

    TextureId TextureSystem::texture_2D_immutable_create(const std::string &path, const TextureProps& props)
    {
        if(instance->external_textures_ref.count(path)) {
            TextureId u_id = instance->external_textures_ref.count(path);
            instance->textures[u_id]->ref_count++;
            return u_id;
        }

        std::filesystem::path cwd = std::filesystem::current_path(); // TODO find better way
        cwd = cwd / path;
        i32 width, height, channels;
        u8* image = stbi_load(cwd.string().c_str(), &width, &height,&channels, 0 ); 
        std::string msg = std::string("Cant load image ") + cwd.string();
        COLD_ASSERT(image != NULL, msg.c_str());
        COLD_TRACE("Image of path %s, with width %d, height %d and channels %d", cwd.string().c_str(), width, height, channels);
        GLenum image_format = get_gl_format(channels);

        TextureId id;
        // TODO right now as we are using 4.0 version of openGL, so for imutable we use glTexImage
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
       
        // TODO right now only hard-coding liner filtering for textures, have to make is more customisable
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, props.minification_filter);  //minification filter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, props.magnification_fillter);  //magnification filter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, props.wrap_x_axis);  //clamp horizontal
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, props.wrap_y_axis);  //clamp vertical   

        if(width % 4 != 0 || height % 4 != 0) glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // if not then they have no 4 bytes allignment
        // TODO for now its better to use image_format got from channels
        glTexImage2D(GL_TEXTURE_2D, 0, image_format, width, height, 0, image_format, props.image_data_type, image);
        if(props.should_generate_mipmap)
            glGenerateMipmap(GL_TEXTURE_2D);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        instance->textures.insert(
            {
                id,
                std::make_shared<Texture>(width, height, 1, std::move(props))

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
        glBindTexture(GL_TEXTURE_2D, 0);
        return id;
    }

    TextureId TextureSystem::texture_2D_mutable_create(const std::string &path, const TextureProps& props)
    {
       if(instance->external_textures_ref.count(path)) {
            TextureId u_id = instance->external_textures_ref.count(path);
            instance->textures[u_id]->ref_count++;
            return u_id;
        }

        std::filesystem::path cwd = std::filesystem::current_path(); // TODO find better way
        cwd = cwd / path;
        i32 width, height, channels;
        u8* image = stbi_load(cwd.string().c_str(), &width, &height,&channels, 0 ); 
        std::string msg = std::string("Cant load image ") + cwd.string();
        COLD_ASSERT(image != NULL, msg.c_str());
        COLD_TRACE("Image of path %s, with width %d, height %d and channels %d", cwd.string().c_str(), width, height, channels);
        GLenum image_format = get_gl_format(channels);

        TextureId id;
        // TODO right now as we are using 4.0 version of openGL, so for imutable we use glTexImage
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
       
        // TODO right now only hard-coding liner filtering for textures, have to make is more customisable
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, props.minification_filter);  //minification filter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, props.magnification_fillter);  //magnification filter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, props.wrap_x_axis);  //clamp horizontal
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, props.wrap_y_axis);  //clamp vertical   
       
        glTexImage2D(GL_TEXTURE_2D, 0, props.internal_format, width, height, 0, image_format, props.image_data_type, image);
        if(props.should_generate_mipmap)
            glGenerateMipmap(GL_TEXTURE_2D);

        instance->textures.insert(
            {
                id,
                std::make_shared<Texture>(width, height, 1, std::move(props))

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
        glBindTexture(GL_TEXTURE_2D, 0);
        return id;
    }
}