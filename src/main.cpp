

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
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);



int main()
{
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


    // --------------------------------  Backend Preparations ---------------------------------

    Cold::RendererBackend::initiate();
    Cold::RendererBackend::set_open_gl_settings();
    Cold::RendererBackend::load_data();

    // --------------------------------  Backend Preparations ---------------------------------

    // render loop
    // -----------
    u64 start = main_clock.get_current_time_in_us();
    while (!main_window->should_close_window())
    {
        u64 last_frame_time = main_clock.get_current_time_in_us();
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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    
}