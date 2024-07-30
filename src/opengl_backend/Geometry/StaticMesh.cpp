#include "StaticMesh.h"
#include <Logger.h>
namespace Cold {
    Mesh::Mesh(TransformSPtr root)
    : root_transform(std::move(root))
    {
        COLD_TRACE("Mesh constructed");
    }
    void Mesh::push_geometry(GeometryId geometry_id)
    {
        geometries.push_back(geometry_id);
    }
    void Mesh::buffer_to_gpu()
    {
        for(GeometryId id : geometries) {
            GeometrySystem::buffer_geometry_data_to_gpu(id);
        }
    }
    void Mesh::render()
    {
        for(GeometryId id : geometries) {
            GeometrySystem::render_geometry(id);
        }
    }
}