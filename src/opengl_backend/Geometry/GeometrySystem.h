#pragma once
#include <includes.h>
#include <initializer_list>
#include <VertexBuffer.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Geometry.h"
#include <mutex>
namespace Cold {
   
    struct GeommetryConfig {
        aiVector3D* verticies;
        u32 vertex_count;
        aiVector3D* normals;
        u32 normal_count;
        aiVector3D* tex_coords;
        u32 tex_coords_count;
        aiFace* faces;
        u32 faces_count;
    };
    using GeometryId = u32;
    using GeometrySPtr = std::shared_ptr<Geometry>;
    class GeometrySystem {
    public:
        GeometrySystem() = default;
        static std::shared_ptr<GeometrySystem> initiate();
        static void shutdown();
        /* Here the path is file name + mesh name provided by assimp */
        GeometryId create_geometry(const std::string& geometry_path);
        GeometryId generate_id();
        void pass_data_to_geometry(GeometryId id, const GeommetryConfig& geometry_config);
        void pass_data_to_geometry(GeometryId id, const std::vector<Vertex>& geom_data);
        void pass_indicies_data_to_geometry(GeometryId id, const std::vector<u32>& index_data);
        void buffer_geometry_data_to_gpu(GeometryId id);
        void render_geometry(GeometryId id, TransformSPtr static_mesh_transform);
        bool delete_geometry(GeometryId id);
        bool should_load_geometry_data(const std::string& geometry_path);
        void set_material(GeometryId id, const Material& material);
        void change_material(GeometryId id, const Material& material);
        void set_geometry_parent_transform(GeometryId id, TransformSPtr transform);
        GeometryId get_geometry_id(const std::string& geom_name);
        void pass_material_to_gpu(GeometryId id);
        TransformSPtr get_transform(GeometryId id);
        ~GeometrySystem();
    private: 
        std::unordered_map<GeometryId, GeometrySPtr> geometries;
        std::unordered_map<std::string , GeometryId> geometry_path_refs;
        std::unordered_set<GeometryId> geometries_with_gpu_data;
        std::mutex geom_mutex;

    };

    using GeometrySystemSPtr = std::shared_ptr<GeometrySystem>;
}