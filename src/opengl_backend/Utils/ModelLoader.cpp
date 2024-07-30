#pragma once
#include "ModelLoader.h"
#include <filesystem>
#include <Logger.h>
#include <Transforms.h>
#include <GeometrySystem.h>
#include <sstream>
namespace Cold {
    static ModelLoader* instance = nullptr;
    void ModelLoader::initiate()
    {
        instance = new ModelLoader;
    }

    void ModelLoader::shutdown()
    {
    }

    Mesh* ModelLoader::model_load(const std::string &file_path, u32 flags)
    {
        GeometrySystem::initiate(); // TODO move this
        TransformSPtr root_t = std::make_shared<Transform>();
        Mesh* static_mesh = new Mesh(root_t);
        std::filesystem::path cwd = std::filesystem::current_path();
        cwd = cwd / file_path;
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(cwd.string(), flags);
        COLD_TRACE("Sponze Root name %s", scene->mRootNode->mName.C_Str());
        auto root = scene->mRootNode;
        u32 root_child = root->mNumChildren;
        COLD_TRACE("Sponza root has childs %d", root_child);
          int l = 1;
        for(u32 i = 0; i < root_child; i++) {
            aiString p;
            auto mesh_id = root->mChildren[i]->mMeshes;
            auto n_mesh = root->mChildren[i]->mNumMeshes;
            if(n_mesh > 0)
            {
               
                auto mesh =  scene->mMeshes[mesh_id[0]];
                // TODO to check if name can be empty or not
                aiString name = mesh->mName;
                std::string geom_path = file_path + std::string(name.C_Str());
                GeometryId geometry = GeometrySystem::create_geometry(geom_path);// geometry creation
                static_mesh->push_geometry(geometry); // adding geom to mesh
                GeometrySystem::get_material_transform(geometry)->set_parent(root_t);
                auto mat = mesh->mMaterialIndex;
                
                aiString path;
                scene->mMaterials[mat]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
                auto count = scene->mMaterials[mat]->GetTextureCount(aiTextureType_DIFFUSE);
                std::stringstream texture_path;
                if(count > 0) {
                    texture_path << "Assets/sponza/";
                    texture_path << path.C_Str();
                }
                else {
                    texture_path << "Assets/default.png";
                }
                
                
                Material mater;
                mater.diff_tex = texture_path.str(); // diffusion texture path final
                GeometrySystem::set_material(geometry, mater); // setting material for our geometry

                /* Setting vertex data and index data for geometry*/
                u32 total_vertex_count = mesh->mNumVertices;
                aiVector3D* vertices = mesh->mVertices;
                aiVector3D* normals = mesh->mNormals;
                aiVector3D* tcs = mesh->mTextureCoords[0];
                u32 total_faces = mesh->mNumFaces;

                GeommetryConfig config;
                config.verticies = vertices;
                config.vertex_count = total_vertex_count;
                config.normals = normals;
                config.normal_count = total_vertex_count;
                config.tex_coords = tcs;
                config.tex_coords_count = total_vertex_count;
                config.faces = mesh->mFaces;
                config.faces_count = total_faces;
                GeometrySystem::pass_data_to_geometry(geometry, config);
            

              // COLD_TRACE("Texture name %s %d", path.C_Str(), l); l++;

                
            }            
        }
        return static_mesh;

    }
}