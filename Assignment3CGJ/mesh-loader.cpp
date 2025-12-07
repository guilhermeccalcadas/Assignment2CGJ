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
#include "../mgl/mglSceneNode.hpp"
#include <iostream>


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
  bool leftMousePressed = false;
  float panSpeed = 0.01f;
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

  mgl::SceneNode* root = nullptr;
  mgl::SceneNode* tableNode = nullptr;
  std::vector<mgl::SceneNode*> nodes;

  glm::mat4 ProjectionMatrix1;
  glm::mat4 ProjectionMatrix2;

  void createMeshes();
  void createShaderPrograms();
  void createCamera();
  void drawScene();
  void updateCamera();
  glm::mat4 getModel(glm::vec3 pos, float rotX, float rotY, float rotZ, float scal);
  void drawMesh(mgl::Mesh* m, glm::vec3 pos, float rotX, float rotY, float rotZ, float scal);
  void createSceneGraph();
};

///////////////////////////////////////////////////////////////////////// MESHES

void MyApp::createMeshes() {
  std::string mesh_dir = "./assets/models/";
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

void MyApp::createSceneGraph() {
    root = new mgl::SceneNode(nullptr, nullptr);
    tableNode = new mgl::SceneNode(Mesh7, Shaders);
    tableNode->transform = glm::mat4(1.0f);
    root->addChild(tableNode);

    auto n0 = new mgl::SceneNode(MeshesList[0], Shaders);
    n0->transform = getModel(glm::vec3(-0.445f, 0.5f, 0.0f), 90, 0, -90, 1.0f);
    tableNode->addChild(n0);
    nodes.push_back(n0);

    auto n1 = new mgl::SceneNode(MeshesList[1], Shaders);
    n1->transform = getModel(glm::vec3(0.0f, 0.5f, -0.445f), 90, 0, 0, 1.0f);
    tableNode->addChild(n1); nodes.push_back(n1);

    auto n2 = new mgl::SceneNode(MeshesList[2], Shaders);
    n2->transform = getModel(glm::vec3(0.0f, 0.5f, 0.2225f), 90, 0, 0, 1.0f);
    tableNode->addChild(n2); nodes.push_back(n2);

    auto n3 = new mgl::SceneNode(MeshesList[3], Shaders);
    n3->transform = getModel(glm::vec3(0.445f, 0.5f, -0.2225f), 90, 0, 0, 1.0f);
    tableNode->addChild(n3); nodes.push_back(n3);

    auto n4 = new mgl::SceneNode(MeshesList[4], Shaders);
    n4->transform = getModel(glm::vec3(0.2225f, 0.5f, 0.2225f), 90, 0, 0, 1.0f);
    tableNode->addChild(n4); nodes.push_back(n4);

    auto n5 = new mgl::SceneNode(MeshesList[5], Shaders);
    n5->transform = getModel(glm::vec3(-0.11125f, 0.5f, 0.33375f), 90, 0, 0, 1.0f);
    tableNode->addChild(n5); nodes.push_back(n5);

    auto n7 = new mgl::SceneNode(MeshesList[7], Shaders);
    n7->transform = getModel(glm::vec3(0.2225f, 0.5f, 0.0f), 90, 0, 0, 1.0f);
    tableNode->addChild(n7); nodes.push_back(n7);  
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

    // --- NOVO: Inicializar as Matrizes de Projeção ---
    // Usamos o tamanho inicial definido no main (800, 600)
    float aspect = 800.0f / 600.0f;

    // 1. Configurar ProjectionMatrix1 (Ortográfica)
    // Ajustamos a largura (left/right) baseada no aspect ratio
    float orthoSize = 2.0f;
    ProjectionMatrix1 = glm::ortho(-orthoSize * aspect, orthoSize * aspect,
        -orthoSize, orthoSize,
        1.0f, 10.0f);

    // 2. Configurar ProjectionMatrix2 (Perspetiva)
    ProjectionMatrix2 = glm::perspective(glm::radians(30.0f), aspect, 1.0f, 10.0f);


    // --- Lógica de View / Posição (Igual ao que tinhas) ---

    // Definir posição inicial das câmaras
    glm::vec3 eye1 = glm::vec3(5.0f, 5.0f, 5.0f);
    glm::vec3 eye2 = glm::vec3(-5.0f, -5.0f, -5.0f);

    // Calcular radius (distância ao target)
    cam1.radius = glm::length(eye1);
    cam2.radius = glm::length(eye2);

    // Calcular quaternion olhando para o target (0,0,0)
    cam1.rotation = glm::quatLookAt(glm::normalize(-eye1), glm::vec3(0, 1, 0));
    cam2.rotation = glm::quatLookAt(glm::normalize(-eye2), glm::vec3(0, 1, 0));

    // Inicializar viewMatrix das câmeras
    cam1.viewMatrix = glm::lookAt(eye1, target, glm::vec3(0, 1, 0));
    cam2.viewMatrix = glm::lookAt(eye2, target, glm::vec3(0, 1, 0));

    // Definir câmara ativa
    activeCam = &cam1;
    Camera->setViewMatrix(activeCam->viewMatrix);

    // Aplica a matriz de perspetiva que acabámos de calcular
    Camera->setProjectionMatrix(ProjectionMatrix2);
}

/////////////////////////////////////////////////////////////////////////// DRAW

glm::mat4 ModelMatrix;

glm::mat4 MyApp::getModel(glm::vec3 pos, float rotX, float rotY, float rotZ, float scal) {
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
    root->draw(); // delega tudo para o scene graph
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
  createSceneGraph();
}

void MyApp::windowSizeCallback(GLFWwindow* win, int width, int height) {
    // 1. Evitar divisão por zero (se a altura for 0, o programa cracha)
    if (height == 0) height = 1;

    // 2. Atualizar o viewport do OpenGL
    glViewport(0, 0, width, height);

    // 3. Calcular o novo Aspect Ratio
    float aspect = (float)width / (float)height;

    // 4. Atualizar Matriz Perspetiva
    // Mantemos o FOV (30 graus), Near e Far. Só mudamos o aspect.
    ProjectionMatrix2 = glm::perspective(glm::radians(30.0f), aspect, 1.0f, 10.0f);

    // 5. Atualizar Matriz Ortográfica
    // Para não esticar, definimos uma altura fixa (-2 a 2 = tamanho 4)
    // E ajustamos a largura multiplicando pelo aspect ratio.
    float orthoSize = 2.0f;
    ProjectionMatrix1 = glm::ortho(-orthoSize * aspect, orthoSize * aspect, -orthoSize, orthoSize, 1.0f, 10.0f);

    // 6. Enviar a matriz correta para a câmara imediatamente
    if (Camera) {
        Camera->setProjectionMatrix(useOrtho ? ProjectionMatrix1 : ProjectionMatrix2);
    }
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

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            leftMousePressed = true;
            glfwGetCursorPos(win, &lastCameraPosX, &lastCameraPosY);
        }
        else if (action == GLFW_RELEASE) {
            leftMousePressed = false;
        }
    }
}

