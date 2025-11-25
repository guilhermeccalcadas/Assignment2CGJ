#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Triangle {
public:
    float rotation;
    float scale;
    glm::vec4 color;

    Triangle(glm::vec4 c, float s = 1.0f, float r = 0.0f);

    glm::mat4 getTriangle() const;
};



