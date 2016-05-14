/*
 * cgame/camera.cpp
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <glm/glm.hpp>

#include "gravity/cgame/camera.hpp"

namespace Gravity {

Camera::Camera(const glm::vec2& size, const glm::vec2& entityPos)
    : size(size)
    , entityPos(entityPos)
    , cameraPos(entityPos)
{}

void Camera::Update(const glm::vec2& entityPos)
{
    const float maxHorizOffs = size.x / 2;
    const float maxVerticalOffs = size.y / 2;
    const float horizOffs = cameraPos.x - entityPos.x;
    const float verticalOffs = cameraPos.y - entityPos.y;

    if (horizOffs > maxHorizOffs)
        cameraPos.x += (maxHorizOffs - horizOffs);
    if (horizOffs < -maxHorizOffs)
        cameraPos.x -= (maxHorizOffs + horizOffs);
    if (verticalOffs > maxVerticalOffs)
        cameraPos.y += (maxVerticalOffs - verticalOffs);
    if (verticalOffs < -maxVerticalOffs)
        cameraPos.y -= (maxVerticalOffs + verticalOffs);
    this->entityPos = entityPos;
}

const glm::vec2& Camera::GetPos() const
{
    return cameraPos;
}

} // namespace Gravity
