////////////////////////////////////////////////////////////////////////////////
//
//  Loading meshes from external files
//
// Copyright (c) 2023-25 by Carlos Martinho
//
// INTRODUCES:
// MODEL DATA, ASSIMP, mglMesh.hpp
//
////////////////////////////////////////////////////////////////////////////////

#include "../mgl/mgl.hpp"

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
public:
  void initCallback(GLFWwindow *win) override;
  void displayCallback(GLFWwindow *win, double elapsed) override;
  void windowSizeCallback(GLFWwindow *win, int width, int height) override;
  void keyCallback(GLFWwindow *win, int key, int scancode, int action, int mods) override;
  void scrollCallback(GLFWwindow *win, double xoffset, double yoffset) override;
  void mouseButtonCallback(GLFWwindow* win, int button, int action, int mods) override;
  void cursorCallback(GLFWwindow* window, double xpos, double ypos) override;

private:
  // Camera control parameters
  bool useOrtho = false;
  bool useCamera2 = false;
  float radius = 10.0f;
  const float zoomSpeed = 1.0f;
  const float minRadius = 2.0f;
  const float maxRadius = 50.0f;
  struct CameraState {
      glm::mat4 viewMatrix;
      glm::quat rotation;
      float radius;
  };
  CameraState cam1;
  CameraState cam2;
  CameraState* activeCam = nullptr;
  glm::vec3 target = glm::vec3(0.0f);

  bool rightMousePressed = false;
  double lastCameraPosX = 0.0f;
  double lastCameraPosY = 0.0f;
  float rotationSpeed = 0.006f;
  float pitchLimit = glm::radians(89.0f);

  const GLuint UBO_BP = 0;
  mgl::ShaderProgram *Shaders = nullptr;
  mgl::Camera *Camera = nullptr;
  GLint ModelMatrixId;
  mgl::Mesh *Mesh = nullptr;
  mgl::Mesh* Mesh2 = nullptr;
  mgl::Mesh* Mesh3 = nullptr;
  mgl::Mesh* Mesh4 = nullptr;
  mgl::Mesh* Mesh5 = nullptr;
  mgl::Mesh* Mesh6 = nullptr;
  mgl::Mesh* Mesh7 = nullptr;
  mgl::Mesh* Mesh8 = nullptr;
  std::vector<mgl::Mesh*> MeshesList;


  void createMeshes();
  void createShaderPrograms();
  void createCamera();
  void drawScene();
  void updateCamera();
  void drawMesh(mgl::Mesh* m, glm::vec3 pos, float rotX, float rotY, float rotZ, float scal);
};

///////////////////////////////////////////////////////////////////////// MESHES

void MyApp::createMeshes() {
  std::string mesh_dir = "./assets/models/";
  // std::string mesh_file = "cube-v.obj";
  // std::string mesh_file = "cube-vn-flat.obj";
  // std::string mesh_file = "cube-vn-smooth.obj";
  // std::string mesh_file = "cube-vt.obj";
  // std::string mesh_file = "cube-vt2.obj";
  // std::string mesh_file = "torus-vtn-flat.obj";
  // std::string mesh_file = "torus-vtn-smooth.obj";
  // std::string mesh_file = "suzanne-vtn-flat.obj";
  // std::string mesh_file = "suzanne-vtn-smooth.obj";
  // std::string mesh_file = "teapot-vn-flat.obj";
  // std::string mesh_file = "teapot-vn-smooth.obj";
  //std::string mesh_file = "bunny-vn-flat.obj";
  // std::string mesh_file = "bunny-vn-smooth.obj";

  /*
  std::string mesh_file = "Triangle1.obj";
  std::string mesh_file = "Triangle2.obj";
  std::string mesh_file = "Triangle4.obj";
  std::string mesh_file = "Triangle6.obj";
  std::string mesh_file = "Triangle7.obj";
  std::string mesh_file = "Cube5.obj";
  std::string mesh_file = "Paralelogram3a.obj";
  std::string mesh_file = "Table.obj";
  std::string mesh_fullname = mesh_dir + mesh_file;

  Mesh = new mgl::Mesh();
  Mesh->joinIdenticalVertices();
  Mesh->create(mesh_fullname);
  */
  Mesh = new mgl::Mesh();
  Mesh->joinIdenticalVertices();
  Mesh->create(mesh_dir + "Triangle1.obj");

  Mesh2 = new mgl::Mesh();
  Mesh2->joinIdenticalVertices();
  Mesh2->create(mesh_dir + "Triangle2.obj");

  Mesh3 = new mgl::Mesh();
  Mesh3->joinIdenticalVertices();
  Mesh3->create(mesh_dir + "Triangle4.obj");

  Mesh4 = new mgl::Mesh();
  Mesh4->joinIdenticalVertices();
  Mesh4->create(mesh_dir + "Triangle6.obj");

  Mesh5 = new mgl::Mesh();
  Mesh5->joinIdenticalVertices();
  Mesh5->create(mesh_dir + "Triangle7.obj");

  Mesh6 = new mgl::Mesh();
  Mesh6->joinIdenticalVertices();
  Mesh6->create(mesh_dir + "Paralelogram3a.obj");

  Mesh7 = new mgl::Mesh();
  Mesh7->joinIdenticalVertices();
  Mesh7->create(mesh_dir + "Table.obj");

  Mesh8 = new mgl::Mesh();
  Mesh8->joinIdenticalVertices();
  Mesh8->create(mesh_dir + "Cube5.obj");

  MeshesList.push_back(Mesh);
  MeshesList.push_back(Mesh2);
  MeshesList.push_back(Mesh3);
  MeshesList.push_back(Mesh4);
  MeshesList.push_back(Mesh5);
  MeshesList.push_back(Mesh6);
  MeshesList.push_back(Mesh7);
  MeshesList.push_back(Mesh8);
}

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderPrograms() {
  Shaders = new mgl::ShaderProgram();
  Shaders->addShader(GL_VERTEX_SHADER, "cube-vs.glsl");
  Shaders->addShader(GL_FRAGMENT_SHADER, "cube-fs.glsl");

  Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
  if (Mesh->hasNormals()) {
    Shaders->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
  }
  if (Mesh->hasTexcoords()) {
    Shaders->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);
  }
  if (Mesh->hasTangentsAndBitangents()) {
    Shaders->addAttribute(mgl::TANGENT_ATTRIBUTE, mgl::Mesh::TANGENT);
  }

  Shaders->addUniform(mgl::MODEL_MATRIX);
  Shaders->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
  Shaders->create();

  ModelMatrixId = Shaders->Uniforms[mgl::MODEL_MATRIX].index;
}

