/*
 * game/resource/resource.cpp
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <string>

#include "gravity/logging.hpp"
#include "gravity/game/resource/resource.hpp"

namespace Gravity {
namespace Game {

Resource::Resource(const std::string& key, const std::string& group)
    : key(key)
    , group(group)
{}

void Resource::Unload()
{
    LOG(trace) << "Unloading resource: " << key;
}

void Resource::Load(bool inBackground)
{
    inBackground = inBackground; // otherwise the compiler will warn
    LOG(trace) << "Loading resource: " << key;
}

void Resource::Reload()
{
    if (!IsLoaded()) {
        LOG(warning) << "Trying to reload a resource (" <<
                        GetKey() <<
                        ") that hasn't been loaded yet";
    }

    Unload();
    Load();
}

} // namespace Game
} // namespace Gravity
