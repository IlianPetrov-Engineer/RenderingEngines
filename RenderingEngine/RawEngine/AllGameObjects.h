#pragma once
#include "GameObject.h"
#include "core/mesh.h"
#include "core/assimpLoader.h"
#include "core/texture.h"
#include <glm/glm.hpp>

class Suzanne : public GameObject
{
public:
    Suzanne();
    void Update(float holderr) override;
    void Render() override;

private:
    core::Model model;
};

class QuadImage : public GameObject
{
public:
    QuadImage();
    void Render() override;

private:
    core::Model model;
    core::Texture texture;
};
