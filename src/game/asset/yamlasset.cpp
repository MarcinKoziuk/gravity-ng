/*
 * game/asset/yamlasset.cpp
 *
 * Copyright (c) 2014, 2015
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <algorithm>

#include <yaml-cpp/yaml.h>

#include <boost/optional.hpp>

#include <gravity/game/logging.hpp>
#include <gravity/game/resource/resourceloader.hpp>
#include <gravity/game/asset/yamlasset.hpp>

namespace Gravity {

YAMLAsset::YAMLAsset()
{}

YAMLAsset::~YAMLAsset()
{}

bool YAMLAsset::Load(const std::string &path)
{
    boost::optional<ResourceLoader::StreamWrapperPtr> maybeIStream = ResourceLoader::OpenAsStream(path);
    if (maybeIStream) {
        try {
            std::istream& is = ***maybeIStream;
            root = YAML::Load(is);
            return root;
        } catch (...) {
            LOG(error) << "Parsing yml file " << path << " failed";
            return false;
        }
    } else {
        return false;
    }
}

const YAML::Node& YAMLAsset::GetRoot() const
{
    return root;
}

std::size_t YAMLAsset::CalculateSize() const
{
    return 0L;
}

const char* YAMLAsset::GetAssetType() const
{
    return "YAML";
}

} // namespace Gravity
