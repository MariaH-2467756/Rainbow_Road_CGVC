#include "Shader.h"
#include "Window.h"
#include "/home/Ramen/Documents/School/2BA/SEM2/Computer_Graphics/project/Rainbow_Road_CGVC/track/TrackRenderer.h"
#include "includes/GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"
#include "ObjLoader.h"

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

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGTH);

    // AI genrated example curves. (for testing NOT FINAL)
    BezierCurve c1 ({ -3,0,0 },  { -1,0,2 },   { 1,0,2 },    { 3,0,0 });
    BezierCurve c2 ({ 3,0,0 },   { 5,0,-2 },   { 7,0,-2 },   { 9,0,0 });
    BezierCurve c3 ({ 9,0,0 },   { 11,0,2 },   { 13,0,2 },   { 15,0,0 });
    BezierCurve c4 ({ 15,0,0 },  { 17,0,-2 },  { 19,0,-2 },  { 21,0,0 });

    BezierCurve c5 ({ 21,0,0 },  { 23,0,3 },   { 23,0,6 },   { 21,0,9 });
    BezierCurve c6 ({ 21,0,9 },  { 19,0,11 },  { 17,0,11 },  { 15,0,9 });
    BezierCurve c7 ({ 15,0,9 },  { 13,0,7 },   { 11,0,7 },   { 9,0,9 });
    BezierCurve c8 ({ 9,0,9 },   { 7,0,11 },   { 5,0,11 },   { 3,0,9 });

    BezierCurve c9 ({ 3,0,9 },   { 1,0,7 },    { -1,0,7 },   { -3,0,9 });
    BezierCurve c10({ -3,0,9 },  { -5,0,11 },  { -7,0,11 },  { -9,0,9 });
    BezierCurve c11({ -9,0,9 },  { -11,0,7 },  { -13,0,7 },  { -15,0,9 });
    BezierCurve c12({ -15,0,9 }, { -17,0,11 }, { -19,0,11 }, { -21,0,9 });

    BezierCurve c13({ -21,0,9 }, { -23,0,6 },  { -23,0,3 },  { -21,0,0 });
    BezierCurve c14({ -21,0,0 }, { -19,0,-2 }, { -17,0,-2 }, { -15,0,0 });
    BezierCurve c15({ -15,0,0 }, { -13,0,2 },  { -11,0,2 },  { -9,0,0 });
    BezierCurve c16({ -9,0,0 },  { -7,0,-2 },  { -5,0,-2 },  { -3,0,0 });

    BezierCurve c17({ -3,0,0 },  { -1,0,-3 },  { 1,0,-3 },   { 3,0,0 });
    BezierCurve c18({ 3,0,0 },   { 5,0,3 },    { 7,0,3 },    { 9,0,0 });
    BezierCurve c19({ 9,0,0 },   { 11,0,-3 },  { 13,0,-3 },  { 15,0,0 });
    BezierCurve c20({ 15,0,0 },  { 9,0,-1 },   { 3,0,-1 },   { -3,0,0 });

    Track track({
    c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,
    c11,c12,c13,c14,c15,c16,c17,c18,c19,c20
    }); // Test track
    track.build(0.4f);

    TrackRenderer renderer;
    renderer.upload(track);

    // Mesh test.
    MeshData grandStarData = ObjLoader::load("assets/Grand_Star_Mario(for_light_source).obj");
    Mesh grandStarMesh = Mesh(grandStarData);

    Shader shader("shaders/basic_vertex_shader.glsl", "shaders/basic_fragment_shader.glsl");

    // matrices
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view  = glm::lookAt(
        glm::vec3(0, 5, -8),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    ); // Hardcoded camera position in view mat.
    glm::mat4 proj  = glm::perspective(
        glm::radians(45.0f),
        (float)WINDOW_WIDTH / WINDOW_HEIGTH,
        0.1f, 100.0f
    ); // basic projection matrix.
    // normalMatrix transforms normals correctly when model is scaled/rotated
    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model))); // Calc normal on CPU now.

    // light
    glm::vec3 lightPos(2.0f, 4.0f, 2.0f); // Hardcoded light.

    glEnable(GL_DEPTH_TEST);

    while (!window.shouldClose())
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.useProgram();

        // matrices unifroms
        shader.setMat4("model",      model);
        shader.setMat4("view",       view);
        shader.setMat4("projection", proj);
        shader.setMat3("normalMatrix", normalMatrix);

        // light uniforms
        shader.setVec3Uniform("light.position", lightPos);
        shader.setVec3Uniform("light.ambient",  glm::vec3(1.0f));
        shader.setVec3Uniform("light.diffuse",  glm::vec3(0.8f));
        shader.setVec3Uniform("light.specular", glm::vec3(1.0f));

        // material unifroms
        shader.setVec3Uniform("material.ambient",   glm::vec3(sin(glfwGetTime() *1.0f), sin(glfwGetTime() *0.2f), sin(glfwGetTime() *0.8f)));
        shader.setVec3Uniform("material.diffuse",   glm::vec3(0.2f, 0.2f, 0.8f));
        shader.setVec3Uniform("material.specular",  glm::vec3(0.5f));
        shader.setFloatUniform("material.shininess", 32.0f);

        // camera position for specular
        shader.setVec3Uniform("viewPosition", glm::vec3(0, 5, -8)); // The hardcode camera.

        renderer.draw();

        glm::mat4 model = glm::mat4(1.0f); // wuick translations on model for test of loader object.
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f)); // move
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.01f));  
        shader.setMat4("model",      model);

        grandStarMesh.draw();

        window.swapBuffers();
        window.pollEvents();
    }
}
