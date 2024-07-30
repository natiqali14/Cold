#include "Transforms.h"

namespace Cold {

    void Transform::set_parent(TransformSPtr p)
    {
        this->parent = std::move(p);
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
}
