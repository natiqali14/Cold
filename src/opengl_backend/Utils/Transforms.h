#pragma once
#include <includes.h>

class Transform {
public:
    glm::vec3 position {glm::vec3(0.0f)};
    glm::vec3 rotation; {glm::vec3(0.0f)}; // TODO make it to quant
    glm::vec3 scale {glm::vec3(1.0f)};;

    Transform* parent {nullptr};

    glm::mat4 model_matrix {glm::mat4(1.0)};
};