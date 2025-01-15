//
// Created by frio on 15/01/2025.
//

#include "GLCommandCentre.h"

namespace Cold {
    void GLCommandCentre::start() {
        instance = new GLCommandCentre;
    }

    void GLCommandCentre::stop() {
        delete instance;
        instance = nullptr;
    }

    GLCommandCentre * GLCommandCentre::get_instance() {
        return instance;
    }

    void GLCommandCentre::run_commands() {
        while (auto command = gl_queue.try_and_pop()) {
            (*command)();
        }
    }
}
