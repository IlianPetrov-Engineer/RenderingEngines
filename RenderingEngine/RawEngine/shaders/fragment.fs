#version 400 core
uniform vec3 lightDirection; // TODO: set uniform. See BC3. Or see the main rendering loop...
out vec4 FragColor;
in vec3 fPos;
in vec3 fNor;
in vec2 uv;

void main()
{
	//lightDirection = vec3(fNor.x, fNor.y, fNor.z);
	// debug, show light direction:
	FragColor = vec4(lightDirection,1);
	// TODO: lighting calculations (start with diffuse: compare normal & lightDirection)
	

	// debug, show normals:
   //FragColor = vec4(fNor.x, fNor.y, fNor.z, 1);
}