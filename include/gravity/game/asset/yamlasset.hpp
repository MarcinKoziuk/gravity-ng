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
    YAMLAsset();

    virtual ~YAMLAsset();

    virtual bool Load(const std::string& path);

    virtual std::size_t CalculateSize() const;

    virtual const char* GetAssetType() const;

    const YAML::Node& GetRoot() const;
};

} // namespace Gravity

#endif /* GRAVITY_GAME_ASSET_YAMLASSET_HPP */
