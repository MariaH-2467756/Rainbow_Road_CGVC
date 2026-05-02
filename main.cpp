#include "Shader.h"
#include "Window.h"
#include "track/TrackRenderer.h"
#include "includes/GLFW/glfw3.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "LakituWithSign.h"
#include "MarioInKart.h"
#include "LightObject.h"

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



    Track track(getCurves()); // Test track
    track.build(0.8f);
    TrackRenderer trackRenderer;
    trackRenderer.upload(track, "assets/rainbow_road_texture.png");


    Shader shader = Shader( "shaders/basic_vertex_shader.glsl", "shaders/basic_fragment_shader.glsl" );

    MarioInKart mario = MarioInKart();

    ChromaObject::initShader();
    LakituWithSign lakituWithSign = LakituWithSign();

    LightObject::initShader();
    LightObject lights[5] = {
    {glm::vec3(10.0f, 20.0f, 10.0f),  glm::vec3(0.15f),  glm::vec3(0.8f),           glm::vec3(1.0f)},
    {glm::vec3(-50.0f, 30.0f, 0.0f),  glm::vec3(0.15f),  glm::vec3(0.6f, 0.0f, 0.6f), glm::vec3(1.0f, 0.0f, 1.0f)},
    {glm::vec3(0.0f, 50.0f, -100.0f), glm::vec3(0.15f),  glm::vec3(1.0f, 0.5f, 0.0f), glm::vec3(1.0f, 0.8f, 0.2f)},
    {glm::vec3(100.0f, 15.0f, 50.0f), glm::vec3(0.15f),  glm::vec3(0.0f, 0.4f, 0.8f), glm::vec3(0.0f, 0.6f, 1.0f)},
    {glm::vec3(-30.0f, 10.0f, 80.0f), glm::vec3(0.15f), glm::vec3(0.5f),           glm::vec3(0.5f)}
    };

    // matrices
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 cameraPos    = glm::vec3(0.0f, 150.0f, -220.0f);
    glm::vec3 cameraTarget = glm::vec3(10.0f, 2.0f, 0.0f);
    glm::vec3 cameraUp     = glm::vec3(0.0f, 1.0f, 0.0f);

    // testiing moddel to put something infront of current hardcoded camera
    glm::vec3 cameraForward = glm::normalize(cameraTarget - cameraPos);
    glm::vec3 objectPos = cameraPos + cameraForward * 10.0f;
    glm::mat4 hudModel = glm::translate(glm::mat4(1.0f), objectPos);

    glm::mat4 view = glm::lookAt(
        cameraPos,
        cameraTarget,
        cameraUp
    );

    glm::mat4 proj = glm::perspective(
        glm::radians(30.0f),
        static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGTH),
        0.1f,
        1000.0f
    );
    // normalMatrix transforms normals correctly when model is scaled/rotated
    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model))); // Calc normal on CPU.

    // light
    glm::vec3 lightPos(2.0f, 4.0f, 2.0f); // Hardcoded light.

    glEnable(GL_DEPTH_TEST);

    // vars for moving calcs.
    float distanceTravelled = 0.0f;
    float kartSpeed = 25.0f;
    float lakituSpeed = 1.5f;
    float lastTime = glfwGetTime();

    while (!window.shouldClose())
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.useProgram();

        // matrices uniforms
        shader.setMat4("model",      model);
        shader.setMat4("view",       view);
        shader.setMat4("projection", proj);
        shader.setMat3("normalMatrix", normalMatrix);

        // set the light uniforms
        for (int i = 0; i < 5; i++) {
            lights[i].setLightUniforms(shader, i);
        }

        // material uniforms
        shader.setVec3Uniform("material.ambient",   glm::vec3(1.0f,1.0f, 1.0f));
        shader.setVec3Uniform("material.diffuse",   glm::vec3(1.0f, 1.0f, 1.f));
        shader.setVec3Uniform("material.specular",  glm::vec3(0.5f));
        shader.setFloatUniform("material.shininess", 32.0f);

        // camera position for specular
        shader.setVec3Uniform("viewPosition", cameraPos); // The hardcoded camera.

        // draw track
        trackRenderer.draw();

        // draw objects moving allong track.
        float now   = glfwGetTime();
        float delta = now - lastTime;
        lastTime    = now;
        distanceTravelled += kartSpeed * delta;
        glm::mat4 kartModel = track.getTransformAtDistance(distanceTravelled);
        glm::mat3 kartNormalMatrix = glm::mat3(glm::transpose(glm::inverse(kartModel)));

        shader.setMat3("normalMatrix", kartNormalMatrix);
        mario.draw(shader,kartModel, view, proj);
        lakituWithSign.draw(shader, kartModel, view, proj,lastTime * lakituSpeed);

        // draw the light objects (AKA grandstars)
        for (int i = 0; i < 5; i++) { // last, different shder.
            lights[i].draw(model, view, proj);
        }

        window.swapBuffers();
        window.pollEvents();
    }
}
