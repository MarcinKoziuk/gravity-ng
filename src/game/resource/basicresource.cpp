/*
 * game/resource/basicresource.cpp
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <string>

#include "gravity/game/logging.hpp"
#include "gravity/game/resource/basicresource.hpp"

namespace Gravity {

BasicResource::BasicResource(const std::string& key, const std::string& group)
    : Resource(key, group)
{}

void BasicResource::Unload()
{
    LOG(trace) << "Unloading resource: " << key;
}

void BasicResource::Load(bool inBackground)
{
    inBackground = inBackground; // otherwise the compiler will warn
    LOG(trace) << "Loading resource: " << key;
}

void BasicResource::Reload()
{
    if (!IsLoaded()) {
        LOG(warning) << "Trying to reload a resource (" <<
                        GetKey() <<
                        ") that hasn't been loaded yet";
    }

    Unload();
    Load();
}

} // namespace Gravity
