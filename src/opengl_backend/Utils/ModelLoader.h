#pragma once
#include <includes.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <StaticMesh.h>
#include "../Geometry/GeometrySystem.h"
namespace Cold {
    class ModelLoader {
    public:
        static void initiate();
        static void shutdown();
        static void model_load(const std::string& file_path, u32 flags, StaticMesh* mesh, const GeometrySystemSPtr& geom);

    private:
    };
}


