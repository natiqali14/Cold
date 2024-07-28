#pragma once
#include <includes.h>
#include <glad/glad.h>
#include <VertexBuffer.h>
#include <VertexArrayBuffer.h>
#include <assimp/scene.h>
namespace Cold {

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normals;
        glm::vec2 tex_coord; 
    };

    
    class Geometry {
    public:
        Geometry(GLenum geometry_usage);
        void push_vertex(const Vertex& vertex_data);
        void push_index(u32 index);

        void push_vertex_data(aiVector3D* vertices, u32 vertex_count,
                              aiVector3D* normals, u32 normal_count,
                              aiVector2D* tex_coords, u32 tex_coords_count);
        void push_indicies(aiFace* faces, u32 face_count);
        void buffer_data_to_gpu();
        void delete_data_from_gpu();

        void render();
        inline const u32 get_ref_count() const {return ref_count;}
        inline void increament_ref_count() {ref_count++;}
        inline void decreament_ref_count() {ref_count--;}
        ~Geometry();
    private:
        /** 
         * gpu vertex data for a geometry
         * can have vertex data or some thing else like normal maps, texture co-oridnates etc
        */
        std::vector<VertexBufferSPtr> vbo_vector;
         std::vector<Vertex> verticies;
        /* unique VAO for this geometry  */
        VertexArrayBufferSPtr vao;
        /* ebo data */
        u32 ebo;
        std::vector<u32> indicies;
        GLenum usage;
        u32 total_vertex_count;
        u32 ref_count;
    };



}