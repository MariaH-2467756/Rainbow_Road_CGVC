#include "ChromaObject.h"
#include "Mesh.h"
#include "ObjLoader.h"
#include <glm/ext/matrix_transform.hpp>

#define LAKITU_OBJ_PATH "assets/Lakitu/Lakitu.obj"
#define LAKITU_TEXTURE_PATH "assets/Lakitu/jugem_tex.png"
#define SIGN_TEXTURE_PATH                                                      \
  "assets/mario_kart_meme.png" //"assets/green_screen.png"

class LakituWithSign {
public:
  LakituWithSign()
      : m_lakitu(ObjLoader().load(LAKITU_OBJ_PATH, LAKITU_TEXTURE_PATH)),
        m_sign(ChromaObject(SIGN_TEXTURE_PATH)) {}

  void draw(Shader &shader, const glm::mat4 &model, const glm::mat4 &view,
            const glm::mat4 &projection, float t) {
    float loopX = 5.0f * cos(t);
    float loopY = 2.5f * sin(2.0f * t);

    glm::mat4 infintyModel = glm::translate(
        model,
        glm::vec3(loopX, loopY,
                  -10.0f)); // moves in infitny sign.(might have to callibrate
                            // pos, but see when cameras are good.)

    glm::mat4 lakituModel = glm::scale(infintyModel, glm::vec3(0.025f));
    shader.setMat4("model", lakituModel);
    m_lakitu.draw();

    glm::mat4 signModel =
        glm::scale(infintyModel, glm::vec3(1.25f, 0.625f, 1.0f));
    m_sign.draw(signModel, view, projection);
  }

private:
  Mesh m_lakitu;
  ChromaObject m_sign;
  GLuint m_signTexture;
};