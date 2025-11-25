#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>



class Shape
{
typedef struct {
    GLfloat XYZW[4];
    GLfloat RGBA[4];
} Vertex;
public:
    glm::vec4 color;
    float rotation;
    glm::vec2 position;
    float scale;

    // OpenGL buffers
    GLuint vao = 0;
    GLuint vboVertices = 0;
    GLuint vboIndices = 0;

    // Geometria (3 ou 6 vértices)
    std::vector<Vertex> vertices;
    std::vector<GLubyte> indices;

    Shape(const std::vector<glm::vec2>& points,
        glm::vec4 color,
        float rotation,
        float scale);

    ~Shape();

    void draw(GLint matrixUniform, GLint colorUniform) const;
    glm::mat4 getModelMatrix() const;

private:
    void createBuffers();
};
