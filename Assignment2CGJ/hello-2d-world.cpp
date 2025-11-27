////////////////////////////////////////////////////////////////////////////////
//
// Basic Triangle rendering
//
// This program demonstrates fundamental Modern OpenGL concepts by rendering
// two triangle instances directly in Clip Space. It serves as an introductory
// example for understanding the OpenGL graphics pipeline and basic shader
// programming.
//
// Key Concepts Demonstrated:
// - Vertex Array Objects (VAOs) and Vertex Buffer Objects (VBOs)
// - Shader program creation and management
// - Attribute and uniform handling
// - Basic transformation matrices
// - Clip space rendering without model/view/projection matrices
//
// Copyright (c) 2013-25 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#include <memory>

#include "../mgl/mgl.hpp"
#include "Shape.h"

std::vector<std::unique_ptr<Shape>> shapes;



////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
public:
  MyApp() = default;
  ~MyApp() override = default;

  void initCallback(GLFWwindow *win) override;
  void displayCallback(GLFWwindow *win, double elapsed) override;
  void windowCloseCallback(GLFWwindow *win) override;
  void windowSizeCallback(GLFWwindow *win, int width, int height) override;

private:
  const GLuint POSITION = 0, COLOR = 1;
  GLuint VaoId, VboId[2];
  std::unique_ptr<mgl::ShaderProgram> Shaders = nullptr;
  GLint MatrixId;

  void createShaderProgram();
  void createBufferObjects();
  void destroyBufferObjects();
  void drawScene();
};

//////////////////////////////////////////////////////////////////////// SHADERs

void MyApp::createShaderProgram() {
  Shaders = std::make_unique<mgl::ShaderProgram>();
  Shaders->addShader(GL_VERTEX_SHADER, "clip-vs.glsl");
  Shaders->addShader(GL_FRAGMENT_SHADER, "clip-fs.glsl");

  Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, POSITION);
  Shaders->addAttribute(mgl::COLOR_ATTRIBUTE, COLOR);
  Shaders->addUniform("Matrix");

  Shaders->create();

  MatrixId = Shaders->Uniforms["Matrix"].index;
}

//////////////////////////////////////////////////////////////////// VAOs & VBOs

typedef struct {
  GLfloat XYZW[4];
  GLfloat RGBA[4];
} Vertex;

const Vertex Vertices[] = {
    {{0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    {{0.4f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    {{0.0f, 0.4f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}};

const GLubyte Indices[] = {0, 1, 2};

////////////////////////////////////////////////////////////////////////// SCENE

const glm::mat4 I(1.0f);
//const glm::mat4 M =
//    glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 0.0f));
//vermelho
const glm::mat4 M1 =
    glm::translate(glm::mat4(1.0f), glm::vec3(-0.2f, -0.5f, 0.0f)) *
    glm::rotate(glm::mat4(1.0f), glm::radians(333.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//azul
const glm::mat4 M2 =
    glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, -0.5f, 0.0f)) *
    glm::rotate(glm::mat4(1.0f), glm::radians(18.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//roxo
const glm::mat4 M3 =
    glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.5f, 0.0f)) *
    glm::rotate(glm::mat4(1.0f), glm::radians(198.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
    glm::scale(glm::mat4(1.0f), glm::vec3(0.75f, 0.75f, 1.0f));
//laranja
const glm::mat4 M4 =
    glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, 0.5f, 0.0f)) *
    glm::rotate(glm::mat4(1.0f), glm::radians(63.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
    glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 1.0f));
//verde
const glm::mat4 M5 =
    glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f)) *
    glm::rotate(glm::mat4(1.0f), glm::radians(153.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
    glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 1.0f));


glm::mat4 pos1 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.2f, -0.5f, 0.0f));
glm::mat4 pos2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, -0.5f, 0.0f));
glm::mat4 pos3 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.5f, 0.0f));
glm::mat4 pos4 = glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, 0.5f, 0.0f));
glm::mat4 pos5 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));




