/*
 * game/resource/resource.hpp
 *
 * Defines an abstract resource class for holding a loadable game resource.
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_GAME_RESOURCE_RESOURCE_HPP
#define GRAVITY_GAME_RESOURCE_RESOURCE_HPP

#include <string>
#include <cstddef>

#include <boost/shared_ptr.hpp>

namespace Gravity {

class Resource {
protected:
    std::string key;
    std::string group;

public:
    Resource(const std::string& key, const std::string& group);

    virtual ~Resource() {}

    const std::string& GetKey() const
    { return key; }

    const std::string& GetGroup() const
    { return group; }

    virtual std::size_t GetSize() const = 0;

    virtual bool IsLoaded() const = 0;

    virtual void Load(bool inBackground = false);

    virtual void Unload();

    virtual void Reload();
};

typedef boost::shared_ptr<Resource> ResourcePtr;

} // namespace Gravity

#endif /* GRAVITY_GAME_RESOURCE_RESOURCE_HPP */
