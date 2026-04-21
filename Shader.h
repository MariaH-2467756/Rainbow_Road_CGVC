#pragma once
#include "includes/glad/glad.h"
#include <glm/glm.hpp>

class Shader {
public:
    Shader(const char* vertexShaderPath, const char* fragementShaderPath);
    void useProgram() { glUseProgram(m_id); };
    void setBoolUniform(const char* &name, bool value) { glUniform1i(glGetUniformLocation(m_id, name), (int)value); }
    void setIntUniform(const char* &name, bool value) { glUniform1i(glGetUniformLocation(m_id, name), (int)value); }
    void setFloatUniform(const char* &name, bool value) { glUniform1f(glGetUniformLocation(m_id, name), (int)value); }
    void setVec3Uniform(const char* &name, const glm::vec3 &value) { glUniform3fv(glGetUniformLocation(m_id, name), 1, &value[0]); }
    void setMat4(const char* &name, const glm::mat4 &mat) { glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, &mat[0][0]); }

private:
    int m_id;
    
};