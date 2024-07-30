#include "StaticMesh.h"
#include <Logger.h>
namespace Cold {
    StaticMesh::StaticMesh(TransformSPtr root)
    : root_transform(root)
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
    TransformSPtr StaticMesh::get_transform()
    {
        return root_transform;
    }
}