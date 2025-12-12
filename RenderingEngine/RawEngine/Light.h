#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <string>

//class Light
//{
//    struct PointLight {
//        glm::vec3 position = glm::vec3(0.0f);
//        glm::vec3 color = glm::vec3(1.0f);
//        float intensity = 1.0f;
//
//        // Attenuation terms: 1.0 / (constant + linear * d + quadratic * d^2)
//        float constant = 1.0f;
//        float linear = 0.09f;
//        float quadratic = 0.032f;
//
//        PointLight() = default;
//        PointLight(const glm::vec3& pos, const glm::vec3& col, float inten = 1.0f,
//            float c = 1.0f, float l = 0.09f, float q = 0.032f)
//            : position(pos), color(col), intensity(inten),
//            constant(c), linear(l), quadratic(q) {
//        }
//
//        // Upload the light into a shader program.
//        // - program: GL shader program (already bound or not, glUseProgram recommended)
//        // - uniformBase: base name of the uniform (e.g. "light" or "lights")
//        // - index: when >=0, uploads to an array element: e.g. "lights[0].position"
//        //          when <0, uploads to a single struct: e.g. "light.position"
//        void uploadToShader(GLuint program, const std::string& uniformBase, int index = -1) const;
//    };
//};
