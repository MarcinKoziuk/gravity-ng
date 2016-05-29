/*
 * game/asset/yamlasset.hpp
 *
 * Copyright (c) 2014, 2015
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_GAME_ASSET_YAMLASSET_HPP
#define GRAVITY_GAME_ASSET_YAMLASSET_HPP

#include <yaml-cpp/node/node.h>

#include <gravity/game/asset/iasset.hpp>

namespace Gravity {

class YAMLAsset : public IAsset {
private:
    YAML::Node root;

public:
    YAMLAsset(const std::string& path);

    virtual ~YAMLAsset();

    virtual bool IsLoaded() const;

    virtual std::size_t CalculateSize() const;

    virtual const char* GetAssetType() const;

    const YAML::Node& GetRoot() const;
};

} // namespace Gravity

#endif /* GRAVITY_GAME_ASSET_YAMLASSET_HPP */
