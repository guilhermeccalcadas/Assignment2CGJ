#include "Triangle.h"

Triangle::Triangle(glm::vec4 c, float s, float r)
    : color(c), scale(s), rotation(r) {
}

glm::mat4 Triangle::getTriangle() const {
    glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 1.0f));
    return transform;
}
