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
#include <memory>
#include <cstdint>

#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>

#include <physfs.hpp>

#include <json/forwards.h>
#include <yaml-cpp/node/node.h>

extern "C" {
	struct NSVGimage;
	void nsvgDelete(NSVGimage*); 
}

namespace Gravity {

struct NSVGimage_deleter {
	void operator()(NSVGimage* p) { nsvgDelete(p); }
};

class ResourceLoader {
public:
    static void Init();

    static void Deinit();

   // static boost::optional<Json::Value>        LoadJson(const std::string& key);

   // static boost::optional<std::vector<char>>  LoadData(const std::string& key);

	static std::unique_ptr<PhysFS::ifstream>              OpenAsStream(const std::string& path);

    static boost::optional<std::vector<std::uint8_t>>     OpenAsBytes(const std::string& path);

	static boost::optional<YAML::Node>                    OpenAsYaml(const std::string& path);

	static std::unique_ptr<NSVGimage, NSVGimage_deleter>  OpenAsSvg(const std::string& path);

	static std::unique_ptr<NSVGimage, NSVGimage_deleter>  OpenAsSvg(const std::string& path, const std::string& units, float dpi);
};

} // namespace Gravity

#endif /* GRAVITY_GAME_RESOURCE_RESOURCELOADER_HPP */
