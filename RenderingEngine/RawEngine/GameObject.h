#pragma once
#include "core/Model.h"
#include "core/texture.h"

class GameObject
{
public:
    core::Model model;
    core::Texture* texture;

    GameObject(const core::Model& model, core::Texture* texture);
};
