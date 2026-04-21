#pragma once
#include "includes/GLFW/glfw3.h"
#include <iostream>

class Window {
public:
    Window(int width, int height, const char* name);
    ~Window();
    bool shouldClose();
    void swapBuffers();
    void pollEvents();


private:
    void init();
    void loop();
    void close();

    int m_width, m_height;
    const char* m_name;
    GLFWwindow* m_window;

};