///////////////////////////////////////////////////////////////////////// CAMERA

// Eye(5,5,5) Center(0,0,0) Up(0,1,0)
const glm::mat4 ViewMatrix1 =
    glm::lookAt(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));

// Eye(-5,-5,-5) Center(0,0,0) Up(0,1,0)
const glm::mat4 ViewMatrix2 =
    glm::lookAt(glm::vec3(-5.0f, -5.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));

// Orthographic LeftRight(-2,2) BottomTop(-2,2) NearFar(1,10)
const glm::mat4 ProjectionMatrix1 =
    glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 1.0f, 10.0f);

// Perspective Fovy(30) Aspect(640/480) NearZ(1) FarZ(10)
const glm::mat4 ProjectionMatrix2 =
    glm::perspective(glm::radians(30.0f), 640.0f / 480.0f, 1.0f, 10.0f);

void MyApp::createCamera() {
  Camera = new mgl::Camera(UBO_BP);

  // Definir posi��o inicial das c�meras
  glm::vec3 eye1 = glm::vec3(5.0f, 5.0f, 5.0f);
  glm::vec3 eye2 = glm::vec3(-5.0f, -5.0f, -5.0f);

  // Calcular radius (dist�ncia ao target)
  cam1.radius = glm::length(eye1);
  cam2.radius = glm::length(eye2);

  // Calcular quaternion olhando para o target (0,0,0)
  cam1.rotation = glm::quatLookAt(glm::normalize(-eye1), glm::vec3(0, 1, 0));
  cam2.rotation = glm::quatLookAt(glm::normalize(-eye2), glm::vec3(0, 1, 0));

  // Inicializar viewMatrix das c�meras
  cam1.viewMatrix = glm::lookAt(eye1, target, glm::vec3(0, 1, 0));
  cam2.viewMatrix = glm::lookAt(eye2, target, glm::vec3(0, 1, 0));

  // Definir c�mera ativa
  activeCam = &cam1;
  Camera->setViewMatrix(activeCam->viewMatrix);
  Camera->setProjectionMatrix(ProjectionMatrix2);

  //Camera->setViewMatrix(ViewMatrix1);
  //Camera->setProjectionMatrix(ProjectionMatrix2);
}

/////////////////////////////////////////////////////////////////////////// DRAW

glm::mat4 ModelMatrix;

glm::mat4 getModel(glm::vec3 pos, float rotX, float rotY, float rotZ, float scal) {
    glm::mat4 M = glm::translate(glm::mat4(1.0f), pos)
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotX), glm::vec3(1, 0, 0))
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotY), glm::vec3(0, 1, 0))
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotZ), glm::vec3(0, 0, 1))
        * glm::scale(glm::mat4(1.0f), glm::vec3(scal, scal, 1.0f));
    return M;
}

void MyApp::drawMesh(mgl::Mesh* m, glm::vec3 pos, float rotX, float rotY, float rotZ, float scal) {
    ModelMatrix = getModel(pos, rotX, rotY, rotZ, scal);
    glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
    m->draw();
}

