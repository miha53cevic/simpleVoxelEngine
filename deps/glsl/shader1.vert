#version 330

in vec3 position;
in vec2 textureCoords;

out vec2 pass_texture;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transformationMatrix;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * transformationMatrix * vec4(position, 1.0);
	pass_texture = textureCoords;
}