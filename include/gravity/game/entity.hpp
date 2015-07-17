/*
 * game/entity.hpp
 *
 * Defines the entity object. All game objects are instances of this class.
 * It's not supposed to be inherited from.
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_GAME_ENTITY_HPP
#define GRAVITY_GAME_ENTITY_HPP

#include <cstddef>
#include <map>

#include "gravity/game/component/icomponent.hpp"

namespace Gravity {
namespace Game {

class World;

class Entity {
private:
    std::size_t id;
    World& world;
    std::map<std::size_t, IComponent*> components;

public:
    static std::size_t idSequence;

    Entity(World& world);

    std::size_t GetId() const
    { return id; }

    World& GetWorld() const
    { return world; }

    template<class T>
    bool HasComponent() const
    { return components.count(T::family); }

    template<class T>
    const T& GetComponent() const
    { return *reinterpret_cast<T*>(components.find(T::family)->second); }

    template<class T>
    void AddComponent(IComponent* component)
    { components[T::family] = component; }
};

} // namespace Game
} // namespace Gravity

#endif /* GRAVITY_GAME_ENTITY_HPP */