void MyApp::cursorCallback(GLFWwindow* window, double xpos, double ypos) {
    if (!rightMousePressed && !leftMousePressed) {
        lastCameraPosX = xpos;
        lastCameraPosY = ypos;
        return;
    }

    double distanceX = xpos - lastCameraPosX;
    double distanceY = ypos - lastCameraPosY;

    if (rightMousePressed) {
        float yawAngle = -distanceX * rotationSpeed;
        float pitchAngle = -distanceY * rotationSpeed;

        glm::vec3 localUp = activeCam->rotation * glm::vec3(0.0f, 1.0f, 0.0f);
        glm::quat yaw = glm::angleAxis(yawAngle, glm::normalize(localUp));
        glm::vec3 localRight = activeCam->rotation * glm::vec3(1.0f, 0.0f, 0.0f);
        glm::quat pitch = glm::angleAxis(pitchAngle, glm::normalize(localRight));

        glm::quat orientation = glm::normalize(pitch * yaw * activeCam->rotation);
        activeCam->rotation = orientation;

        updateCamera();
    }

    else if (leftMousePressed) {
        glm::vec3 camRight = activeCam->rotation * glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 camFront = activeCam->rotation * glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 translation = (camRight * (float)distanceX * panSpeed) +
            (camFront * (float)-distanceY * panSpeed);

        tableNode->transform = glm::translate(glm::mat4(1.0f), translation) * tableNode->transform;
    }
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