void MyApp::drawScene() {
    Shaders->bind();

    drawMesh(MeshesList[0], glm::vec3(-0.445f, 0.5f, 0.0f), 90.0f, 0.0f, -90.0f, 1.0f);
    drawMesh(MeshesList[1], glm::vec3(0.0f, 0.5f, -0.445f), 90.0f, 0.0f, 0.0f, 1.0f);
    drawMesh(MeshesList[2], glm::vec3(0.0f, 0.5f, 0.2225f), 90.0f, 0.0f, 90.0f, 1.0f);
    drawMesh(MeshesList[3], glm::vec3(0.445f, 0.5f, -0.2225f), 90.0f, 0.0f, 0.0f, 1.0f);
    drawMesh(MeshesList[4], glm::vec3(0.2225f, 0.5f, 0.2225f), 90.0f, 0.0f, 0.0f, 1.0f);
    drawMesh(MeshesList[5], glm::vec3(-0.11125f, 0.5f, 0.33375f), 90.0f, 0.0f, 0.0f, 1.0f); //paralellogram
    drawMesh(MeshesList[6], glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.0f, 1.0f); //table
    drawMesh(MeshesList[7], glm::vec3(0.2225f, 0.5f, 0.0f), 90.0f, 0.0f, 0.0f, 1.0f); //square

    /*
    for (mgl::Mesh* m : MeshesList) {
        if (m) m->draw();
    }
    */

    Shaders->unbind();
}


void MyApp::updateCamera() {
    glm::vec3 initialPos(0.0f, 0.0f, activeCam->radius);
    glm::vec3 rotatedPos = activeCam->rotation * initialPos;
    glm::vec3 localUp = activeCam->rotation * glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 view = glm::lookAt(rotatedPos + target, target, glm::normalize(localUp));
    activeCam->viewMatrix = view;

    Camera->setViewMatrix(activeCam->viewMatrix);
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win) {
  createMeshes();
  createShaderPrograms(); // after mesh;
  createCamera();
}

void MyApp::windowSizeCallback(GLFWwindow *win, int winx, int winy) {
  glViewport(0, 0, winx, winy);
  // change projection matrices to maintain aspect ratio
}

void MyApp::displayCallback(GLFWwindow *win, double elapsed) { drawScene(); }

void MyApp::keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) return;

    switch (key) {

    case GLFW_KEY_P:
        useOrtho = !useOrtho;
        Camera->setProjectionMatrix(useOrtho ? ProjectionMatrix1 : ProjectionMatrix2);
        break;

    case GLFW_KEY_C:
        if (activeCam == &cam1)
            activeCam = &cam2;
        else
            activeCam = &cam1;

        Camera->setViewMatrix(activeCam->viewMatrix);
        break;


    default:
        break;
    }
}

void MyApp::scrollCallback(GLFWwindow* win, double xoffset, double yoffset) {
    if (!useOrtho) {
        activeCam->radius -= yoffset * zoomSpeed;

        if (activeCam->radius < minRadius) activeCam->radius = minRadius;
        if (activeCam->radius > maxRadius) activeCam->radius = maxRadius;

        updateCamera();
    }
}

void MyApp::mouseButtonCallback(GLFWwindow* win, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            rightMousePressed = true;
            glfwGetCursorPos(win, &lastCameraPosX, &lastCameraPosY);
        }
        else if (action == GLFW_RELEASE) {
            rightMousePressed = false;
        }
    }
}

void MyApp::cursorCallback(GLFWwindow* window, double xpos, double ypos) {
    if (!rightMousePressed) {
        lastCameraPosX = xpos;
        lastCameraPosY = ypos;
        return;
    }

    double distanceX = xpos - lastCameraPosX;
    double distanceY = ypos - lastCameraPosY;
    float yawAngle = -distanceX * rotationSpeed;
    float pitchAngle = -distanceY * rotationSpeed;

    glm::vec3 localUp = activeCam->rotation * glm::vec3(0.0f, 1.0f, 0.0f);
    glm::quat yaw = glm::angleAxis(yawAngle, glm::normalize(localUp));
    glm::vec3 localRight = activeCam->rotation * glm::vec3(1.0f, 0.0f, 0.0f);
    glm::quat pitch = glm::angleAxis(pitchAngle, glm::normalize(localRight));

    // Apply yaw and pitch to the current rotation
    glm::quat orientation = glm::normalize(pitch * yaw * activeCam->rotation);

    activeCam->rotation = orientation;
    updateCamera();

    lastCameraPosX = xpos;
    lastCameraPosY = ypos;
}

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char *argv[]) {
  mgl::Engine &engine = mgl::Engine::getInstance();
  engine.setApp(new MyApp());
  engine.setOpenGL(4, 6);
  engine.setWindow(800, 600, "Mesh Loader", 0, 1);
  engine.init();
  engine.run();
  exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
