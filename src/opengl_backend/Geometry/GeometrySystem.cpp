#include "GeometrySystem.h"
#include <Logger.h>
namespace Cold {
    std::shared_ptr<GeometrySystem> GeometrySystem::initiate()
    {
        COLD_TRACE("Geometry System Initiated");
        return std::make_shared<GeometrySystem>();
    }

    void GeometrySystem::shutdown()
    {
        COLD_TRACE("Geometry System shutdown. Releasing All remaining resources");
    }

    GeometrySystem::~GeometrySystem()
    {
        /* No need to forcfully delete  */
        // for(auto id : geometries) {
        //     id.second.reset();
        // }
    }

    GeometryId GeometrySystem::create_geometry(const std::string& geometry_path)
    {
        if(geometry_path_refs.count(geometry_path)) {
            auto geomerty_ref = geometry_path_refs[geometry_path];
            geometries[geomerty_ref]->increament_ref_count();
            return geomerty_ref;
        }
        GeometryId id = generate_id();
        GeometrySPtr geometry_ptr = std::make_shared<Geometry>(GL_STATIC_DRAW);
        geometries.insert({id, geometry_ptr});
        geometry_path_refs.insert({geometry_path, id});
        COLD_TRACE("Geometry created with path %s and id %d", geometry_path.c_str(), id);
        return id;
    }

    GeometryId GeometrySystem::generate_id()
    {
        static GeometryId id = 1;
        return id++; 
    }

    void GeometrySystem::pass_data_to_geometry(GeometryId id, const GeommetryConfig &geometry_config)
    {
        COLD_ASSERT(geometries.count(id) > 0, "No Geometry found in the map");
        GeometrySPtr geometry = geometries.at(id);
        geometry->push_vertex_data(geometry_config.verticies, geometry_config.vertex_count, 
                                   geometry_config.normals, geometry_config.normal_count,
                                   geometry_config.tex_coords, geometry_config.tex_coords_count);
        geometry->push_indicies(geometry_config.faces, geometry_config.faces_count);
    }

    void GeometrySystem::pass_data_to_geometry(GeometryId id, const std::vector<Vertex> &geom_data)
    {
        COLD_ASSERT(geometries.count(id) > 0, "No Geometry found in the map");
        GeometrySPtr geometry = geometries.at(id);
        geometry->push_vertex_data(geom_data);   
    }

    void GeometrySystem::pass_indicies_data_to_geometry(GeometryId id, const std::vector<u32> &index_data)
    {
        COLD_ASSERT(geometries.count(id) > 0, "No Geometry found in the map");
        GeometrySPtr geometry = geometries.at(id);
        geometry->push_inicies(index_data);
    }

    void GeometrySystem::buffer_geometry_data_to_gpu(GeometryId id)
    {
        COLD_ASSERT(geometries.count(id) > 0, "No Geometry found in the map");
        GeometrySPtr geometry = geometries.at(id);
        geometry->buffer_data_to_gpu();

    }

    void GeometrySystem::render_geometry(GeometryId id, TransformSPtr static_mesh_transform)
    {
        COLD_ASSERT(geometries.count(id) > 0, "No Geometry found in the map");
        GeometrySPtr geometry = geometries.at(id);
        geometry->render(std::move(static_mesh_transform));
    }

    bool GeometrySystem::delete_geometry(GeometryId id)
    {
        COLD_ASSERT(geometries.count(id) > 0, "No Geometry found in the map");
        auto geometry_ref = geometries[id];
        geometry_ref->decreament_ref_count();
        if(geometry_ref->get_ref_count() == 0) {
            geometry_ref.reset();
        }
        return false;
    }

    /* NO in use this function*/
    bool GeometrySystem::should_load_geometry_data(const std::string &geometry_path)
    {
        if(geometry_path_refs.count(geometry_path)) {
            return false;
        }
        return true;
    }

    void GeometrySystem::set_material(GeometryId id, const Material &material)
    {
        COLD_ASSERT(geometries.count(id) > 0, "No Geometry found in the map");
        GeometrySPtr geometry = geometries.at(id);
        geometry->set_material(material);

    }

    void GeometrySystem::change_material(GeometryId id, const Material &material)
    {
        COLD_ASSERT(geometries.count(id) > 0, "No Geometry found in the map");
        GeometrySPtr geometry = geometries.at(id);   
        geometry->change_material(material);
    }

    void GeometrySystem::set_geometry_parent_transform(GeometryId id, TransformSPtr transform)
    {
        COLD_ASSERT(geometries.count(id) > 0, "No Geometry found in the map");
        GeometrySPtr geometry = geometries.at(id);
        geometry->get_transform()->set_parent(transform);
    }

    GeometryId GeometrySystem::get_geometry_id(const std::string & geom_name)
    {
        COLD_ASSERT(geometry_path_refs.count(geom_name) != 0, "No Geometry found in the map");
        return geometry_path_refs[geom_name];
    }

    void GeometrySystem::pass_material_to_gpu(GeometryId id)
    {
        COLD_ASSERT(geometries.count(id) > 0, "No Geometry found in the map");
        GeometrySPtr geometry = geometries.at(id);
        geometry->buffer_material_data();
    }

    TransformSPtr GeometrySystem::get_transform(GeometryId id)
    {
        COLD_ASSERT(geometries.count(id) > 0, "No Geometry found in the map");
        GeometrySPtr geometry = geometries.at(id);
        return geometry->get_transform();
    }
}