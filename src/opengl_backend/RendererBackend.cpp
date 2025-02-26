#include "RendererBackend.h"
#include <GeometrySystem.h>
#include <TextureSystem.h>
#include <ShaderSystem.h>
#include <BackendConfigData.h>
#include <BackendUtil.h>
#include <GLCommandCentre/GLCommandCentre.h>

#include "../includes/GLFW/glfw3.h"
#include "ThreadPool/ThreadPool.h"

namespace Cold {
    RendererBackend::RendererBackend() {
       // geometry_system = GeometrySystem::initiate();
    }

    void RendererBackend::initiate()
    {
        // initialising global ubo
        this->global_uniform_object = new GlobalUniformObject;


        // Texture Sub-system
        TextureSystem::initiate();

        // Geometry sub-system
        geometry_system = GeometrySystem::initiate();

        // shader sub-system
        initiate_shader_map();
        ShaderSystem::initiate();
        ShaderSystem::global_uniform_buffer_object_add(global_uniform_object,
                     sizeof(GlobalUniformObject), global_data.global_uniform_buffer_name);
        initialise_default_shader();

        GLCommandCentre::start();
        set_open_gl_settings();

        // TODO move these

        key_press_handler = BIND_EVENT_FUNCTION(KeyPressedEvent, &RendererBackend::on_key_press_event, this);
        key_press_handler->is_toggle = true;
        EventSystemHelper::subscribe(EVENTTYPE_KEY_PRESSED, key_press_handler);



    }
    void RendererBackend::shut_down()
    {
        GLCommandCentre::stop();
        ShaderSystem::shut_down();
        GeometrySystem::shutdown();
        TextureSystem::shutdown();
    }

    void RendererBackend::on_frame_render()
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLCommandCentre::get_instance()->run_commands();
        for(auto [mesh_name, mesh] : meshes)
        {
            if(mesh_name ==  "sqaure")
                mesh->get_transform()->anim_rotate({0,0.5,0});
            mesh->render(geometry_system);
        }

    }

    void RendererBackend::set_frame_packet(FramePacketSPtr frame_packet)
    {
    }
    
    void RendererBackend::set_open_gl_settings()
    {
        glEnable(GL_DEPTH_TEST); 
        glDepthFunc(GL_LEQUAL);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CW);
        
    }

    void RendererBackend::on_camera_props_change(const glm::mat4& new_view_model, const glm::vec3 new_camera_position)
    {
        global_uniform_object->view_model = new_view_model;
        global_uniform_object->camera_position = new_camera_position;
        GLCommandCentre::get_instance()->submit(&RendererBackend::pass_camera_props_to_gpu,
            this);
    }

    void RendererBackend::pass_camera_props_to_gpu() {
        ShaderSystem::global_uniform_buffer_object_update(global_data.global_uniform_buffer_name,
                                                            &global_uniform_object->view_model,
                                                            sizeof(glm::mat4),
                                                            offsetof(GlobalUniformObject, view_model));

        ShaderSystem::global_uniform_buffer_object_update(global_data.global_uniform_buffer_name,
                                                            &global_uniform_object->camera_position,
                                                            sizeof(glm::vec3),
                                                            offsetof(GlobalUniformObject, camera_position));
    }

    void RendererBackend::load_data()
    {
       // MEMORY LEAK HERE
        TransformSPtr root = std::make_shared<Cold::Transform>();
        TransformSPtr root_2 = std::make_shared<Cold::Transform>();
        StaticMesh* sponza_mesh = new StaticMesh(root, "Assets/Models/sponza/sponza.obj");
        root->scale({0.05,00.05,0.05});
        ThreadPool::get_instance()->submit(&StaticMesh::load_mesh, sponza_mesh, geometry_system);
     ///   sponza_mesh->load_mesh(geometry_system);
    //    sponza_mesh->buffer_to_gpu(geometry_system);

        StaticMesh* falcon = new StaticMesh(root_2, "Assets/Models/falcon/falcon.obj");
        root_2->translate({50,0,0});
        ThreadPool::get_instance()->submit(&StaticMesh::load_mesh, falcon, geometry_system);
       // falcon->load_mesh(geometry_system);
     //   falcon->buffer_to_gpu(geometry_system);

        TransformSPtr square_transform = std::make_shared<Transform>();
        square_transform->translate({0,10,0});
        square_transform->scale({5,5,5});
        auto sqaure_mesh = new StaticMesh(square_transform, "Assets/Models/Cube/cube.obj");
        ThreadPool::get_instance()->submit(&StaticMesh::load_mesh, sqaure_mesh, geometry_system);
    //    sqaure_mesh->load_mesh(geometry_system);
     //   sqaure_mesh->buffer_to_gpu(geometry_system);
        sqaure_mesh->set_cull_facing(false);

        meshes.insert({"sponza", sponza_mesh});
        meshes.insert({"falcon", falcon});
        meshes.insert({"sqaure", sqaure_mesh});
    }


    RendererBackend::~RendererBackend()
    {
        delete global_uniform_object;
        shut_down();
    }

    void RendererBackend::initialise_default_shader()
    {
        ShaderSystem::create_shader(default_data.default_shader_name);
        ShaderSystem::shaders_compile(default_data.default_shader_name, ShaderEnum::VERT_3D, ShaderEnum::FRAG_3D);
    }
    void RendererBackend::on_key_press_event(KeyPressedEvent *event)
    {
        static u32 i = 2;
        if(event->get_key_code() == GLFW_KEY_T) {
            Material mat;
            if (i == 2) {
                mat.diff_tex = cobble_stone.diffuse;
            }

            else if (i == 3) {
                mat.diff_tex = cobble_stone.diffuse;
                mat.specular_texure = cobble_stone.specular;
                mat.shininess = cobble_stone.shininess;
            }

            else if (i == 4) {
                mat.diff_tex = cobble_stone.diffuse;
                mat.specular_texure = cobble_stone.specular;
                mat.shininess = cobble_stone.shininess;
                mat.normal_texture = cobble_stone.normal;
            }

            else {
                mat.diff_tex = default_data.default_texture_path;
                i = 1;
            }

            if(meshes.count("sqaure")) {
                meshes["sqaure"]->set_material(geometry_system, mat, "Cube");
            }
            i++;
            COLD_ERROR("Value of i = %d", i);
        }

        event->set_handled_flag(true);
    }

    ////////////////////////////////////  API CALLS  ////////////////////////////////////

    i32 RendererBackend::create_static_mesh(const std::string& path,
        glm::vec3 translation , glm::vec3 scale) {
        TransformSPtr root = std::make_shared<Transform>();
        root->translate(translation);
        root->scale(scale);
        StaticMesh* s = new StaticMesh(root, path);
        s->load_mesh(geometry_system);
        static int i = 0;
        if (meshes.count(path)) {
            auto new_path = path + std::to_string(i);
            i++;
            meshes.insert({new_path, s});
        }
        else
            meshes.insert({path, s});
        return 0; // TODO for now return 0 as will make a Mesh manager
    }

    void RendererBackend::delete_static_mesh(i32 static_mesh_id) {
    }

    ////////////////////////////////////  API CALLS  ////////////////////////////////////
}
