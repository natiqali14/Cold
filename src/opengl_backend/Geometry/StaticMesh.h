#pragma once
#include <includes.h>
#include <GeometrySystem.h>
#include <Transforms.h>
namespace Cold {
    class StaticMesh {
    public:
        StaticMesh(TransformSPtr root, const std::string& mesh_path);
        ~StaticMesh();
        void push_geometry(GeometryId geometry_id);
        void buffer_to_gpu(const GeometrySystemSPtr& geom);
        void render(const GeometrySystemSPtr& geom);
        void load_mesh(const GeometrySystemSPtr& geom);
        void set_cull_facing(bool cull_face);
        void set_material(const GeometrySystemSPtr& geom, const Material& material, const std::string& geom_name);
        TransformSPtr get_transform();
    private:
        TransformSPtr root_transform;
        std::vector<GeometryId> geometries {};
        std::string mesh_file_path;
        bool should_cull_face;
        bool should_buffer_gpu_data {true};
        std::mutex mtx;
    };
}