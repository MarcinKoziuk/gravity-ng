/*
 * cgame/camera.hpp
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_CGAME_CAMERA_HPP
#define GRAVITY_CGAME_CAMERA_HPP

#include <glm/glm.hpp>

namespace Gravity {
namespace CGame {

class Camera {
public:
    Camera(const glm::vec2& size, const glm::vec2& entityPos);

    void Update(const glm::vec2& entityPos);

    const glm::vec2& GetPos() const;

private:
    glm::vec2 size;
    glm::vec2 entityPos;
    glm::vec2 cameraPos;
};

} // namespace CGame
} // namespace Gravity

#endif /* GRAVITY_CGAME_CAMERA_HPP */
