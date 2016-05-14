/*
 * game/asset/textasset.hpp
 *
 * Copyright (c) 2014, 2015
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_GAME_ASSET_TEXTASSET_HPP
#define GRAVITY_GAME_ASSET_TEXTASSET_HPP

#include <string>

#include <gravity/game/asset/iasset.hpp>

namespace Gravity {

class TextAsset : public IAsset {
private:
    std::string text;

public:
    TextAsset();

    virtual bool Load(const std::string& path);

    const std::string& GetText() const;

    virtual ~TextAsset();
};

} // namespace Gravity

#endif /* GRAVITY_GAME_ASSET_TEXTASSET_HPP */
