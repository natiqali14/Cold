#pragma once
#include <includes.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
namespace Cold {
    class ModelLoader {

    public:
        static void initiate();
        static void shutdown();
        static void  model_load(const std::string& file_path, u32 flags);

    private:
    };
}


