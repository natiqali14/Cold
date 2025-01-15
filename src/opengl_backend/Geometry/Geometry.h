#pragma once
#include <includes.h>
#include <glad/glad.h>
#include <VertexBuffer.h>
#include <VertexArrayBuffer.h>
#include <assimp/scene.h>
#include <Transforms.h>
#include <VertexStructures.h>
#include <ShaderSystem.h>
#include <mutex>
namespace Cold {
    class Geometry {
    public:
        Geometry(GLenum geometry_usage);
        void push_vertex(const Vertex& vertex_data);
        void push_index(u32 index);

        void push_vertex_data(aiVector3D* vertices, u32 vertex_count,
                              aiVector3D* normals, u32 normal_count,
                              aiVector3D* tex_coords, u32 tex_coords_count);
        void push_vertex_data(const std::vector<Vertex> &vert_data);
        void push_indicies(aiFace* faces, u32 face_count);
        void push_inicies(const std::vector<u32>& index_data);
        void buffer_data_to_gpu();
        void delete_data_from_gpu();
        void buffer_material_data();

        void render(TransformSPtr static_mesh_root);
        void set_material(const Material& material);
        void change_material(const Material& material);
        TransformSPtr get_transform();
        inline const u32 get_ref_count() const {return ref_count;}
        inline void increament_ref_count() {ref_count++;}
        inline void decreament_ref_count() {ref_count--;}

        ~Geometry();
    private:
        /** 
         * gpu vertex data for a geometry 
        */
        std::vector<Vertex> verticies;
        i32 total_vertex_count;
        u32 ref_count;
        /* unique VAO for this geometry  */
        VertexArrayBufferSPtr vao;
        Material geometry_material;
         
        TransformSPtr transform;
        GLenum usage;

        /* ebo data */
        u32 ebo;
        std::vector<u32> indicies;

        // shaders
        std::string shader;

        std::mutex mtx;

    };



}