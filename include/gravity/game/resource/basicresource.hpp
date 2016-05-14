/*
 * game/resource/basicresource.hpp
 *
 * Boilerplate for managing the isLoaded state in a consistent way.
 *
 * Copyright (c) 2014, 2015
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_GAME_RESOURCE_BASICRESOURCE_HPP
#define GRAVITY_GAME_RESOURCE_BASICRESOURCE_HPP

#include <gravity/game/resource/resource.hpp>

namespace Gravity {

class BasicResource : public Resource {
private:
    bool isLoaded;

protected:
    /*
     * Check if loaded, if not: force loading immediately.
     * Use inside methods returning resource data.
     */
    bool EnsureIsLoaded();

    virtual void LoadImpl() = 0;

    virtual void UnloadImpl() = 0;

public:
    BasicResource(const std::string& name, const std::string& group);

    virtual bool IsLoaded() const
    { return isLoaded; }

    virtual void Load(bool inBackground = false);

    virtual void Unload();

    virtual void Reload();
};

} // namespace Gravity

#endif /* GRAVITY_GAME_RESOURCE_BASICRESOURCE_HPP */
