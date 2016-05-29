/*
 * game/asset/iasset.hpp
 *
 * Copyright (c) 2014, 2015
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_GAME_ASSET_IASSET_HPP
#define GRAVITY_GAME_ASSET_IASSET_HPP

#include <cstddef>

namespace Gravity {

class IAsset {
public:
    virtual ~IAsset() {}

    virtual bool IsLoaded() const = 0;

    virtual std::size_t CalculateSize() const = 0;

    virtual const char* GetAssetType() const = 0;
};

} // namespace Gravity

#endif /* GRAVITY_GAME_ASSET_IASSET_HPP */
