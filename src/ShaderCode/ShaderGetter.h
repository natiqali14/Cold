#pragma once
// TODO a very bad way to do shader loading but for now its gonna work ;)
#include <includes.h>
#include <unordered_map>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <Logger.h>
namespace Cold {
    enum ShaderEnum {
        SE_NONE,
        VERT_3D,


        // frags
        FRAG_3D,

        // ..
       SE_MAX,
    };
    inline std::unordered_map<ShaderEnum, std::string> shader_map;
    static void get_shader_string(ShaderEnum e) {
        std::filesystem::path cwd = std::filesystem::current_path();
        cwd /= "src/ShaderCode/";
        switch (e)
        {
        case VERT_3D:
        {
            cwd /= "VertShaders/vert_3D.vert";
        }
            break;
        case FRAG_3D:
        {
            cwd /= "FragShaders/frag_3D.frag";
        }
        default:
            break;
        }
        std::ifstream file(cwd.string());
        if(!file.is_open()) COLD_ERROR("Error opening file %s", cwd.string().c_str());
        std::stringstream ss;
        ss << file.rdbuf();
        shader_map.insert({e, ss.str()});
    }
    static void initiate_shader_map() {
        for(i32 i = 1; i < SE_MAX; i++) {
           get_shader_string((ShaderEnum)i);
        }
    }
}