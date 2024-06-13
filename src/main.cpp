

#include "includes.h"
#include <iostream>
#include "WindowSystem/GWindow.h"
#include "WindowSystem/WindowSystemUtility.h"
#include "opengl_backend/playground.h"
#include <GLFW/glfw3.h>
#include "EventSystem/EventSystemHelper.h"
#include "CameraSystem/CameraSystem.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Utility/Clock/Clock.h"
#include "Utility/helper.h"
#include "comman_data.h"
#include "Utility/Logger.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
///
void m(int arr[], int size ) {
    arr[size -1]  =4;
}

int main()
{
    Clock main_clock;
    Assimp::Importer importer;
    EventSystemHelper::initialise();
    WindowSystemUtility::initialise_glfw();
    // --------------------------------  Creating main window  --------------------------------

    std::unique_ptr<GWindow> main_window = std::make_unique<GWindow>(SCR_WIDTH, SCR_HEIGHT, "OPENGL WINDOW");
    main_window->should_hide_cursor(true);

    // --------------------------------  Creating main window  --------------------------------

        // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // ---------------------------------------
    // render loop

    std::vector<u32> shader_ids;
    u32 v_shader = create_shader(GL_VERTEX_SHADER, vertexShaderSource);
    u32 f_shader = create_shader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    shader_ids.push_back(v_shader);
    shader_ids.push_back(f_shader);
    u32 program = create_program(shader_ids);
   
    buffer_data data1 = initialise_triangle();
    buffer_data data2 = initialise_triangle();
    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    u64 start = main_clock.get_current_time_in_us();
    while (!main_window->should_close_window())
    {
        u64 last_frame_time = main_clock.get_current_time_in_us();
        //std::cout<< "FRAME RATE: " << 1e6  / (last_frame_time - start) << "\n";
        start = main_clock.get_current_time_in_us();
        // input
        // -----
        processInput(main_window->get_window_ptr());
        EventSystemHelper::dispatch_events();

        // render
        // ------
        glClearColor(0.2f, 0.1f, 0.1f, 01.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw our first triangle
        //transformations(program);
        render_cubes(program, data1, data2);
        glUseProgram(program);
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(main_window->get_window_ptr());   // swap buffers
        main_window->poll_input_events();  //  polling input events in Event system Queue
        u64 end = main_clock.get_current_time_in_us();
        u64 time_taken = end - start;
        
        // --------------------------------   BELOW code is for sleep if needed.  --------------------------------
        // TODO find a better approach, here nanosleep is not accurate on mac os
        // so right now using while loop for fake but precise sleep
        // if(time_taken < MICRO_SECONDS_PER_FRAME) {
        //     u64 time_to_sleep = (MICRO_SECONDS_PER_FRAME - time_taken) * 1e3;
        //     Engine::Helper::sleep_current_thread(0, time_to_sleep);
        // }

        u64 remainin_time = MICRO_SECONDS_PER_FRAME - time_taken;
        while(remainin_time <= MICRO_SECONDS_PER_FRAME) {
            // do nothing
            u64 new_time = main_clock.get_current_time_in_us();
            remainin_time = new_time - start;
        }
        ERROR("ERROR here");
        WARNING("Warning here");
        TRACE("TRACE here");
        INFO("INFO here");
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
   

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