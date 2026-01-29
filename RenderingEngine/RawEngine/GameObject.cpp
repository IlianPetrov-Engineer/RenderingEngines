#include "GameObject.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

GameObject::GameObject(const core::Model& model, core::Texture* texture)
    : model(model), texture(texture)
{}