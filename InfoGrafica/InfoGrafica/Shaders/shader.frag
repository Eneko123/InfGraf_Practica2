#version 330
in vec4 vColor;
in vec3 normal;

uniform vec3 color;
uniform float ambientInten;
uniform vec3 lightDir;
uniform float diffuseInten;

out vec4 colour;
void main(){
    vec3 ambient = ambientInten * color;

    vec3 luzFinal = ambient;

    colour = vec4(vColor.rgb * luzFinal, 1.0f);
}