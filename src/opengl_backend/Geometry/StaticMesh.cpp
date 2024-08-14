#include "StaticMesh.h"
#include <Logger.h>
#include <ModelLoader.h>
#include <helper.h>
namespace Cold {
    StaticMesh::StaticMesh(TransformSPtr root, const std::string file_path)
    : root_transform(root)
    , mesh_file_path(Helper::normalize_paths(file_path))
    , should_cull_face(true)
    {
        COLD_TRACE("StaticMesh constructed with file path %s", mesh_file_path.c_str());
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
        if(!should_cull_face) {
            glDisable(GL_CULL_FACE);
        }
        for(GeometryId id : geometries) {
            GeometrySystem::render_geometry(id);
        }
        if(!should_cull_face) {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            glFrontFace(GL_CW);
        }
    }
    void StaticMesh::load_mesh()
    {
        // TODO have to find some ways for the flags but for now const flags are ok
        ModelLoader::model_load(mesh_file_path,
                                aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals,
                                this);
    }
    void StaticMesh::set_cull_facing(bool cull_face)
    {
        should_cull_face = cull_face;
    }
    TransformSPtr StaticMesh::get_transform()
    {
        return root_transform;
    }
}