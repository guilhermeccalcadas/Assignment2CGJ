#include "mglSceneNode.hpp"

#include "mglMesh.hpp"
#include "mglShader.hpp"
#include "mglConventions.hpp"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

namespace mgl {

    SceneNode::SceneNode()
        : mesh_(nullptr), shader_(nullptr),
        transform(glm::mat4(1.0f)) {
    }

    SceneNode::SceneNode(Mesh* mesh, ShaderProgram* shader)
        : mesh_(mesh), shader_(shader),
        transform(glm::mat4(1.0f)) {
    }

    void SceneNode::setMesh(Mesh* mesh) { mesh_ = mesh; }
    void SceneNode::setShader(ShaderProgram* shader) { shader_ = shader; }

    void SceneNode::addChild(SceneNode* child) {
        if (child) children_.push_back(child);
    }

    void SceneNode::removeChild(SceneNode* child) {
        auto it = std::find(children_.begin(), children_.end(), child);
        if (it != children_.end()) children_.erase(it);
    }

    void SceneNode::draw() {
        draw(glm::mat4(1.0f), nullptr);
    }

    void SceneNode::draw(const glm::mat4& parentModel, ShaderProgram* inheritedShader) {
        ShaderProgram* active = shader_ ? shader_ : inheritedShader;
        bool boundHere = false;

        if (active && active != inheritedShader) {
            active->bind();
            boundHere = true;
        }

        glm::mat4 global = parentModel * transform;

        if (active && active->isUniform(mgl::MODEL_MATRIX)) {
            GLint loc = active->Uniforms[mgl::MODEL_MATRIX].index;
            if (loc >= 0)
                glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(global));
        }

        if (mesh_)
            mesh_->draw();

        for (auto child : children_) {
            if (child)
                child->draw(global, active);
        }

        if (boundHere)
            active->unbind();
    }

} // namespace mgl
