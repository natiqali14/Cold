#pragma once
#include <includes.h>
#include <GeometrySystem.h>
#include <Transforms.h>
namespace Cold {
    class Mesh {
    public:
        Mesh(TransformSPtr root);
        ~Mesh();
        void push_geometry(GeometryId geometry_id);
        void buffer_to_gpu();
        void render();
    private:
        TransformSPtr root_transform;
        std::vector<GeometryId> geometries;
    };
}