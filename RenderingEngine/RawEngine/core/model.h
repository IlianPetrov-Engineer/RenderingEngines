#pragma once

#include <vector>
#include <glm/ext/matrix_float4x4.hpp>
#include "mesh.h"

namespace core {
    class Model {
    private:
        std::vector<core::Mesh> meshes;
        glm::mat4 modelMatrix;

        glm::vec3 objectColor = glm::vec3(1.0f);
        float specularStrength = 32.0f;
        
    public:
        Model(std::vector<core::Mesh> meshes) : meshes(meshes), modelMatrix(1) {}

        void render();

        void translate(glm::vec3 translation);
        void rotate(glm::vec3 axis, float radians);
        void scale(glm::vec3 scale);
        glm::mat4 getModelMatrix() const;

        void setObjectColor(const glm::vec3& color) 
        { 
            objectColor = color; 
        }

        void setSpecularStrength(float strength) 
        { 
            specularStrength = strength; 
        }

        glm::vec3 getObjectColor() const 
        { 
            return objectColor; 
        }

        float getSpecularStrength() const 
        { 
            return specularStrength; 
        }
    };
}