#include "Transforms.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Logger.h>
namespace Cold {
    void Transform::re_calculate_transform_members()
    {
        glm::vec3 skew;
        glm::vec4 presp;
        bool is_success = glm::decompose(model_matrix, _scale, rotation, this->position, skew, presp);
        COLD_ASSERT(is_success, "Decomposing of transform is failing");
        is_dirty = false;
    }

    void Transform::set_parent(TransformSPtr p)
    {
        this->parent = p;
    }
    
    glm::mat4 Transform::get_world_model()
    {
        if(parent) {
            return parent->model_matrix * model_matrix;
        }
        return model_matrix;
    }
    glm::mat4 Transform::get_local_model()
    {
        return model_matrix;
    }

    void Transform::translate(glm::vec3 amount)
    {
        model_matrix = glm::translate(model_matrix, amount);
        is_dirty = true;
    }
    
    void Transform::set_absolute_position(glm::vec3 position)
    {
        if(is_dirty) re_calculate_transform_members();
        glm::vec3 to_translate = position - this->position;
        model_matrix = glm::translate(model_matrix, to_translate);
        this->position = position;
    }

    void Transform::scale(glm::vec3 amount)
    {
        model_matrix = glm::scale(model_matrix, amount);
        is_dirty = true;
    }

    void Transform::set_absolute_scale(glm::vec3 _scale)
    {
        if(is_dirty) re_calculate_transform_members();
        glm::vec3 target_scale = _scale / this->_scale;
        model_matrix = glm::scale(model_matrix, target_scale);
        this->_scale = _scale;
    }

    void Transform::rotate(glm::vec3 amount)
    {
        glm::vec3 current_in_degrees = glm::degrees(glm::eulerAngles(rotation));
        glm::vec3 to_rotate = amount - current_in_degrees;
        glm::quat new_rot = glm::quat(glm::radians(to_rotate));
        glm::mat4 rot_mat = glm::mat4_cast(new_rot);
        model_matrix = rot_mat * model_matrix;
        rotation = new_rot;
        is_dirty = true;
    }
}
