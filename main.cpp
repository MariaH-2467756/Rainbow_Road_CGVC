#include "Shader.h"
#include "Window.h"
#include <glm/glm.hpp>

const unsigned int WINDOW_WIDTH = 1900;
const unsigned int WINDOW_HEIGTH = 1000;
const char* WINDOW_NAME = "Rainbow Road";

int main()
{
    Window window = Window(WINDOW_WIDTH, WINDOW_HEIGTH, WINDOW_NAME);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader shader = Shader("shaders/basic_vertex_shader.glsl","shaders/basic_fragment_shader.glsl");

    while (!window.shouldClose())
    {
        
        glClearColor(sin(glfwGetTime() * 2.0), sin(glfwGetTime() * 1.5), sin(glfwGetTime() * 1.0), 1.0f);

        // Flush the background
        glClear(GL_COLOR_BUFFER_BIT);

        // All complex shizzle.

        // swap the buffers.
        window.swapBuffers();

        // when we do the interactions check to polls shizzle her (No callback logic made yet.).
        window.pollEvents();
    }
}