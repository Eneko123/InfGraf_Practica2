#version 330 core

in vec2 fragTex;
in float fragAlpha;

out vec4 colour;

uniform sampler2D colorMap;

void main()
{
    vec4 texColor = texture(colorMap, fragTex);

    float mask = texColor.r;

    float alpha = smoothstep(0.05, 1.0, mask) * fragAlpha;

    if (alpha < 0.02)
        discard;

    vec3 particleColor = vec3(0.55, 0.55, 0.55);

    colour = vec4(particleColor, alpha);
}