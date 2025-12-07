#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;

out vec4 FragmentColor;

uniform vec3 uColor;

vec3 constantColor(void) {
    return vec3(0.5);
}

vec3 positionColor(void) {
    return (exPosition + vec3(1.0)) * 0.5;
}

vec3 uvColor(void) {
    return vec3(exTexcoord, 0.0);
}

vec3 normalColor(void) {
    return (exNormal + vec3(1.0)) * 0.5;
}

vec3 diffuseColor(void) {
    vec3 N = normalize(exNormal);
    vec3 direction = vec3(1.0, 0.5, 0.25);
    float intensity = max(dot(direction, N), 0.0);
    return vec3(intensity);
}

void main(void)
{

    vec3 N = normalize(exNormal);

    float variation = 0.3 * (abs(N.x) + abs(N.y) + abs(N.z));


    vec3 finalColor = clamp(uColor + variation, 0.0, 1.0);

    FragmentColor = vec4(finalColor, 1.0);
}
