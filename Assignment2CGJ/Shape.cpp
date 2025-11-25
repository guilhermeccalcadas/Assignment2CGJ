#include "Shape.h"
#include <mgl.hpp>
Shape::Shape(const std::vector<glm::vec2>& points,
    glm::vec4 color,
    float rotation,
    float scale)
    : color(color),
    rotation(rotation),
    position(glm::vec2(0.0f, 0.0f)),
    scale(scale)
{
    vertices.reserve(points.size());
    for (auto& p : points) {
        Vertex v;
        v.XYZW[0] = p.x;
        v.XYZW[1] = p.y;
        v.XYZW[2] = 0.0f;
        v.XYZW[3] = 1.0f;

        v.RGBA[0] = color.r;
        v.RGBA[1] = color.g;
        v.RGBA[2] = color.b;
        v.RGBA[3] = color.a;

        vertices.push_back(v);
    }

    indices.resize(points.size());
    for (int i = 0; i < indices.size(); i++)
        indices[i] = i;

    createBuffers();
}

Shape::~Shape()
{
    if (vao) glDeleteVertexArrays(1, &vao);
    if (vboVertices) glDeleteBuffers(1, &vboVertices);
    if (vboIndices) glDeleteBuffers(1, &vboIndices);
}

void Shape::createBuffers()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // VBO de vertices
    glGenBuffers(1, &vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
        vertices.data(), GL_STATIC_DRAW);

    // posição → atributo 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (void*)offsetof(Vertex, XYZW));

    // cor → atributo 1
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (void*)offsetof(Vertex, RGBA));

    // VBO de indices
    glGenBuffers(1, &vboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte),
        indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

glm::mat4 Shape::getModelMatrix() const
{
    glm::mat4 M(1.0f);
    M = glm::translate(M, glm::vec3(position, 0));
    M = glm::rotate(M, glm::radians(rotation), glm::vec3(0, 0, 1));
    M = glm::scale(M, glm::vec3(scale, scale, 1.0f));
    return M;
}

void Shape::draw(GLint matrixUniform, GLint colorUniform) const
{
    glm::mat4 M = getModelMatrix();

    glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, glm::value_ptr(M));
    glUniform4fv(colorUniform, 1, glm::value_ptr(color));

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_BYTE, 0);
    glBindVertexArray(0);
}


