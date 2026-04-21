#include "Window.h"

Window::Window(int width, int height, const char* name) : m_width(width), m_height(height), m_name(name) 
{
    init();
}

Window::~Window() 
{
    close();
}

bool Window::shouldClose()
{
    return glfwWindowShouldClose(m_window);
}

void Window::swapBuffers()
{
    glfwSwapBuffers(m_window);
}

void Window::pollEvents()
{
    glfwPollEvents();
}


void Window::init()
{
    if (!glfwInit()){
        std::cerr << "Failed to initialize GLFW\n";
    }

    // Tell it what version of openGL I'll we using (3.3 with core profile.)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create our window
    m_window = glfwCreateWindow( m_width, m_height, m_name, nullptr, nullptr);

    // Check if the window was created
    if (!m_window) {
            std::cerr << "Failed to create window\n";
            glfwTerminate(); // If failed we use GLFW to terminate.
            return;
    }

    // Put our window in the context.
    glfwMakeContextCurrent(m_window);
}



void Window::close()
{
    // Clean up the window;
    glfwDestroyWindow(m_window);
    glfwTerminate();
}