#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norms;
layout(location = 2) in vec2 uvs;
layout(location = 3) in vec2 tang;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 directionalLightTransform;

out vec3 normal;
out vec4 vColor;
out vec3 posFrag;
out vec2 uvFrag;
out vec4 directionalLightSpacePos;

void main()
{
    vec4 worldPosition = model * vec4(pos, 1.0);

    gl_Position = projection * view * worldPosition;

    directionalLightSpacePos = directionalLightTransform * worldPosition;

    vColor = vec4(clamp(pos.y, 0.0, 1.0), 0.0, 0.0, 1.0);

    normal = normalize(mat3(transpose(inverse(model))) * norms);

    posFrag = worldPosition.xyz;

    uvFrag = uvs;
}