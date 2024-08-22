#include "ModelLoader.h"
#include <filesystem>
#include <Logger.h>
#include <Transforms.h>
#include <GeometrySystem.h>
#include <sstream>
#include <helper.h>
#include <Geometry.h>
namespace Cold {
    static ModelLoader* instance = nullptr;
    void ModelLoader::initiate()
    {
        instance = new ModelLoader;
    }

    void ModelLoader::shutdown()
    {
    }

     /** This function will do following things
            * 1. checks if the path is starting from Assets/Models/
            * 2. check if it has a extension or not
            * 3. check if extension is obj or not
            * 4. also returns the sub path for textures staring from Assets/Models/     
    */
    static std::string helper_verification_for_model_loader(const std::string& file_path) {
        std::string targert_directory = Helper::normalize_paths("Assets/Models/") ;
        u64 pos = file_path.find(targert_directory);
        COLD_ASSERT(pos == 0, "ERROR target  model is not in directory Assets/Models");
        u64 obj_file_check = file_path.rfind("."); // TODO right now only .obj is tested and being supported
        COLD_ASSERT(obj_file_check != std::string::npos, "No file extension is provided");
        std::string extension = file_path.substr(obj_file_check+1, file_path.length());
        COLD_ASSERT(strcmp(extension.c_str(), "obj") == 0 || strcmp(extension.c_str(), "fbx") == 0 , 
                            "Provided file is not of obj or fbx extension");
        u64 split_pos = file_path.rfind(Helper::normalize_paths("/"));
        return file_path.substr(0,split_pos+1);
    }

    static void  prepare_vertex3D_data(aiVector3D* vertices, aiVector3D* tex_coords, aiVector3D* normals, aiVector3D* tangents, aiVector3D* biTangents, u32 vertex_count ,std::vector<Vertex>& data) {
        Vertex v;
        for(u32 i = 0; i < vertex_count; i++) {
            aiVector3D vert = vertices[i];
            aiVector3D tex = tex_coords[i];
            aiVector3D norm = normals[i];
            aiVector3D tang = tangents[i];
            aiVector3D biTang = biTangents[i];
            v.position = {vert.x, vert.y, vert.z};
            v.normals = {norm.x, norm.y, norm.z};
            v.tex_coord = {tex.x, tex.y};
            v.tangents = {tang.x, tang.y, tang.z};
            v.bitangents = {biTang.x, biTang.y, biTang.z};
            data.push_back(v);
        }
    }
    static void prepare_index_data(aiFace* faces, u32 face_count,  std::vector<u32>& data) {
        for(u32 i = 0; i < face_count; i++) {
            aiFace face = faces[i];
            for(u8 j = 0; j < face.mNumIndices;  j++) {
                face.mIndices[j];
                data.push_back(face.mIndices[j]);
            }
        }
    }

    static void set_diffusion_info(Material& mater, aiMaterial* material, const std::string& texture_dir) {
        auto count = material->GetTextureCount(aiTextureType_DIFFUSE);
        std::stringstream texture_path;
        // if yes then make a full path of this texture
        if(count > 0) {
            aiString path;  // path for diffuse texture
            material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
            texture_path << texture_dir;
            texture_path << path.C_Str();
        }
        // if not then use default texture
        else {
            texture_path << Helper::normalize_paths("Assets/default.png");
        }

        mater.diff_tex = texture_path.str();

        aiColor3D diffuse_color;
        auto status = material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color);
        if(status == AI_SUCCESS) {
            mater.diffuse_color = {diffuse_color.r, diffuse_color.g, diffuse_color.b};
        }
    }

    static void set_specular_info(Material& mater, aiMaterial* material, const std::string& texture_dir) {
        std::stringstream specular_path;
        auto count = material->GetTextureCount(aiTextureType_SPECULAR);
        if(count > 0) 
        {
            aiString path;  // path for diffuse texture
            material->GetTexture(aiTextureType_SPECULAR, 0, &path);
            specular_path << texture_dir;
            specular_path << path.C_Str();
            mater.specular_texure = specular_path.str();
            f32 shininess;
            if (AI_SUCCESS == material->Get(AI_MATKEY_SHININESS, shininess)) 
            {
                mater.shininess = shininess;
            }
        }
    }

    static void set_normal_info(Material& mater, aiMaterial* material, const std::string& texture_dir) {
        std::stringstream normal_path;
        auto count = material->GetTextureCount(aiTextureType_HEIGHT);
        if(count > 0)
        {
            aiString path;
            material->GetTexture(aiTextureType_HEIGHT, 0, &path);
            normal_path << texture_dir;
            normal_path << path.C_Str();
            mater.normal_texture = normal_path.str();
        }
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
                std::string geom_path =  std::string(name.C_Str());
                // geom creation
                GeometryId geometry = GeometrySystem::create_geometry(geom_path);// geometry creation
                static_mesh->push_geometry(geometry); // adding geom to mesh
                // setting parent root transform
                GeometrySystem::get_transform(geometry)->set_parent(static_mesh->get_transform());
                // getting material index for this geom
                auto mat = mesh->mMaterialIndex;
                // creation of material object for geom
                Material mater;

                // for diffusion
                set_diffusion_info(mater, scene->mMaterials[mat], texture_dir);

                // for specular 
                set_specular_info(mater,scene->mMaterials[mat], texture_dir);

                // for normals
                set_normal_info(mater, scene->mMaterials[mat], texture_dir);
                
                GeometrySystem::set_material(geometry, mater); // setting material for our geometry

                /* Setting vertex data and index data for geometry*/
                u32 total_vertex_count = mesh->mNumVertices;
                aiVector3D* vertices = mesh->mVertices;
                aiVector3D* normals = mesh->mNormals;
                aiVector3D* t = mesh->mTangents;
                aiVector3D* b = mesh->mBitangents;
                aiVector3D* tcs = mesh->mTextureCoords[0];
                u32 total_faces = mesh->mNumFaces;
                std::vector<Vertex> vertices_data;
                prepare_vertex3D_data(vertices, tcs, normals, t, b, total_vertex_count, vertices_data);
                std::vector<u32> index_data;
                prepare_index_data(mesh->mFaces, total_faces, index_data);
                GeometrySystem::pass_data_to_geometry(geometry, vertices_data);
                GeometrySystem::pass_indicies_data_to_geometry(geometry, index_data);
            }            
        }

        importer.FreeScene();
    }
}