#include "Shader.h"
#include "Window.h"
#include "track/TrackRenderer.h"
#include "includes/GLFW/glfw3.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
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



    Track track(getCurves()); // Test track
    track.build(0.8f);

    TrackRenderer renderer;
    renderer.upload(track, "assets/rainbow_road_texture.png");

    // Mesh test.
    MeshData grandStarData = ObjLoader().load("assets/Grand_Star_Mario(for_light_source).obj");
    Mesh grandStarMesh = Mesh(grandStarData);

    MeshData mario = ObjLoader().load("assets/Mario Kart/model_0.obj", "assets/Mario Kart/F2_Item_Kart_Mario_Body_S.png");
    Mesh marioMesh = Mesh(mario);
    MeshData kart = ObjLoader().load("assets/Mario Kart/model_1.obj", "assets/Mario Kart/F2_Item_Kart_Mario_Kart_S.png");
    Mesh kartMesh = Mesh(kart);
    MeshData wheels = ObjLoader().load("assets/Mario Kart/model_2.obj", "assets/Mario Kart/F2_Item_Kart_Mario_Tire_S.png");
    Mesh wheelsMesh = Mesh(wheels);

    Shader shader("shaders/basic_vertex_shader.glsl", "shaders/basic_fragment_shader.glsl");

    // matrices
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 cameraPos    = glm::vec3(0.0f, 150.0f, -220.0f);
    glm::vec3 cameraTarget = glm::vec3(10.0f, 2.0f, 0.0f);
    glm::vec3 cameraUp     = glm::vec3(0.0f, 1.0f, 0.0f);

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
    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model))); // Calc normal on CPU now.

    // light
    glm::vec3 lightPos(2.0f, 4.0f, 2.0f); // Hardcoded light.

    glEnable(GL_DEPTH_TEST);

    float distanceTravelled = 0.0f;
    float speed             = 25.0f;
    float lastTime          = glfwGetTime();

    while (!window.shouldClose())
    {
        float now   = glfwGetTime();
        float delta = now - lastTime;
        lastTime    = now;

        distanceTravelled += speed * delta;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.useProgram();
        shader.setIntUniform("texture1", 0);

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
        shader.setVec3Uniform("material.ambient",   glm::vec3(1.0f,1.0f, 1.0f));
        shader.setVec3Uniform("material.diffuse",   glm::vec3(1.0f, 1.0f, 1.f));
        shader.setVec3Uniform("material.specular",  glm::vec3(0.5f));
        shader.setFloatUniform("material.shininess", 32.0f);

        // camera position for specular
        shader.setVec3Uniform("viewPosition", cameraPos); // The hardcode camera.

        renderer.draw();

        glm::mat4 kartModel = track.getTransformAtDistance(distanceTravelled);
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f)); // move
        kartModel = glm::rotate(kartModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        kartModel = glm::scale(kartModel, glm::vec3(0.8f));  
        shader.setMat4("model",kartModel);
        marioMesh.draw();
        kartMesh.draw();
        wheelsMesh.draw();


        window.swapBuffers();
        window.pollEvents();
    }
}
