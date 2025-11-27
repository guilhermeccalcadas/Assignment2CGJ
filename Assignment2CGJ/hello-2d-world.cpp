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

//std::vector<std::unique_ptr<Shape>> shapes;
Shape *triangleBase;
Shape *squareBase;
Shape *paralelloBase;



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
  std::unique_ptr<mgl::ShaderProgram> Shaders = nullptr;
  GLint MatrixId;

  void createShaderProgram();
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

void MyApp::drawScene() {
  // Drawing directly in clip space

  Shaders->bind();
  
  //Red big triangle
  triangleBase->changeColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  triangleBase->draw(MatrixId, Shaders->Uniforms["Cor"].index, glm::vec3(0.0f, 0.0f, 0.0f), 333.0f, 1.0f);
  //Blue big triangle
  triangleBase->changeColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
  triangleBase->draw(MatrixId, Shaders->Uniforms["Cor"].index, glm::vec3(0.3564f, -0.1816f, 0.0f), 18.0f, 1.0f);
  //Medium purple triangle
  triangleBase->changeColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
  triangleBase->draw(MatrixId, Shaders->Uniforms["Cor"].index, glm::vec3(0.0938f, 0.6251f, 0.0f), 198.0f, 0.707f);
  //Small orange triangle
  triangleBase->changeColor(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
  triangleBase->draw(MatrixId, Shaders->Uniforms["Cor"].index, glm::vec3(0.7368f, -0.0580f, 0.0f), 63.0f, 0.5f);
  //Small cyan triangle
  triangleBase->changeColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
  triangleBase->draw(MatrixId, Shaders->Uniforms["Cor"].index, glm::vec3(0.0908f, 0.1782f, 0.0f), 153.0f, 0.5f);

  //Green square
  squareBase->draw(MatrixId, Shaders->Uniforms["Cor"].index, glm::vec3(-0.0875f, 0.2690f, 0.0f), 333.0f, 0.5f);

  //Yellow paralellogram
  paralelloBase->draw(MatrixId, Shaders->Uniforms["Cor"].index, glm::vec3(-0.1782f, 0.0908f, 0.0f), 333.0f, 0.5f);
  
  Shaders->unbind();
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win) {
  createShaderProgram();

  triangleBase = new Shape(std::vector<glm::vec4>{{0,0,0,1.0f}, {0.4f,0,0,1.0f}, {0,0.4f,0,1.0f}},
      std::vector<GLubyte>{0,1,2}, glm::vec4(1, 0, 0, 1.0f));

  std::vector<glm::vec4> squareVerts = {
    {0.0f, 0.0f, 0.0f, 1.0f}, {0.4f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.4f, 0.0f, 1.0f}, //triangle 1
    {0.0f, 0.4f, 0.0f, 1.0f}, {0.4f, 0.0f, 0.0f, 1.0f}, {0.4f, 0.4f, 0.0f, 1.0f}  //triangle 2
  };

  squareBase = new Shape(squareVerts,
      std::vector<GLubyte>{0,1,2,3,4,5,6}, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

  std::vector<glm::vec4> paralelloVerts = {
    {0.0f, 0.0f, 0.0f, 1.0f}, {0.4f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.4f, 0.0f, 1.0f}, //triangle 1
    {0.0f, 0.0f, 0.0f, 1.0f}, {0.4f, -0.4f, 0.0f, 1.0f}, {0.4f, 0.0f, 0.0f, 1.0f} //triangle 2
  };

  paralelloBase = new Shape(paralelloVerts,
      std::vector<GLubyte>{0, 1, 2, 3, 4, 5, 6}, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
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
