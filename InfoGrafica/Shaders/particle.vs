#version 330 core

layout (location = 0) in vec2 localPos;
layout (location = 1) in vec2 texCoord;


layout (location = 2) in vec3 instancePosition;
layout (location = 3) in float instanceSize;
layout (location = 4) in float instanceAlpha;

out vec2 fragTex;
out float fragAlpha;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    mat4 invView = inverse(view);

    vec3 cameraRight = normalize(vec3(invView[0]));
    vec3 cameraUp    = normalize(vec3(invView[1]));

    vec3 worldPosition =
        instancePosition +
        cameraRight * localPos.x * instanceSize +
        cameraUp    * localPos.y * instanceSize;

    fragTex = texCoord;
    fragAlpha = instanceAlpha;

    gl_Position = projection * view * vec4(worldPosition, 1.0);
}