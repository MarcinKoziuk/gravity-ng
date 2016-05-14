/*
 * game/component/icomponent.hpp
 *
 * Defines the abstract class for entity components.
 * Note that all inheriting classes should have an unique, public static
 * "family" integral.
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_GAME_COMPONENT_ICOMPONENT_HPP
#define GRAVITY_GAME_COMPONENT_ICOMPONENT_HPP

#include "gravity/game/entity.hpp"

namespace Gravity {

class IComponent {
public:
    virtual ~IComponent() {}
};

} // namespace Gravity

#endif /* GRAVITY_GAME_COMPONENT_ICOMPONENT_HPP */
