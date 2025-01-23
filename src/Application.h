//
// Created by frio on 16/01/2025.
//

#ifndef APPLICATION_H
#define APPLICATION_H


#include <CameraSystem.h>
#include <Clock.h>
#include <RendererBackend.h>
#include <GWindow.h>
#include <includes.h>
#include <FrontEnd/MainGUI.h>

class Application {
public:
    Application();
    ~Application();
    void run();
private:
    // main entry to gui
    Cold::MainGUI* main_editor;
    // Entry to opengl backend communications
    Cold::RendererBackend* renderer_backend;
    // main window for rendering
    GWindow* main_window;
    Cold::Clock application_clock;
    CameraSystem* camera_system;

    // Initialisations
    void initialise_opengl();
    void initialise_imgui();
    void initialise_systems();
};



#endif //APPLICATION_H
