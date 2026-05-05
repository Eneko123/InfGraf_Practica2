#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norms;
layout(location = 2) in vec2 uvs;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec3 normal;
out vec4 vColor;
out vec3 posFrag;
out vec2 uvFrag;
void main(){
    gl_Position = projection*view*model*vec4(pos.x, pos.y, pos.z, 1);
    vColor = vec4(clamp(pos.y, 0.0f, 1.0f), 0, 0, 1.0f);
    normal = normalize(mat3(transpose(inverse(model)))*norms);
    posFrag = (model*vec4(pos, 1.0f)).xyz;
    uvFrag = uvs;
}