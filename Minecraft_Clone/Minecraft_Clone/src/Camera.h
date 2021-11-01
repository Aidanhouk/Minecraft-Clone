#pragma once

#include "Config.h"
#include "Entity.h"
#include "Maths/Frustum.h"
#include "Maths/glm.h"

class Camera : public Entity {
public:
    Camera(const Config &config) noexcept;

    void update() noexcept;
    void hookEntity(const Entity &entity) noexcept;

    const glm::mat4 &getViewMatrix() const noexcept;
    const glm::mat4 &getProjMatrix() const noexcept;
    const glm::mat4 &getProjectionViewMatrix() const noexcept;

    const ViewFrustum &getFrustum() const noexcept;

private:
    const Entity *m_pEntity;

    ViewFrustum m_frustum;

    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projViewMatrx;

    Config m_config;
};