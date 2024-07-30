
// This is just a testing file for opengl, will be deleted
#pragma once
#include "../includes/glad/glad.h"
#include "../comman_data.h"
#include "../includes/GLFW/glfw3.h"

#include "../comman_data.h"
#include "../CameraSystem/CameraSystem.h"
#include "buffers/VertexArrayBuffer.h"
#include "buffers/Vertexbuffer.h"
#include "test_data.h"
#include "../Utility/Logger.h"
#include <TextureSystem.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>
#include <sstream>
#include <ModelLoader.h>
#include <StaticMesh.h>
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 aNormal;\n"
                                 "layout (location = 2) in vec2 aTexCoord;\n"
                                 "uniform mat4 model;\n"
                                 "uniform mat4 view;\n"
                                 "uniform mat4 p;\n"
                                 "out vec2 tc;\n"
                                 "out vec4 v_col;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = p * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                    "   v_col = vec4(aPos.x + 0.3, aPos.y + 0.5, 1.0, 1.0);\n"
                                 "   tc = aTexCoord;\n"
                                 "}\0";

const char *vertexMultiInstanceSource = "#version 330 core\n"
                                        "layout (location = 0) in vec3 aPos;\n"
                                        "layout (location = 1) in vec3 aCol;\n"
                                        "uniform mat4 model;\n"
                                        "uniform mat4 view;\n"
                                        "uniform mat4 p;\n"
                                        "uniform float time;\n"
                                        "out vec4 v_col;\n"
                                        "mat4 buildTranslate(float x, float y, float z);\n"
                                        "mat4 buildScale(float x, float y, float z);\n"
                                        "mat4 buildRotate(float x, float y, float z);\n"
                                        "float hash(float n) {\n"
                                        "return sin(fract(sin(n) * 43758.5453));\n"
                                        "}\n"
                                        "void main()\n"
                                        "{\n"
                                        "   float i = float(gl_InstanceID) ;\n"
                                        "   float seed = float(i)  * 0.1;\n"
                                        "   float randX = hash(seed);\n"
                                        "   float randY = hash(seed * 2.0);\n"
                                        "   float randZ = hash(seed * 3.0); \n"
                                        "   float x = sin(400.0 * i/8000.) * 140.0;\n"
                                        "   float y = sin(300 * i/2000) * 100.0;\n"
                                        "   float z = sin(600* i/9203) * 150.0;\n"
                                        "float a = sin(203.0 * i/8000.0) * 403.0;\n"
                                        "float b = cos(301.0 * i/4001.0) * 401.0;\n"
                                        "float c = sin(400.0 * i/6003.0) * 405.0;\n"
                                        "   mat4 trans = buildTranslate(a,b,c);\n"
                                        "   mat4 scale = buildScale(randX * 10.,randX * 10.,randX* 10.);\n"
                                        "   gl_Position = p * view * trans * scale * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                        "   v_col = vec4(vec3(aCol.x, aCol.y, randX),1.0);\n"
                                        "}\n"
                                        "mat4 buildTranslate(float x, float y, float z) {\n"
                                        "mat4 trans = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,x, y, z, 1.0 );\n"
                                        "return trans;\n}"
                                        "mat4 buildScale(float x, float y, float z) {\n"
                                        "mat4 trans = mat4(x, 0.0, 0.0, 0.0, 0.0, y, 0.0, 0.0, 0.0, 0.0, z, 0.0,0., 0., 0., 1.0 );\n"
                                        "return trans;\n}"
                                        "mat4 buildRotate(float x, float y, float z) {\n"
                                        "mat4 trans = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,x, y, z, 1.0 );\n"
                                        "return trans;\n}";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec2 tc;\n"
                                   "in vec4 v_col;\n"
                                   "uniform sampler2D frameTexture;\n"
                                   "void main()\n"
                                   "{\n"
                                    "  vec4 col = texture(frameTexture, tc);\n"
                                   "   FragColor = vec4(col);\n"
                                   "}\n\0";

