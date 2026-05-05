#version 330

const int TAM_POINT_LIGHTS = 6;

in vec4 vColor;
in vec3 normal;
in vec3 posFrag;
in vec2 uvFrag;

struct BaseLight{
    vec3 color;
    float ambientInten;
    float diffuseInten;
};

struct DirectionalLight{
    BaseLight base;
    vec3 lightDir;
};

struct PointLight{
    BaseLight base;
    vec3 pos;
    float constV;
    float linearV;
    float exponentialV;
};

uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform PointLight[TAM_POINT_LIGHTS] pointLights;
uniform int numPointLights;
uniform vec3 cameraPos;
uniform float specularInten;
uniform float shininess;
uniform float time;

uniform sampler2D colorMap;

out vec4 colour;

vec3 CalculateLightValues(BaseLight bl, vec3 norm, vec3 lDir, vec3 viewDir){
    //Ambient
    vec3 ambient = bl.ambientInten * bl.color;
    //Diffuse
    float diff = max(dot(norm, lDir), 0.0f);
    vec3 diffuse = bl.color * bl.diffuseInten * diff;
    //Specular
    vec3 halfWayVec = normalize(lDir + viewDir);
    float spec = pow(max(dot(norm, halfWayVec), 0.0f), shininess);
    vec3 specular = bl.color * specularInten * spec;
    return ambient + diffuse + specular;
}


vec3 CalculateDirectionalLight(DirectionalLight dirLight){
    vec3 norm = normalize(normal);
    vec3 lDir  = normalize(directionalLight.lightDir);
    vec3 viewDir = normalize(cameraPos - posFrag);

    return CalculateLightValues(dirLight.base, norm, lDir, viewDir);

}

vec3 CalculatePointLight(PointLight pLight){
    vec3 norm = normalize(normal);
    vec3 lDir  = normalize(posFrag - pLight.pos);
    vec3 viewDir = normalize(cameraPos - posFrag);

    float d = distance(posFrag, pLight.pos);
    float attenuation = 1/(pLight.exponentialV * d *d +  pLight.linearV * d + pLight.constV);

    return CalculateLightValues(pLight.base, norm, lDir, viewDir) * attenuation;
}

void main(){
    
    vec3 texColor = texture(colorMap, uvFrag).rgb;

    vec3 luzFinal = vec3(0.0f);
    luzFinal += CalculateDirectionalLight(directionalLight);
    luzFinal += CalculatePointLight(pointLight);

    colour = vec4(texColor*luzFinal, 1.0); 
}