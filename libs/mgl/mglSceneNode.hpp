#ifndef MGL_SCENENODE_HPP
#define MGL_SCENENODE_HPP

#include "mglScenegraph.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace mgl {

class Mesh;
class ShaderProgram;

// SceneNode: cada nodo tem um ponteiro para uma Mesh, um ShaderProgram opcional
// e um ModelMatrix local (transform). A hierarquia é aplicada multiplicando
// a matriz do pai pela matriz local ao desenhar.
class SceneNode : public IDrawable {
public:
  SceneNode();
  explicit SceneNode(Mesh* mesh, ShaderProgram* shader = nullptr);
  ~SceneNode() override;

  void setMesh(Mesh* mesh);
  void setShader(ShaderProgram* shader);

  void addChild(SceneNode* child);
  void removeChild(SceneNode* child);

  // Desenha com matriz identidade e sem shader herdado
  void draw() override;

  // Desenha usando a matriz do pai e um shader herdado (se != nullptr)
  void draw(const glm::mat4& parentModel, ShaderProgram* inheritedShader = nullptr);

  // Transform local (ModelMatrix)
  glm::mat4 transform{1.0f};

private:
  Mesh* mesh_{nullptr};
  ShaderProgram* shader_{nullptr};
  std::vector<SceneNode*> children_;
};

} // namespace mgl

#endif /* MGL_SCENENODE_HPP */