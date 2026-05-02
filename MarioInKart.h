#include "Mesh.h"
#include "ObjLoader.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define MARIO_OBJ_PATH "assets/Mario Kart/model_0.obj"
#define MARIO_TEXTURE_PATH "assets/Mario Kart/F2_Item_Kart_Mario_Body_S.png"
#define KART_OBJ_PATH "assets/Mario Kart/model_1.obj"
#define KART_TEXTURE_PATH "assets/Mario Kart/F2_Item_Kart_Mario_Kart_S.png"
#define WHEELS_OBJ_PATH "assets/Mario Kart/model_2.obj"
#define WHEELS_TEXTURE_PATH "assets/Mario Kart/F2_Item_Kart_Mario_Tire_S.png"

class MarioInKart {
public:
  MarioInKart()
      : m_mario(ObjLoader().load(MARIO_OBJ_PATH, MARIO_TEXTURE_PATH)),
        m_kart(ObjLoader().load(KART_OBJ_PATH, KART_TEXTURE_PATH)),
        m_wheels(ObjLoader().load(WHEELS_OBJ_PATH, WHEELS_TEXTURE_PATH)) {};

  void draw(Shader &shader, const glm::mat4 &model, const glm::mat4 &view,
            const glm::mat4 &projection) {

    shader.useProgram();

    glm::mat4 kartModel =
        glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    kartModel = glm::scale(kartModel, glm::vec3(0.8f));

    shader.setMat4("model", kartModel);

    m_mario.draw();
    m_kart.draw();
    m_wheels.draw();
  }

private:
  Mesh m_mario;
  Mesh m_kart;
  Mesh m_wheels;
};