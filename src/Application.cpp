//
// Created by frio on 16/01/2025.
//

#include "Application.h"
#include "comman_data.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <WindowSystemUtility.h>

#include "ThreadPool/ThreadPool.h"


Application::Application() {
    initialise_systems();  // Systems that are not dependent on opengl or glfw

    initialise_opengl();

    initialise_imgui();

    camera_system = new CameraSystem;
    renderer_backend = new Cold::RendererBackend;
    renderer_backend->initiate();
    main_editor = new Cold::MainGUI;
    main_editor->initialise();

    COLD_ERROR("Application 1");
   // renderer_backend->initiate();
    COLD_ERROR("Application 2");
    main_editor->initialise();
    COLD_ERROR("Application 3");

    main_editor->backend_ptr = renderer_backend;


}

Application::~Application() {
    Cold::ThreadPool::stop();
    delete camera_system;
    delete main_editor;
    delete renderer_backend;
    delete main_window;
}

void Application::run() {
    f32 frame_rate = 0;
    u64 start = application_clock.get_current_time_in_us();
 //   renderer_backend->load_data();
    // -----------------------------  Graphic Setting UI ------------------------ //

    while (!main_window->should_close_window())
    {
        u64 last_frame_time = application_clock.get_current_time_in_us();
        frame_rate =  1e6  / (last_frame_time - start);
        COLD_INFO("FRAME RATE %f", 1e6  / (last_frame_time - start));
        start = application_clock.get_current_time_in_us();
        // input
        // -----
       // processInput(main_window->get_window_ptr());
        EventSystemHelper::dispatch_events();

        // render
        // ------
        renderer_backend->on_camera_props_change(camera_system->get_camera_view_space(),
                                                      camera_system->get_camera_position());
        renderer_backend->on_frame_render();

        // imGUI start
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        main_editor->set_frame_rate(std::to_string(static_cast<i32>(frame_rate)));
        main_editor->render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // imGUI end

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(main_window->get_window_ptr());   // swap buffers

        main_window->poll_input_events();  //  polling input events in Event system Queue

        u64 end = application_clock.get_current_time_in_us();
        u64 time_taken = end - start;
        // spin lock for now on rendering thread
        u64 remaining_time = MICRO_SECONDS_PER_FRAME - time_taken;
        while(remaining_time <= MICRO_SECONDS_PER_FRAME) {
            // do nothing
            remaining_time = application_clock.get_current_time_in_us() - start;
        }
    }
}

void Application::initialise_opengl() {
    WindowSystemUtility::initialise_glfw();
    main_window = new GWindow(SCR_WIDTH, SCR_HEIGHT, "Main Window");
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
}

void Application::initialise_imgui() {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(main_window->get_window_ptr(), true);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());
}

void Application::initialise_systems() {
    Cold::ThreadPool::start(5);
    EventSystemHelper::initialise();
}
