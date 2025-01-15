

#include "includes.h"
#include "../includes/glad/glad.h"
#include <iostream>
#include "WindowSystem/GWindow.h"
#include "WindowSystem/WindowSystemUtility.h"
#include <GLFW/glfw3.h>
#include "EventSystem/EventSystemHelper.h"

#include "Utility/helper.h"
#include "comman_data.h"
#include "Utility/Logger.h"
#include <filesystem>
#include <Clock.h>
#include <fstream>
#include <sstream>
#include <ShaderCode/ShaderGetter.h>
#include <RendererBackend.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <Buttons/Button.h>
#include <Inputs/InputText.h>
#include <Text/Text.h>
#include <UIHolder/UIHolder.h>
#include "ThreadPool/ThreadPool.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);



int main()
{
   

    //////////////////////  THREAD POOL  //////////////////////
    Cold::ThreadPool::start(5);
    /////////////////////////  THREAD POOL  //////////////////
    Cold::Clock main_clock;
    EventSystemHelper::initialise();
    WindowSystemUtility::initialise_glfw();
    CameraSystem* camera = new CameraSystem;
    // --------------------------------  Creating main window  --------------------------------

    std::unique_ptr<GWindow> main_window = std::make_unique<GWindow>(SCR_WIDTH, SCR_HEIGHT, "OPENGL WINDOW");
    main_window->should_hide_cursor(true);

    // --------------------------------  Creating main window  --------------------------------



    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }


    //  -------------------------------  ImGUI INIT  -------------------------------  //

    const char* glsl_version = "#version 400"; // For OpenGL 3.2+

    // Initialize ImGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(main_window->get_window_ptr(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //  -------------------------------  ImGUI INIT  -------------------------------  //
    

    // --------------------------------  Backend Preparations ---------------------------------

    Cold::RendererBackend::initiate();
    Cold::RendererBackend::set_open_gl_settings();
    Cold::RendererBackend::load_data();

    // --------------------------------  Backend Preparations ---------------------------------

    // render loop
    // -----------
    u64 start = main_clock.get_current_time_in_us();
    bool a = true;
    auto ui_holder = Cold::UIHolder::create_ui_holder_object({400,400});
    std::shared_ptr<Cold::InputText> asset_input =
        std::shared_ptr<Cold::InputText>(new Cold::InputText("Enter Asset pathe"));
    auto btn_on_click = [&]() {
        COLD_ERROR("%s", asset_input->get_text().c_str());
    };
    std::shared_ptr<Cold::Button> btn1 =
        std::shared_ptr<Cold::Button> (new Cold::Button([](){}, "btn1", {100, 20}));
    ui_holder->add_component(asset_input);
    ui_holder->add_component_in_new_row(btn1);
    btn1->set_on_click(btn_on_click);


    // -----------------------------  Graphic Setting UI ------------------------ //

    f32 frame_rate = 0;
    auto setting_ui_holder =Cold::UIHolder::create_ui_holder_object({0,0}, "2");
    std::shared_ptr<Cold::Text> fps = std::shared_ptr<Cold::Text>(new Cold::Text("0", 2.0, {1,1,0,1}));
    setting_ui_holder->add_component(fps);

    // -----------------------------  Graphic Setting UI ------------------------ //

    while (!main_window->should_close_window())
    {
        u64 last_frame_time = main_clock.get_current_time_in_us();
        frame_rate =  1e6  / (last_frame_time - start);
        COLD_INFO("FRAME RATE %f", 1e6  / (last_frame_time - start));
        start = main_clock.get_current_time_in_us();
        // input
        // -----
        processInput(main_window->get_window_ptr());
        EventSystemHelper::dispatch_events();

        // render
        // ------
        Cold::RendererBackend::on_camera_props_change(camera->get_camera_view_space(),
                                                      camera->get_camera_position());
        Cold::RendererBackend::on_frame_render();

        // imGUI start

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ui_holder->render();
        fps->set_content(std::to_string(static_cast<i32>(frame_rate)));
        setting_ui_holder->render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(main_window->get_window_ptr());   // swap buffers
        main_window->poll_input_events();  //  polling input events in Event system Queue
        u64 end = main_clock.get_current_time_in_us();
        u64 time_taken = end - start;

        u64 remainin_time = MICRO_SECONDS_PER_FRAME - time_taken;
        while(remainin_time <= MICRO_SECONDS_PER_FRAME) {
            // do nothing
            u64 new_time = main_clock.get_current_time_in_us();
            remainin_time = new_time - start;
        }
    }
   

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    // static bool should_hide = false;
    // if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
    //     u32 value = should_hide ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
    //     glfwSetInputMode(window, GLFW_CURSOR, value);
    //     should_hide = !should_hide;
    // }
        
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    
}