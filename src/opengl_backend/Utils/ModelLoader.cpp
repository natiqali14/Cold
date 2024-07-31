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

     /* This function will do following things
            * 1. checks if the path is starting from Assets/Models/
            * 2. check if it has a extension or not
            * 3. check if extension is obj or not
            * 4. also returns the sub path for textures staring from Assets/Models/     
    */
    static std::string helper_verification_for_model_loader(const std::string& file_path) {
        std::string targert_directory = "Assets/Models/";
        u64 pos = file_path.find("Assets/Models/");
        COLD_ASSERT(pos == 0, "ERROR target  model is not in directory Assets/Models");
        u64 obj_file_check = file_path.rfind("."); // TODO right now only .obj is tested and being supported
        COLD_ASSERT(obj_file_check != std::string::npos, "No file extension is provided");
        COLD_ASSERT((file_path.length() - obj_file_check) ==  3 + 1, "Provided file is not of obj extension");
        u64 split_pos = file_path.rfind("/");
        return file_path.substr(0,split_pos+1);
    }

    void  ModelLoader::model_load(const std::string &file_path, u32 flags, StaticMesh* static_mesh)
    {
        std::string texture_dir = helper_verification_for_model_loader(file_path);
        std::filesystem::path cwd = std::filesystem::current_path();
        cwd = cwd / file_path;
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(cwd.string(), flags);
        auto root = scene->mRootNode;
        u32 root_child = root->mNumChildren;
          int l = 1;
        for(u32 i = 0; i < root_child; i++) {
            aiString p;
            auto mesh_id = root->mChildren[i]->mMeshes;
            auto n_mesh = root->mChildren[i]->mNumMeshes;
            if(n_mesh > 0)
            {
               
                auto mesh =  scene->mMeshes[mesh_id[0]];
                // TODO to check if name can be empty or not
                // creation of unique path for geometry
                aiString name = mesh->mName;
                std::string geom_path = file_path + std::string(name.C_Str());
                // geom creation
                GeometryId geometry = GeometrySystem::create_geometry(geom_path);// geometry creation
                static_mesh->push_geometry(geometry); // adding geom to mesh
                // setting parent root transform
                GeometrySystem::get_transform(geometry)->set_parent(static_mesh->get_transform());
                // getting material index for this geom
                auto mat = mesh->mMaterialIndex;
                aiString path;  // path for diffuse texture
                scene->mMaterials[mat]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
                // checking if it has diffuse texture or not
                auto count = scene->mMaterials[mat]->GetTextureCount(aiTextureType_DIFFUSE);
                std::stringstream texture_path;
                // if yes then make a full path of this texture
                if(count > 0) {
                    texture_path << texture_dir;
                    texture_path << path.C_Str();
                }
                // if not then use default texture
                else {
                    texture_path << "Assets/default.png";
                }
                // creation of material object for geom
                Material mater;
                mater.diff_tex = texture_path.str(); // diffusion texture path final
                GeometrySystem::set_material(geometry, mater); // setting material for our geometry

                /* Setting vertex data and index data for geometry*/
                u32 total_vertex_count = mesh->mNumVertices;
                aiVector3D* vertices = mesh->mVertices;
                aiVector3D* normals = mesh->mNormals;
                aiVector3D* tcs = mesh->mTextureCoords[0];
                u32 total_faces = mesh->mNumFaces;

                GeommetryConfig config; // geom config
                config.verticies = vertices;
                config.vertex_count = total_vertex_count;
                config.normals = normals;
                config.normal_count = total_vertex_count;
                config.tex_coords = tcs;
                config.tex_coords_count = total_vertex_count;
                config.faces = mesh->mFaces;
                config.faces_count = total_faces;
                GeometrySystem::pass_data_to_geometry(geometry, config);
            }            
        }

        importer.FreeScene();
    }
}