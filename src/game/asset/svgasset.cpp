/*
 * game/asset/svgasset.cpp
 *
 * Copyright (c) 2014, 2015
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <boost/optional.hpp>

#include <gravity/game/resource/resourceloader.hpp>
#include <gravity/game/asset/svgasset.hpp>

namespace Gravity {

SVGAsset::SVGAsset()
    : image(nullptr)
{}

SVGAsset::~SVGAsset()
{
    if (image) {
        nsvgDelete(image);
    }
}

bool SVGAsset::Load(const std::string &path)
{
    boost::optional<std::vector<char>> maybeBytes = ResourceLoader::OpenAsBytes(path);
    if (maybeBytes) {
        std::vector<char> bytes = *maybeBytes;
        bytes.push_back(0);
        image = nsvgParse(&bytes[0], "px", 96);

        return true;
    } else {
        return false;
    }
}

const NSVGimage* SVGAsset::GetImage() const
{
    return image;
}

std::size_t SVGAsset::CalculateSize() const
{
    std::size_t acc = 0;
    if (image) {
        acc += sizeof *image;
        for (NSVGshape* shape = image->shapes; shape != nullptr; shape = shape->next) {
            acc += sizeof *shape;
            for (NSVGpath* path = shape->paths; path != nullptr; path = path->next) {
                acc += sizeof *path;
            }
        }
    }

    return acc;
}

const char* SVGAsset::GetAssetType() const
{
    return "SVG";
}

} // namespace Gravity