void MyApp::drawScene() {
  // Drawing directly in clip space

  //glBindVertexArray(VaoId);
  Shaders->bind();

  //glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(I));
  //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
  //               reinterpret_cast<GLvoid *>(0));
  /*
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M1));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
                 reinterpret_cast<GLvoid *>(0));

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M2));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M3));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M4));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M5));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));
      */
  /*
  for (auto& s : shapes) {
      s->draw(MatrixId, Shaders->Uniforms["Cor"].index);
  }*/
  
  shapes[0]->changeColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  shapes[0]->draw(MatrixId, Shaders->Uniforms["Cor"].index, glm::vec2(0.0f, 0.0f), 0.0f, 1.0f);
  shapes[1]->draw(MatrixId, Shaders->Uniforms["Cor"].index, glm::vec2(-0.2f, 0.2f), 0.0f, 0.5f);
  shapes[0]->changeColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
  shapes[0]->draw(MatrixId, Shaders->Uniforms["Cor"].index, glm::vec2(0.0f, 0.2f), 180.0f, 0.5f);
  shapes[0]->changeColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
  shapes[0]->draw(MatrixId, Shaders->Uniforms["Cor"].index, glm::vec2(0.4f, 0.0f), 45.0f, 1.0f);
  shapes[2]->draw(MatrixId, Shaders->Uniforms["Cor"].index, glm::vec2(0.0f, 0.0f), -180.0f, 0.5f);
  shapes[0]->changeColor(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
  shapes[0]->draw(MatrixId, Shaders->Uniforms["Cor"].index, glm::vec2(0.6828f, 0.2828f), 90.0f, 0.5f);
  shapes[0]->changeColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
  shapes[0]->draw(MatrixId, Shaders->Uniforms["Cor"].index, glm::vec2(-0.2f, 0.6f), 225.0f, 0.707f);
  /*
  shapes[0]->changeColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  shapes[0]->draw(MatrixId, Shaders->Uniforms["Cor"].index, glm::vec2(0.0f, 0.0f), 333.0f, 1.0f);
  shapes[0]->changeColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
  shapes[0]->draw(MatrixId, Shaders->Uniforms["Cor"].index, glm::vec2(0.3f, -0.5f), 18.0f, 1.0f);
  shapes[0]->changeColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
  shapes[0]->draw(MatrixId, Shaders->Uniforms["Cor"].index, glm::vec2(-0.5f, 0.8f), 198.0f, 0.707f);
  shapes[0]->changeColor(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
  shapes[0]->draw(MatrixId, Shaders->Uniforms["Cor"].index, glm::vec2(0.3f, 0.5f), 63.0f, 0.5f);
  shapes[0]->changeColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
  shapes[0]->draw(MatrixId, Shaders->Uniforms["Cor"].index, glm::vec2(-0.5f, -0.7f), 153.0f, 0.5f);

  shapes[1]->draw(MatrixId, Shaders->Uniforms["Cor"].index, glm::vec2(-0.8f, 0.3f), 333.0f, 0.5f);

  shapes[2]->draw(MatrixId, Shaders->Uniforms["Cor"].index, glm::vec2(-0.8f, -0.2f), 333.0f, 0.5f);
  
  shapes[0]->changeColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  shapes[0]->draw(MatrixId, Shaders->Uniforms["Cor"].index, { 0.0f,0.0f }, 0.0f, 1.0f);        // triângulo 1
  shapes[0]->changeColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  shapes[0]->draw(MatrixId, Shaders->Uniforms["Cor"].index, { 0.4f,0.0f }, 90.0f, 1.0f);       // triângulo 2 encostado
  */
  Shaders->unbind();
  //glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win) {
  createShaderProgram();
  auto triangle = std::make_unique<Shape>(
      std::vector<glm::vec2>{{0, 0}, { 0.4f,0 }, { 0,0.4f }},
      glm::vec4(1, 0, 0, 1),
      0.0f,
      1.0f
  );

  //triangle->position = glm::vec2(-0.2f, -0.5f);
  shapes.push_back(std::move(triangle));
  std::vector<glm::vec2> squareVerts = {
    {0.0f, 0.0f},  // V0 - canto inferior esquerdo
    {0.4f, 0.0f},  // V1 - canto inferior direito
    {0.0f, 0.4f},  // V2 - canto superior esquerdo (triângulo 1)

    {0.0f, 0.4f},  // V3 - canto superior esquerdo (triângulo 2)
    {0.4f, 0.0f},  // V4 - canto inferior direito (triângulo 2)
    {0.4f, 0.4f}   // V5 - canto superior direito (triângulo 2)
  };
  auto square = std::make_unique<Shape>(
      squareVerts,
      glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), // cor verde
      0.0f,  // rotação
      0.5f   // escala
  );
  //square->position = glm::vec2(0.2f, 0.2f);
  shapes.push_back(std::move(square));

  std::vector<glm::vec2> losangVerts = {
    {0.0f, 0.0f},  // V0 - canto inferior esquerdo
    {0.4f, 0.0f},  // V1 - canto inferior direito
    {0.0f, 0.4f},  // V2 - canto superior esquerdo (triângulo 1)

    {0.0f, 0.0f},  // V3 - canto superior esquerdo (triângulo 2)
    {0.4f, -0.4f},  // V4 - canto inferior direito (triângulo 2)
    {0.4f, 0.0f}   // V5 - canto superior direito (triângulo 2)
  };
  auto losang = std::make_unique<Shape>(
      losangVerts,
      glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), // cor amarela
      0.0f,  // rotação
      0.5f   // escala
  );
  shapes.push_back(std::move(losang));
}

void MyApp::windowCloseCallback(GLFWwindow *win) {}

void MyApp::windowSizeCallback(GLFWwindow *win, int winx, int winy) {
  glViewport(0, 0, winx, winy);
}

void MyApp::displayCallback(GLFWwindow *win, double elapsed) { drawScene(); }

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char *argv[]) {
  mgl::Engine &engine = mgl::Engine::getInstance();
  engine.setApp(new MyApp());
  engine.setOpenGL(4, 6);
  engine.setWindow(600, 600, "Hello Modern 2D World", 0, 1);
  engine.init();
  engine.run();
  exit(EXIT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////// END
