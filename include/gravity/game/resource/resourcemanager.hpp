/*
 * game/resource/resourcemanager.hpp
 *
 * Generic resource manager supporting deferred loading.
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_GAME_RESOURCE_RESOURCEMANAGER_HPP
#define GRAVITY_GAME_RESOURCE_RESOURCEMANAGER_HPP

#include <string>
#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "gravity/game/resource/resource.hpp"

namespace Gravity {

class ResourceManager {
private:
    std::map<std::string, boost::weak_ptr<Resource> > resourcePtrs;

public:
    template<class T>
    boost::shared_ptr<T> CreateResource(const std::string& key,
                                        const std::string& group = "default");

    template<class T>
    boost::shared_ptr<T> Load(const std::string& key,
                              const std::string& group = "default",
                              bool inBackground = false);

    void DestroyResource(ResourcePtr resource);

    void UnloadAll();

    void ReloadAll();

    void UnloadGroup();

    void ReloadGroup();
};

#include "inline/resourcemanager.inl"

} // namespace Gravity

#endif /* GRAVITY_GAME_RESOURCE_RESOURCEMANAGER_HPP */
