#include "Shader.h"

#include <string>
#include <fstream>
#include <iostream>

// Helper to read the file into a string
std::string readFile(const std::string& path)
{
    std::ifstream handle(path);
    handle.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    return std::string(
        std::istreambuf_iterator<char>(handle),
        std::istreambuf_iterator<char>()
    );
}

Shader::Shader(const char* vertexShaderPath, const char* fragementShaderPath)
{
    std::string vertexShaderCodeStr;
    std::string fragmentShaderCodeStr;

    try{
        vertexShaderCodeStr = readFile(vertexShaderPath);
        fragmentShaderCodeStr = readFile(fragementShaderPath);
    } catch (const std::exception& e) {
        std::cout << "ERROR: Failed to read shader files\n" << e.what() << "\n";
        return;
    }

    const char* vertexShaderCode = vertexShaderCodeStr.c_str();
    const char* fragmentShaderCode = fragmentShaderCodeStr.c_str();

    GLint success;
    GLchar errorLog[4096];

    // create vertex shader
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)// check if was succesful.
    {
        glGetShaderInfoLog(vertex, 4096, nullptr, errorLog);
        std::cout << "ERROR: error during compilation of the vertex shader\n" << errorLog << "\n";
        return;
    }

    // create fragment Shader
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)// check if was succesful.
    {
        glGetShaderInfoLog(fragment, 4096, nullptr, errorLog);
        std::cout << "ERROR: error during compilation of the fragment shader.\n" << errorLog << "\n";
        return;
    }

    // create Program
    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success)// check if was succesful.
    {
        glGetProgramInfoLog(m_id, 4096, nullptr, errorLog);
        std::cout << "ERROR: error during program linking\n" << errorLog << "\n";
        m_id = 0;
        return;
    }

    // cleanup shaders, they can go now(they are in the program now).
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}