u32 create_shader(GLenum shader_type, const char *shader_code)
{
    u32 shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_code, NULL);
    glCompileShader(shader);
    i32 s;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &s);
    if (!s)
    {
        char info_log[1024];
        glGetShaderInfoLog(shader, 1024, NULL, info_log);
        COLD_ERROR("Shader compilation failed %s", info_log);
        std::cout << "Shader compilation failed. \n"
                  << info_log << "\n";
    }
    return shader;
}
u32 create_program(std::vector<u32> &shaders_id)
{
    if (shaders_id.size() < 2)
        throw std::runtime_error("not enough shader for opengl program");
    u32 program = glCreateProgram();
    glAttachShader(program, shaders_id[0]);
    glAttachShader(program, shaders_id[1]);
    glLinkProgram(program);
    i32 s;
    glGetProgramiv(program, GL_LINK_STATUS, &s);
    if (!s)
    {
        char info[512];
        glGetProgramInfoLog(program, 512, NULL, info);
        std::cout << "program linking failed. \n"
                  << info << "\n";
    }
    glDeleteShader(shaders_id[0]);
    glDeleteShader(shaders_id[1]);

    Cold::v_data.program_id = program;

    return program;
}

void transformations(u32 shader_program)
{


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Cold::v_data.tex_id);
    u32 loci = glGetUniformLocation(shader_program, "frameTexture");
    glUniform1i(loci, 0);
    glBindTexture(GL_TEXTURE_2D, 0);


    f64 time = glfwGetTime();
    f32 translation = (time) * 70;
    glm::mat4 model = glm::mat4(1.0);
    glm::mat4 view = glm::mat4(1.0);
    model = glm::rotate(model, glm::radians(translation), glm::vec3(01.0f, 0.5, .50));
    // model = glm::translate(model, glm::vec3(0.2 * glm::sin(translation),0.0,0.0));
    view = glm::translate(view, glm::vec3(0, 01.0f, -100.0f));
    // view = glm::scale(view,glm::vec3(100.0f));
    glm::mat4 p = glm::perspective(45.0f, 800.f / 600.f, 0.1f, 10000.f);

    u32 loc = glGetUniformLocation(shader_program, "model");
    glUseProgram(shader_program);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));

    u32 loc2 = glGetUniformLocation(shader_program, "view");
    glUseProgram(shader_program);
    glUniformMatrix4fv(loc2, 1, GL_FALSE, glm::value_ptr(view));

    u32 loc3 = glGetUniformLocation(shader_program, "p");
    glUseProgram(shader_program);
    glUniformMatrix4fv(loc3, 1, GL_FALSE, glm::value_ptr(p));

    u32 time_loc = glGetUniformLocation(shader_program, "time");
    glUseProgram(shader_program);
    glUniform1f(time_loc, time);
}

void render_cubes(u32 shader_program)
{
    static CameraSystem *cs = new CameraSystem;
//   //  glBindTexture(GL_TEXTURE_2D, Cold::v_data.tex_id);
//     //  std::cout << cs->camera_data.position.z << " " << cs->camera_data.position.x << " ";
//     glBindTexture(GL_TEXTURE_2D, Cold::v_data.tex_id);
//     glm::mat4 vM = cs->get_camera_view_space(); // glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f,-10.0));
//     f64 time = glfwGetTime();
//     f32 translation = (time) * 70;
//     glm::mat4 model_1 = glm::mat4(1.0);// glm::rotate(glm::mat4(1.0), glm::radians(translation), glm::vec3(0.0, 1.0, 0.0));

//     u32 loc_1 = glGetUniformLocation(shader_program, "view");
//     glUseProgram(shader_program);
//     glUniformMatrix4fv(loc_1, 1, GL_FALSE, glm::value_ptr(vM));

//     u32 loc_2 = glGetUniformLocation(shader_program, "model");
//     glUseProgram(shader_program);
//     glUniformMatrix4fv(loc_2, 1, GL_FALSE, glm::value_ptr(model_1));

//     glm::mat4 p = glm::perspective(45.0f, 800.f / 600.f, 0.1f, 200.f);
//     u32 loc3 = glGetUniformLocation(shader_program, "p");
//     glUseProgram(shader_program);
//     glUniformMatrix4fv(loc3, 1, GL_FALSE, glm::value_ptr(p));

//     // glBindVertexArray(cube_1.vao);
//     Cold::v_data.vertex_array_buffers[0]->bind();
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Cold::v_data.ebo_id);
//     glDrawElements(GL_TRIANGLES, 25968,GL_UNSIGNED_INT ,0);
    // glBindBuffer(GL_ARRAY_BUFFER, cube_1.vbo);
    //glDrawArrays(GL_TRIANGLES, 0, 259683);
    

    // glm::mat4 model_2 = glm::translate(glm::mat4(1.0), glm::vec3(5.0f, 0.0f, 0.0f));
    // model_2 = glm::rotate(model_2, glm::radians(translation), glm::vec3(1.0, 0.0, 0.0));
    // glm::mat4 vM_2 = vM * model_1;

    // u32 loc_4 = glGetUniformLocation(shader_program, "view");
    // glUseProgram(shader_program);
    // glUniformMatrix4fv(loc_4, 1, GL_FALSE, glm::value_ptr(vM_2));

    // u32 loc_5 = glGetUniformLocation(shader_program, "model");
    // glUseProgram(shader_program);
    // glUniformMatrix4fv(loc_5, 1, GL_FALSE, glm::value_ptr(model_2));

    // glDrawArrays(GL_TRIANGLES, 0, 36);




    Cold::v_data.cs = cs;
   if(Cold::v_data.m && Cold::v_data.cs) {
    Cold::v_data.m->render();
   }

}

