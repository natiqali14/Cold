#include "StaticMesh.h"
#include <Logger.h>
#include <ModelLoader.h>
#include <helper.h>
namespace Cold {
    StaticMesh::StaticMesh(TransformSPtr root, const std::string& file_path)
    : root_transform(std::move(root))
    , mesh_file_path(Helper::normalize_paths(file_path))
    , should_cull_face(true)
    {
        COLD_TRACE("StaticMesh constructed with file path %s", mesh_file_path.c_str());
    }
    void StaticMesh::push_geometry(GeometryId geometry_id)
    {
        std::lock_guard<std::timed_mutex> lock(mtx);
        geometries.push_back(geometry_id);
       // should_buffer_gpu_data = true;
    }
    void StaticMesh::buffer_to_gpu(const GeometrySystemSPtr& geom)
    {
        i32 index = 0;
        for(GeometryId id : geometries) {
            bool ret = geom->buffer_geometry_data_to_gpu(id);
            if (ret) index++;
            if (index  > 3) break;
        }

    }

    void StaticMesh::render(const GeometrySystemSPtr& geom) {
        std::unique_lock<std::timed_mutex> lock(mtx, std::defer_lock);
        if (lock.try_lock_for(std::chrono::milliseconds(1))) {
            buffer_to_gpu(geom);
            if(!should_cull_face) {
                glDisable(GL_CULL_FACE);
            }
            for(GeometryId id : geometries) {
                geom->render_geometry(id, root_transform);
            }
            if(!should_cull_face) {
                glEnable(GL_CULL_FACE);
                glCullFace(GL_FRONT);
                glFrontFace(GL_CW);
            }
        }

    }

    void StaticMesh::load_mesh(const GeometrySystemSPtr& geom) {
        // TODO have to find some ways for the flags but for now const flags are ok
        ModelLoader::model_load(mesh_file_path,
                                aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace,
                                this, geom);
    }

    void StaticMesh::set_cull_facing(bool cull_face) {
        should_cull_face = cull_face;
    }

    void StaticMesh::set_material(const GeometrySystemSPtr& geom, const Material& material, const std::string &geom_name) {
        auto id = geom->get_geometry_id(geom_name);
        geom->change_material(id, material);
    }

    TransformSPtr StaticMesh::get_transform() {
        return root_transform;
    }
}