#pragma once
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
            return instance->geometry_path_refs[geometry_path];
        }
        GeometryId id = generate_id();
        GeometrySPtr geometry_ptr = std::make_shared<Geometry>(GL_STATIC_DRAW);
        instance->geometries.insert({id, geometry_ptr});
        instance->geometry_path_refs.insert({geometry_path, id});
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
        // TODO here fill it
        return false;
    }

    bool GeometrySystem::should_load_geometry_data(const std::string &geometry_path)
    {
        if(instance->geometry_path_refs.count(geometry_path)) {
            return false;
        }
        return true;
    }

    GeometrySystem::~GeometrySystem()
    {
        // TODO here fill it
    }
}