#version 330 core

const int TAM_POINT_LIGHTS = 6;
const int MAX_SPOT_LIGHTS = 4; 

in vec4 vColor;
in vec3 normal;
in vec3 posFrag;
in vec2 uvFrag;
in vec4 directionalLightSpacePos;

struct BaseLight
{
    vec3 color;
    float ambientInten;
    float diffuseInten;
};

struct DirectionalLight
{
    BaseLight base;
    vec3 lightDir;
};

struct PointLight
{
    BaseLight base;
    vec3 pos;
    float constV;
    float linearV;
    float exponentialV;
};

// Estructura SpotLight
struct SpotLight
{
    BaseLight base;
    vec3 pos;
    vec3 direction;
    float constV;
    float linearV;
    float exponentialV;
    float edge;  // coseno del angulo de corte
};

uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform PointLight pointLights[TAM_POINT_LIGHTS];
uniform int numPointLights;

// Uniforms para SpotLights
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int numSpotLights;

uniform vec3 cameraPos;
uniform float specularInten;
uniform float shininess;
uniform float time;

uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D directionalShadowMap;

out vec4 colour;


float CalcDirectionalShadowFactor(DirectionalLight light, vec3 norm)
{

    vec3 projCoords = directionalLightSpacePos.xyz / directionalLightSpacePos.w;

    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0)
    {
        return 0.0;
    }

    if(projCoords.x < 0.0 || projCoords.x > 1.0 ||
       projCoords.y < 0.0 || projCoords.y > 1.0)
    {
        return 0.0;
    }

    vec3 lightDir = normalize(light.lightDir);

    float bias = max(0.005 * (1.0 - dot(norm, lightDir)), 0.0005);

    float currentDepth = projCoords.z;
    float shadow = 0.0;

    vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);

    for(int x = -1; x <= 1; x++)
    {
        for(int y = -1; y <= 1; y++)
        {
            float closestDepth = texture(
                directionalShadowMap,
                projCoords.xy + vec2(x, y) * texelSize
            ).r;

            if(currentDepth - bias > closestDepth)
            {
                shadow += 1.0;
            }
        }
    }

    shadow /= 9.0;

    return shadow;
}

mat3 CalculateTBN(vec3 worldPos, vec3 worldNormal, vec2 uv)
{
    vec3 dp1 = dFdx(worldPos);
    vec3 dp2 = dFdy(worldPos);

    vec2 duv1 = dFdx(uv);
    vec2 duv2 = dFdy(uv);

    vec3 N = normalize(worldNormal);

    vec3 T = normalize(dp1 * duv2.y - dp2 * duv1.y);
    vec3 B = normalize(-dp1 * duv2.x + dp2 * duv1.x);

    return mat3(T, B, N);
}


vec3 GetNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, uvFrag).rgb;

    tangentNormal = tangentNormal * 2.0 - 1.0;

    mat3 TBN = CalculateTBN(posFrag, normal, uvFrag);

    vec3 finalNormal = normalize(TBN * tangentNormal);

    return finalNormal;
}



vec3 CalculateLightValues(BaseLight bl, vec3 norm, vec3 lDir, vec3 viewDir, float shadow)
{
    // Ambiente
    vec3 ambient = bl.ambientInten * bl.color;

    // Difusa
    float diff = max(dot(norm, lDir), 0.0);
    vec3 diffuse = bl.color * bl.diffuseInten * diff;

    // Especular tipo Blinn-Phong
    vec3 halfWayVec = normalize(lDir + viewDir);
    float spec = pow(max(dot(norm, halfWayVec), 0.0), shininess);
    vec3 specular = bl.color * specularInten * spec;

    return ambient + (diffuse + specular) * (1.0 - shadow);
}


vec3 CalculateDirectionalLight(DirectionalLight dirLight, vec3 norm, float shadow)
{
    vec3 lDir = normalize(-dirLight.lightDir);
    vec3 viewDir = normalize(cameraPos - posFrag);

    return CalculateLightValues(dirLight.base, norm, lDir, viewDir, shadow);
}



vec3 CalculatePointLight(PointLight pLight, vec3 norm)
{
    vec3 lDir = normalize(pLight.pos - posFrag);
    vec3 viewDir = normalize(cameraPos - posFrag);

    float d = distance(posFrag, pLight.pos);

    float attenuation =
        1.0 / (pLight.exponentialV * d * d +
               pLight.linearV * d +
               pLight.constV);

    return CalculateLightValues(pLight.base, norm, lDir, viewDir, 0.0) * attenuation;
}

// Funcion para calcular SpotLight
vec3 CalculateSpotLight(SpotLight sLight, vec3 norm)
{
    // Vector desde la luz hacia el fragmento (para comparar con la direccion del foco)
    vec3 lightToFrag = normalize(posFrag - sLight.pos);
    vec3 viewDir = normalize(cameraPos - posFrag);
    
    // theta: cuanto se alinea la direccion del foco con el vector luz->fragmento
    // Si theta > edge (coseno), el fragmento esta dentro del cono
    float theta = dot(lightToFrag, normalize(sLight.direction));
    
    if(theta > sLight.edge)
    {
        // Atenuacion por distancia
        float d = distance(posFrag, sLight.pos);
        float attenuation = 1.0 / (sLight.exponentialV * d * d +
                                    sLight.linearV * d +
                                    sLight.constV);
        
        // Suavizado del borde: zona de transicion de 5 grados (~cos margen)
        // epsilon es el borde interior (mas cerrado que edge), epsilon > edge en cosenos
        float epsilon = sLight.edge + 0.05;
        float intensity = clamp((theta - sLight.edge) / (epsilon - sLight.edge), 0.0, 1.0);
        
        // lDir apunta del fragmento hacia la luz (convencion correcta para iluminacion)
        vec3 lDir = -lightToFrag;
        
        vec3 lightResult = CalculateLightValues(sLight.base, norm, lDir, viewDir, 0.0);
        
        return lightResult * attenuation * intensity;
    }
    
    return vec3(0.0);
}


void main()
{
    vec3 texColor = texture(colorMap, uvFrag).rgb;

    vec3 norm = GetNormalFromMap();

    vec3 luzFinal = vec3(0.0);

    // Luz direccional con sombras
    float shadow = CalcDirectionalShadowFactor(directionalLight, norm);
    luzFinal += CalculateDirectionalLight(directionalLight, norm, shadow);

    // Point light unica
    luzFinal += CalculatePointLight(pointLight, norm);

    // Array de point lights
    for(int i = 0; i < numPointLights && i < TAM_POINT_LIGHTS; i++)
    {
        luzFinal += CalculatePointLight(pointLights[i], norm);
    }

    // Calcular SpotLights
    for(int i = 0; i < numSpotLights && i < MAX_SPOT_LIGHTS; i++)
    {
        luzFinal += CalculateSpotLight(spotLights[i], norm);
    }

    colour = vec4(texColor * luzFinal, 1.0);

}