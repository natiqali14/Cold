#include "RendererBackend.h"
#include <GeometrySystem.h>
#include <TextureSystem.h>
#include <ShaderSystem.h>
#include <BackendConfigData.h>
#include <BackendUtil.h>
#include "../includes/GLFW/glfw3.h"
namespace Cold {
    RendererBackend* instance = nullptr;
    void RendererBackend::initiate()
    {
        instance = new RendererBackend;
        instance->global_uniform_object = new GlobalUniformObject;


        // Texture Sub-system
        TextureSystem::initiate();

        // Geometry sub-system
        GeometrySystem::initiate();

        // shader sub-system
        initiate_shader_map();
        ShaderSystem::initiate();
        ShaderSystem::global_uniform_buffer_object_add(instance->global_uniform_object,
                     sizeof(GlobalUniformObject), global_data.global_uniform_buffer_name);
        instance->initialise_default_shader();

        // TODO move these

        instance->key_press_handler = BIND_EVENT_FUNCTION(KeyPressedEvent, &RendererBackend::on_key_press_event, instance);
        EventSystemHelper::subscribe(EVENTTYPE_KEY_PRESSED, instance->key_press_handler);
        instance->key_press_handler->is_toggle = true;


    }
    void RendererBackend::shut_down()
    {
        ShaderSystem::shut_down();
        GeometrySystem::shutdown();
        TextureSystem::shutdown();

        delete instance;
        instance = nullptr;
    }

    void RendererBackend::on_frame_render()
    {
        glClearColor(0.2f, 0.1f, 0.1f, 01.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for(auto mesh : instance->meshes) 
        {
            if(mesh.first ==  "sqaure")
                mesh.second->get_transform()->anim_rotate({0,0.5,0});
            mesh.second->render();
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
        instance->global_uniform_object->view_model = new_view_model;
        instance->global_uniform_object->camera_position = new_camera_position;
        ShaderSystem::global_uniform_buffer_object_update(global_data.global_uniform_buffer_name,
                                                            &instance->global_uniform_object->view_model,
                                                            sizeof(glm::mat4),
                                                            offsetof(GlobalUniformObject, view_model));

        ShaderSystem::global_uniform_buffer_object_update(global_data.global_uniform_buffer_name,
                                                            &instance->global_uniform_object->camera_position,
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
        sponza_mesh->load_mesh();
        sponza_mesh->buffer_to_gpu();
        StaticMesh* falcon = new StaticMesh(root_2, "Assets/Models/falcon/falcon.obj");
        root_2->translate({50,0,0});
        falcon->load_mesh();
        falcon->buffer_to_gpu();

        TransformSPtr square_transform = std::make_shared<Transform>();
        square_transform->translate({0,10,0});
        square_transform->scale({5,5,5});
        auto sqaure_mesh = new StaticMesh(square_transform, "Assets/Models/Cube/cube.obj");
        sqaure_mesh->load_mesh();
        sqaure_mesh->buffer_to_gpu();
        sqaure_mesh->set_cull_facing(false);

        instance->meshes.insert({"sponza", sponza_mesh});
        instance->meshes.insert({"falcon", falcon});
        instance->meshes.insert({"sqaure", sqaure_mesh});
    }

    RendererBackend::~RendererBackend()
    {
        delete global_uniform_object;
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
            if(instance->meshes.count("sqaure")) {
                instance->meshes["sqaure"]->set_material(mat, "Cube");

            }
            i++;
        }

        event->set_handled_flag(true);
    }
}