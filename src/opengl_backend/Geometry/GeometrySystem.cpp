#include "GeometrySystem.h"
#include <Logger.h>
namespace Cold {
    static GeometrySystem* instance = nullptr;
    void GeometrySystem::initiate()
    {
        instance = new GeometrySystem;
        COLD_TRACE("Geometry System Initiated");
    }

    void GeometrySystem::shutdown()
    {
        delete instance;
        instance = nullptr;
    }

    GeometryId GeometrySystem::create_geometry(const std::string& geometry_path)
    {
        if(instance->geometry_path_refs.count(geometry_path)) {
            auto geomerty_ref = instance->geometry_path_refs[geometry_path];
            instance->geometries[geomerty_ref]->increament_ref_count();
            return geomerty_ref;
        }
        GeometryId id = generate_id();
        GeometrySPtr geometry_ptr = std::make_shared<Geometry>(GL_STATIC_DRAW);
        instance->geometries.insert({id, geometry_ptr});
        instance->geometry_path_refs.insert({geometry_path, id});
        COLD_TRACE("Geometry created with path %s and id %d", geometry_path, id);
        return id;
    }

    GeometryId GeometrySystem::generate_id()
    {
        static GeometryId id = 0;
        return id++; 
    }

    void GeometrySystem::pass_data_to_geometry(GeometryId id, const GeommetryConfig &geometry_config)
    {
        COLD_ASSERT(instance->geometries.count(id) > 0, "No Geometry found in the map");
        GeometrySPtr geometry = instance->geometries.at(id);
        geometry->push_vertex_data(geometry_config.verticies, geometry_config.vertex_count, 
                                   geometry_config.normals, geometry_config.normal_count,
                                   geometry_config.tex_coords, geometry_config.tex_coords_count);
        geometry->push_indicies(geometry_config.faces, geometry_config.faces_count);
    }

    void GeometrySystem::buffer_geometry_data_to_gpu(GeometryId id)
    {
        COLD_ASSERT(instance->geometries.count(id) > 0, "No Geometry found in the map");
        GeometrySPtr geometry = instance->geometries.at(id);
        geometry->buffer_data_to_gpu();

    }

    void GeometrySystem::render_geometry(GeometryId id)
    {
        COLD_ASSERT(instance->geometries.count(id) > 0, "No Geometry found in the map");
        GeometrySPtr geometry = instance->geometries.at(id);
        geometry->render();
    }

    bool GeometrySystem::delete_geometry(GeometryId id)
    {
        COLD_ASSERT(instance->geometries.count(id) > 0, "No Geometry found in the map");
        auto geometry_ref = instance->geometries[id];
        geometry_ref->decreament_ref_count();
        if(geometry_ref->get_ref_count() == 0) {
            geometry_ref.reset();
        }
        return false;
    }

    /* NO in use this function*/
    bool GeometrySystem::should_load_geometry_data(const std::string &geometry_path)
    {
        if(instance->geometry_path_refs.count(geometry_path)) {
            return false;
        }
        return true;
    }

    void GeometrySystem::set_material(GeometryId id, const Material &material)
    {
        COLD_ASSERT(instance->geometries.count(id) > 0, "No Geometry found in the map");
        GeometrySPtr geometry = instance->geometries.at(id);
        geometry->set_material(material);

    }

    TransformSPtr GeometrySystem::get_transform(GeometryId id)
    {
        COLD_ASSERT(instance->geometries.count(id) > 0, "No Geometry found in the map");
        GeometrySPtr geometry = instance->geometries.at(id);
        return geometry->get_transform();
    }

    GeometrySystem::~GeometrySystem()
    {
        for(auto id : geometries) {
            id.second.reset();
        }
    }
}