/*
 * game/resource/resourceloader.hpp
 *
 * Loads resources *physically* from the filesystem or archives.
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_GAME_RESOURCE_RESOURCELOADER_HPP
#define GRAVITY_GAME_RESOURCE_RESOURCELOADER_HPP

#include <vector>
#include <iosfwd>

#include <boost/optional.hpp>

#ifdef __linux__
#include <json/forwards.h>
#else
#include <jsoncpp/json/forwards.h>
#endif

namespace Gravity {
namespace Game {

class ResourceLoader {
public:
    static void Init();

    static void Deinit();

    static boost::optional<Json::Value>        LoadJson(const std::string& key);

    static boost::optional<std::vector<char> > LoadData(const std::string& key);
};

} // namespace Game
} // namespace Gravity

#endif /* GRAVITY_GAME_RESOURCE_RESOURCELOADER_HPP */
