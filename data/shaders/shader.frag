#version 330 core

in vec2 pass_textureCoords;
in vec3 pass_surfaceNormal;
in vec3 pass_toLightVector;

out vec4 out_Color;

uniform sampler2D textureSampler;
uniform vec3 lightColor;

void main() {
    vec3 unitNormal = normalize(pass_surfaceNormal);
    vec3 unitLightVector = normalize(pass_toLightVector);

    float nDotL = dot(unitNormal, unitLightVector);
    float brightness = max(nDotL, 0.0);
    vec3 diffuse = brightness * lightColor;

    out_Color = vec4(diffuse, 1.0) * texture(textureSampler, pass_textureCoords);
}
