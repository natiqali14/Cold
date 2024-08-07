#pragma once
#include <includes.h>
#include <initializer_list>
#include <VertexBuffer.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Geometry.h"
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
        static void initiate();
        static void shutdown();
        /* Here the path is file name + mesh name provided by assimp */
        static GeometryId create_geometry(const std::string& geometry_path);
        static GeometryId generate_id();
        static void pass_data_to_geometry(GeometryId id, const GeommetryConfig& geometry_config);
        static void pass_data_to_geometry(GeometryId id, const std::vector<Vertex>& geom_data);
        static void pass_indicies_data_to_geometry(GeometryId id, const std::vector<u32>& index_data);
        static void buffer_geometry_data_to_gpu(GeometryId id);
        static void render_geometry(GeometryId id);
        static bool delete_geometry(GeometryId id);
        static bool should_load_geometry_data(const std::string& geometry_path);
        static void set_material(GeometryId id, const Material& material);
        static void set_geometry_parent_transform(GeometryId id, TransformSPtr transform);
        static TransformSPtr get_transform(GeometryId id);
    private: 
        GeometrySystem() = default;
        ~GeometrySystem();
        std::unordered_map<GeometryId, GeometrySPtr> geometries;
        std::unordered_map<std::string , GeometryId> geometry_path_refs;
        std::unordered_set<GeometryId> geometries_with_gpu_data;
    };
}