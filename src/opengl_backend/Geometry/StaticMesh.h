#pragma once
#include <includes.h>
#include <GeometrySystem.h>
#include <Transforms.h>
namespace Cold {
    class StaticMesh {
    public:
        StaticMesh(TransformSPtr root, const std::string mesh_path);
        ~StaticMesh();
        void push_geometry(GeometryId geometry_id);
        void buffer_to_gpu();
        void render();
        void load_mesh();
        void set_cull_facing(bool cull_face);
        TransformSPtr get_transform();
    private:
        TransformSPtr root_transform;
        std::vector<GeometryId> geometries;
        std::string mesh_file_path;
        bool should_cull_face;
    };
}