void assimp_testing() 
{
    Assimp::Importer importer;
    std::filesystem::path cwd = std::filesystem::current_path();
    std::string path = "Assets/falcon/falcon.obj";
    cwd = cwd / path;
    const aiScene* cottage =   importer.ReadFile(cwd.string(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
    COLD_ASSERT(cottage != nullptr, "Can not open Obj file");

    aiNode* root = cottage->mRootNode;
    
    COLD_TRACE("Cottage assets has Meshes %d, Materials %d, Textures %d, childeren %d t %d", 
    cottage->mNumMeshes, cottage->mNumMaterials, cottage->mNumTextures, root->mNumChildren);
    aiString t_path;
    cottage->mMaterials[1]->GetTexture(aiTextureType_DIFFUSE, 0, &t_path);
    aiMesh* cottage_mesh = nullptr;
    if(cottage->mMeshes) {
        for(int i = 0; i < cottage->mNumMeshes ;  i++) {
            std::string mesh_name = cottage->mMeshes[i]->mName.C_Str();
            if(strcmp(mesh_name.c_str(), "Plane_Plane.001") == 0) {
                cottage_mesh = cottage->mMeshes[i];
            }
        }
    }
    if (cottage_mesh == nullptr) return;
    COLD_TRACE("GOT COTTAGE MESH has a diffuse map %s", t_path.C_Str());
    std::stringstream texture_path;
    texture_path << "Assets/falcon/";
    texture_path << t_path.C_Str();
    aiVector3D* cottage_vertex_data = cottage_mesh->mVertices;
    aiVector3D* norms = cottage_mesh->mNormals;
    aiVector3D* cottage_tc_data = cottage_mesh->mTextureCoords[0];
    u32 total_vertices = cottage_mesh->mNumVertices;
   

    std::vector<float> vertex_data;
    // vertex_data.resize((total_vertices * 3));
    for(u32 i = 0; i < total_vertices; i ++) {
        vertex_data.push_back(cottage_vertex_data[i].x);
        vertex_data.push_back(cottage_vertex_data[i].y);
        vertex_data.push_back(cottage_vertex_data[i].z);
        vertex_data.push_back(norms[i].x);
        vertex_data.push_back(norms[i].y);
        vertex_data.push_back(norms[i].z);
        vertex_data.push_back(cottage_tc_data[i].x);
        vertex_data.push_back(cottage_tc_data[i].y);
    }

    Cold::VertexBufferSPtr cottage_vert_buffer = Cold::VertexBuffer::create_vertex_buffer(
        vertex_data.data(),
        sizeof(float) * vertex_data.size(),
        GL_DYNAMIC_DRAW,
        {
            {"aPos", 3, GL_FLOAT, false, sizeof(float) * 8, 0}, 
             {"aNormal", 3, GL_FLOAT, false, sizeof(float) * 8, sizeof(float) * 3},
            {"aTexCoord", 2, GL_FLOAT, false, sizeof(float) * 8, sizeof(float) * 6}
        }
    );
    Cold::VertexArrayBufferSPtr cottage_vao = Cold::VertexArrayBuffer::create_vertex_array_buffer();
    cottage_vao->push_vertex_buffers({cottage_vert_buffer});
    Cold::v_data.vertex_array_buffers.push_back(cottage_vao);

    auto u_id = Cold::TextureSystem::texture_2D_immutable_create(texture_path.str(),
    {GL_RGBA, true, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_UNSIGNED_BYTE});
    
    u32 total_indices = cottage_mesh->mNumFaces;
    std::vector<u32> faces;
    for(u32 i = 0; i < total_indices; i++) {
        aiFace face = cottage_mesh->mFaces[i];
        faces.push_back(face.mIndices[0]);
        faces.push_back(face.mIndices[1]);
        faces.push_back(face.mIndices[2]);
    }
    COLD_TRACE("Cottage Meshh total Vertices %d %d", cottage_mesh->mNumVertices, cottage_mesh->mNumFaces);
    u32 ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * faces.size(), faces.data(), GL_DYNAMIC_DRAW);
    Cold::v_data.ebo_id = ebo;


    Cold::v_data.tex_id = u_id;
    glBindTexture(GL_TEXTURE_2D, Cold::v_data.tex_id);



    // asimp model loader testing

   auto sm =  Cold::ModelLoader::model_load("Assets/sponza/sponza.obj",aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
    sm->buffer_to_gpu();
    Cold::v_data.m = sm;
    auto sm_transform = sm->get_transform();
    sm_transform->scale({0.05,0.05,0.05});
    sm_transform->translate({100,0,0});

}

void initialise_triangle()
{

    Cold::VertexBufferSPtr v_buffer_1 =
            Cold::VertexBuffer::create_vertex_buffer(
                vertices_with_tc,
                sizeof(vertices_with_tc),
                GL_DYNAMIC_DRAW,
                {
                    {"aPos", 3, GL_FLOAT, false, sizeof(float) * 5, 0},                      // for vertices
                    {"aTexCoord", 2, GL_FLOAT, false, sizeof(float) * 5, sizeof(float) * 3} // for tc's
                });


    auto v_array_buffer = 
        Cold::VertexArrayBuffer::create_vertex_array_buffer();
    v_array_buffer->push_vertex_buffers({v_buffer_1});
  //  v_array_buffer->push_vertex_buffer(v_buffer_2);
    // data making
    Cold::TextureSystem::initiate();
    // u32 u_id = Cold::TextureSystem::texture_2D_immutable_create("Assets/image4.jpeg", 
    // {GL_RGB, true, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_UNSIGNED_BYTE});

  //  Cold::v_data.vertex_array_buffers.push_back(v_array_buffer);
  //  Cold::v_data.tex_id = u_id;
   // glBindTexture(GL_TEXTURE_2D, Cold::v_data.tex_id);
  //  assimp_testing();
    // u32 vbo;
    // u32 vao;
    // u32 ebo;
    // glGenVertexArrays(1, &vao);
    // glBindVertexArray(vao);

    // glGenBuffers(1, &vbo);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 180, vertices, GL_DYNAMIC_DRAW);
    // glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float) * 3,(void*) 0);
    // glEnableVertexAttribArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER,0);
    // glGenBuffers(1,&ebo);
    // glBindBuffer(GL_ARRAY_BUFFER, ebo);
    // glBufferData(GL_ARRAY_BUFFER,sizeof(texcoord), texcoord, GL_DYNAMIC_DRAW);
    // glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE, sizeof(float) * 2, (void*) (0));
    // glEnableVertexAttribArray(1);
    // glBindBuffer(GL_ARRAY_BUFFER,0);
    // glBindVertexArray(0);
    // buffer_data data = {vbo,vao,ebo};
    // glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LEQUAL);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);
    // glFrontFace(GL_CW);

auto sm =  Cold::ModelLoader::model_load("Assets/sponza/sponza.obj",aiProcess_Triangulate | aiProcess_FlipUVs );
    sm->buffer_to_gpu();
    Cold::v_data.m = sm;
    return;
}

// void draw_triangle(buffer_data &data) {
//     glBindVertexArray(data.vao);
//     glBindBuffer(GL_ARRAY_BUFFER, data.vbo);
//     glDrawArrays(GL_TRIANGLES, 0, 36);
//     glBindVertexArray(0);
//     glBindBuffer(GL_ARRAY_BUFFER, 0);
// }
