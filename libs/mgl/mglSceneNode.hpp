#ifndef MGL_SCENENODE_HPP
#define MGL_SCENENODE_HPP

#include <vector>
#include <glm/mat4x4.hpp>

#include "mglMesh.hpp"
#include "mglShader.hpp"
#include "mglScenegraph.hpp"  // <--- aqui está o IDrawable

namespace mgl {

    class SceneNode : public IDrawable {
    public:
        SceneNode();
        SceneNode(Mesh* mesh, ShaderProgram* shader);

        void setMesh(Mesh* mesh);
        void setShader(ShaderProgram* shader);

        void addChild(SceneNode* child);
        void removeChild(SceneNode* child);

        // Implementação da interface IDrawable
        void draw(void) override;

    public:
        glm::mat4 transform;

    private:
        void draw(const glm::mat4& parentModel, ShaderProgram* inheritedShader);

        Mesh* mesh_;
        ShaderProgram* shader_;

        std::vector<SceneNode*> children_;
    };

} // namespace mgl

#endif /* MGL_SCENENODE_HPP */
