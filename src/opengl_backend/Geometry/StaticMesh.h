#pragma once
#include <includes.h>
#include <GeometrySystem.h>
#include <Transforms.h>
namespace Cold {
    class StaticMesh {
    public:
        StaticMesh(TransformSPtr root);
        ~StaticMesh();
        void push_geometry(GeometryId geometry_id);
        void buffer_to_gpu();
        void render();
        TransformSPtr get_transform();
    private:
        TransformSPtr root_transform;
        std::vector<GeometryId> geometries;
    };
}