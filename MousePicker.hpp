#include "PlayMode.hpp"
#include "Scene.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/ext.hpp>

#include <random>

struct MousePicker {
    glm::vec3 currentRay = glm::vec3(0.0f);
    int screen_x;
    int screen_y;
    //local copy of the game scene
    Scene scene;
    //camera:
    Scene::Camera *camera = nullptr;

    glm::mat4x4 projectionMatrix;
    glm::mat4x4 viewMatrix;

    glm::vec3 getMouseRay();
};


