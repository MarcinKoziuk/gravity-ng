/*
 * cgame/resource/shadersource.cpp
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <cstddef>
#include <cstdlib>
#include <vector>
#include <string>

#include "gravity/game/logging.hpp"
#include "gravity/game/resource/resourceloader.hpp"
#include "gravity/cgame/resource/shadersource.hpp"

namespace Gravity {

ShaderSource::ShaderSource(const std::string& key, const std::string& group)
    : Resource(key, group)
    , isLoaded(false)
{}

ShaderSource::~ShaderSource() {
    Unload();
}

std::size_t ShaderSource::GetSize() const
{
    return source.size();
}

void ShaderSource::Load(bool inBackground)
{
    Resource::Load(inBackground);

    if (!isLoaded) {
        boost::optional<std::vector<char> > maybeShaderSource = ResourceLoader::LoadData(key);
        if (!maybeShaderSource) {
            LOG(fatal) << "Shader source does not exist: " << key;
            std::exit(1);
        }
        // nul terminate so it may be converted to const char *
        source = *maybeShaderSource;
        source.push_back(0);
        isLoaded = true;
    }
}

void ShaderSource::Unload()
{
    Resource::Unload();

    if (isLoaded) {
        source.clear();
        isLoaded = false;
    }
}

const std::vector<char>& ShaderSource::GetSource()
{
    if (!isLoaded) {
        Load(false);
        LOG(trace) << "Had to implicitly load " << key;
    }
    return source;
}

} // namespace Gravity
