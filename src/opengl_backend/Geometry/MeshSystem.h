//
// Created by frio on 26/11/2024.
//

#ifndef MESHSYSTEM_H
#define MESHSYSTEM_H

#include <memory>
#include <unordered_map>
#include <includes.h>
#include "GeometrySystem.h"
using StaticMeshId = u32;
class StaticMesh;
using StaticMeshSPtr = std::shared_ptr<StaticMesh>;
namespace Cold {
    class MeshSystem {
    public:
        static std::shared_ptr<MeshSystem> initiate();
        StaticMeshId create_static_mesh(const std::string& model_path, const GeometrySPtr& geom, TransformSPtr root_transform);
        ~MeshSystem();

    private:
        MeshSystem();

    private:
        std::unordered_map<StaticMeshId, StaticMeshSPtr> static_meshes;
        GeometrySPtr geometry_system;
    };
}




#endif //MESHSYSTEM_H
