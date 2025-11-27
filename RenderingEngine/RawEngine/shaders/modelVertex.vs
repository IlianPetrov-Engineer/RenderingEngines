#version 400
// position, normal (in object space) + uv
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;
layout (location = 2) in vec2 aUv;
uniform mat4 mvpMatrix;
out vec3 fPos;
out vec3 fNor;
out vec2 uv;

void main() {
	// just pass the position, normal and UV to the fragment shader:
  fPos = aPos;
  fNor = aNor;
  uv = aUv;

  gl_Position = mvpMatrix * vec4(aPos, 1.0); // transforms the model vertex positions from object space all the way to screen space (more or less)
}