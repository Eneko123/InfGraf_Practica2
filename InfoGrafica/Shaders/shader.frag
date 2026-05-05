#version 330
in vec4 vColor;
in vec3 normal;
in vec3 PosFrag;

struct DirectionalLight{
    vec3 color;
    float ambientInten;
    vec3 lightDir;
    float diffuseInten;
};

uniform DirectionalLight directionalLight;
uniform vec3 cameraPos = vec3(0,0,0);
uniform float specularStrength = 10.0f;
uniform float shininess = 32.0f;

out vec4 colour;
void main(){
    vec3 norm = normalize(normal);
    vec3 lDir = normalize(directionalLight.lightDir);
    vec3 viewDir = normalize(cameraPos - PosFrag));

    // Ambient
    vec3 ambient = directionalLight.ambientInten * directionalLight.color;
    // Diffuse
    float diff = max(dot(norm, lDir), 0.0f);
    vec3 diffuse = directionalLight.color * directionalLight.diffuseInten * diff;
    //Specular
    vec3 halfWayVec = normalize(lDir + viewDir);
    float spec = pow(max(dot(norm, halfWayVec), 0.0f), shininess);
    vec3 specular = directionalLight.color * specularStrength * spec;

    vec3 luzFinal = ambient + diffuse + specular;

    colour = vec4(luzFinal, 1.0f);
}