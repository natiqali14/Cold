#include "BackendUtil.h"

#include "../test_data.h"
namespace Cold {
    namespace BackendUtil {
        StaticMesh* create_sqaure() 
        {
            std::vector<Cold::Vertex> vertices;
            for(u32 i = 0; i < 36; i ++) {
                Cold::Vertex v;
                v.position.x = vertices_with_tc[(i * 5) + 0];
                v.position.y = vertices_with_tc[(i * 5) + 1];
                v.position.z = vertices_with_tc[(i * 5) + 2];
                v.tex_coord.x = vertices_with_tc[(i * 5) + 3];
                v.tex_coord.y = vertices_with_tc[(i * 5) + 4];
                vertices.push_back(v);
            }

            Cold::TransformSPtr sqaur_trans = std::make_shared<Cold::Transform>();
            Cold::StaticMesh* sqaure_mesh = new Cold::StaticMesh(sqaur_trans, "");
            
            sqaur_trans->scale({5.0,5.0,5.0});
            sqaur_trans->translate({0,5,0});
            Cold::GeometryId square_geom = Cold::GeometrySystem::create_geometry("sqaure_my");
            Cold::GeometrySystem::set_geometry_parent_transform(square_geom, sqaur_trans);
            Cold::GeometrySystem::pass_data_to_geometry(square_geom, vertices);
            Cold::Material sqaure_material;
            sqaure_material.diff_tex = "Assets/default.png";
            Cold::GeometrySystem::set_material(square_geom, sqaure_material);
            sqaure_mesh->push_geometry(square_geom);
            sqaure_mesh->buffer_to_gpu();

            return sqaure_mesh;
        }
    }
}