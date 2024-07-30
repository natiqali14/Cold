#pragma once
#include <includes.h>
namespace Cold {
    class Transform;
    using TransformSPtr = std::shared_ptr<Transform>;
    class Transform {
    public:
        void set_parent(TransformSPtr p);
        glm::mat4 get_world_model();
        glm::mat4 get_local_model();
        glm::vec3 position {glm::vec3(0.0f)};
        glm::vec3 rotation {glm::vec3(0.0f)}; // TODO make it to quant
        glm::vec3 scale {glm::vec3(1.0f)};;

        TransformSPtr parent {nullptr};

        glm::mat4 model_matrix {glm::mat4(1.0)};
    };
}
