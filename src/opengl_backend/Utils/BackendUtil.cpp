#include "BackendUtil.h"

#include "../test_data.h"
#include <BackendConfigData.h>
namespace Cold {
    namespace BackendUtil {
        StaticMesh* create_sqaure() 
        {
            std::vector<Cold::Vertex> vertices (36 * 8);
            std::vector<u32> faces(36);
            for(u32 i = 0; i < 36; i ++) 
            {
                Cold::Vertex v;
                v.position.x = vertices_with_tc_and_normals[(i * 8) + 0];
                v.position.y = vertices_with_tc_and_normals[(i * 8) + 1];
                v.position.z = vertices_with_tc_and_normals[(i * 8) + 2];

                v.normals.x = vertices_with_tc_and_normals[(i * 8) + 3];
                v.normals.y = vertices_with_tc_and_normals[(i * 8) + 4];
                v.normals.z = vertices_with_tc_and_normals[(i * 8) + 5];

                v.tex_coord.x = vertices_with_tc_and_normals[(i * 8) + 6];
                v.tex_coord.y = vertices_with_tc_and_normals[(i * 8) + 7];
                vertices.push_back(v);
                faces.push_back(cube_indices[i]);
            }

            Cold::TransformSPtr sqaur_trans = std::make_shared<Cold::Transform>();
            Cold::StaticMesh* sqaure_mesh = new Cold::StaticMesh(sqaur_trans, "");
            
            sqaur_trans->scale({5.0,5.0,5.0});
            sqaur_trans->translate({0,3,0});

            Cold::GeometryId square_geom = Cold::GeometrySystem::create_geometry("sqaure_my");
            Cold::GeometrySystem::set_geometry_parent_transform(square_geom, sqaur_trans);
            Cold::GeometrySystem::pass_data_to_geometry(square_geom, vertices);
            Cold::GeometrySystem::pass_indicies_data_to_geometry(square_geom, faces);
            
            Cold::Material sqaure_material;
            sqaure_material.diff_tex = cobble_stone.diffuse;
            sqaure_material.specular_texure = cobble_stone.specular;
            sqaure_material.shininess = cobble_stone.shininess;
            Cold::GeometrySystem::set_material(square_geom, sqaure_material);
            sqaure_mesh->push_geometry(square_geom);

            return sqaure_mesh;
        }
    }
}