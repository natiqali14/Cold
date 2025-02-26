
#define GLAD_GL_IMPLEMENTATION

#include "Application.h"
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

#ifdef __APPLE__
void signal_handler(int signal, siginfo_t* info, void* context) {
    void *array[100];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 100);

    // print out all the frames to stderr
    COLD_ERROR("Error: signal %d:\n", signal);

    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(signal);
}
#endif

int main() {

    // For showing back trace on signal traps (crashes)
#ifdef __APPLE__
    // Set up the sigaction struct
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO; // Use the sa_sigaction field
    sa.sa_sigaction = signal_handler;
    sigemptyset(&sa.sa_mask); // Block no additional signals during handling

    // Register the handler for SIGSEGV
    if (sigaction(SIGSEGV, &sa, nullptr) == -1) {
        perror("sigaction");
        return 1;
    }

    // Cause a segmentation fault (for testing purposes)

#endif




    Application application;
    application.run();
    return 0;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    
}