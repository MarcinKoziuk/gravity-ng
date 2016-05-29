/*
 * game/asset/svgasset.hpp
 *
 * Copyright (c) 2014, 2015
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_GAME_ASSET_SVGASSET_HPP
#define GRAVITY_GAME_ASSET_SVGASSET_HPP

#include <nanosvg.h>

#include <gravity/game/asset/iasset.hpp>

namespace Gravity {

class SVGAsset : public IAsset {
private:
    NSVGimage* image;

public:
    SVGAsset(const std::string& path);

    virtual ~SVGAsset();

    virtual bool IsLoaded() const;

    virtual std::size_t CalculateSize() const;

    virtual const char* GetAssetType() const;

    const NSVGimage* GetImage() const;
};

} // namespace Gravity

#endif /* GRAVITY_GAME_ASSET_SVGASSET_HPP */
