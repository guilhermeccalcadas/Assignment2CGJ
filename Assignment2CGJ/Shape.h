#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>



class Shape {

typedef struct {
    GLfloat XYZW[4];
    GLfloat RGBA[4];
} Vertex;

public:
    glm::vec4 color;
    //float rotation;
    //glm::vec2 position;
    //float scale;

    //Buffers
    GLuint VaoId, VboId[2];

    //Substitute later when we have sub classes?
    std::vector<Vertex> vertices;
    std::vector<GLubyte> indices;

    Shape(const std::vector<glm::vec4>& points,const std::vector<GLubyte> index,glm::vec4 color);

    ~Shape();

    void changeColor(glm::vec4 newColor);

    void draw(GLint matrixUniform, GLint colorUniform, glm::vec2 pos, float rot, float scal);
    glm::mat4 getModelMatrix(glm::vec2 pos, float rot, float scal);

private:
    const GLuint POSITION = 0, COLOR = 1;

    void createBuffers();
};
