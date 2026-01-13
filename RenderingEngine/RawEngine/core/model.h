#pragma once

#include <vector>
#include <glm/ext/matrix_float4x4.hpp>
#include "mesh.h"

namespace core {
    class Model {
    private:
        std::vector<core::Mesh> meshes;
        glm::mat4 modelMatrix;
        // option: add 'material properties' here:
        //float glossiness;
        //glm::vec3 objectColor;
        // Even better:
        //Material mat;  // -> create this class!
    public:
        Model(std::vector<core::Mesh> meshes) : meshes(meshes), modelMatrix(1) {}

        void render();

        void translate(glm::vec3 translation);
        void rotate(glm::vec3 axis, float radians);
        void scale(glm::vec3 scale);
        glm::mat4 getModelMatrix() const;
    };
}