#pragma once
#include <includes.h>
namespace Cold {
    class Transform;
    using TransformSPtr = std::shared_ptr<Transform>;
    class Transform {
        glm::vec3 position {glm::vec3(0.0f)};
        glm::quat rotation {glm::identity<glm::quat>()}; // TODO make it to quant
        glm::vec3 _scale {glm::vec3(1.0f)};;

        glm::mat4 translation_matrix {glm::mat4(1.0)};
        glm::mat4 roation_matrix {glm::mat4(1.0)};
        glm::mat4 scale_matrix {glm::mat4(1.0)};
        bool is_dirty {false};

        void re_calculate_transform_members();
    public:
        void set_parent(TransformSPtr p);
        glm::mat4 get_world_model();
        glm::mat4 get_local_model();
        void translate(glm::vec3 amount);
        void set_absolute_position(glm::vec3 position);
        void scale(glm::vec3 amount);
        void set_absolute_scale(glm::vec3 _scale);
        void rotate(glm::vec3 amount);
        void anim_rotate(glm::vec3 amount);

        TransformSPtr parent {nullptr};

        glm::mat4 model_matrix {glm::mat4(1.0)};
    };
}
