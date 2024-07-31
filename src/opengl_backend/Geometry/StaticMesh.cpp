#include "StaticMesh.h"
#include <Logger.h>
#include <ModelLoader.h>
namespace Cold {
    StaticMesh::StaticMesh(TransformSPtr root, const std::string file_path)
    : root_transform(root)
    , mesh_file_path(std::move(file_path))
    {
        COLD_TRACE("StaticMesh constructed");
    }
    void StaticMesh::push_geometry(GeometryId geometry_id)
    {
        geometries.push_back(geometry_id);
    }
    void StaticMesh::buffer_to_gpu()
    {
        for(GeometryId id : geometries) {
            GeometrySystem::buffer_geometry_data_to_gpu(id);
        }
    }
    void StaticMesh::render()
    {
        for(GeometryId id : geometries) {
            GeometrySystem::render_geometry(id);
        }
    }
    void StaticMesh::load_mesh()
    {
        // TODO have to find some ways for the flags but for now const flags are ok
        ModelLoader::model_load(mesh_file_path,
                                aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals,
                                this);
    }
    TransformSPtr StaticMesh::get_transform()
    {
        return root_transform;
    }
}