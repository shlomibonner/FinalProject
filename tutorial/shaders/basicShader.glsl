#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

uniform vec4 lightColor;
uniform sampler2D sampler1;
uniform vec4 lightDirection;
uniform vec2 trans;

out vec4 Color;
void main()
{
    Color = texture2D(sampler1, texCoord0) * vec4(1,1,1, trans.x);	
}