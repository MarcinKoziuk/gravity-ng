/*
 * game/resource/resourceloader.cpp
 *
 * Copyright (c) 2014, 2015
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <cstddef>
#include <cstdlib>
#include <vector>
#include <istream>
#include <fstream>
#include <iostream>
#include <memory>

#include <boost/optional.hpp>
#include <boost/none.hpp>

#include <physfs.h>
#include <physfs.hpp>

#include <json/reader.h>
#include <yaml-cpp/yaml.h>
#include <nanosvg.h>

#include <gravity/gravity.hpp>
#include <gravity/game/logging.hpp>
#include <gravity/game/resource/resourceloader.hpp>

#include <iterator>
#include <algorithm>

namespace Gravity {

static std::string FixPath(const std::string& path)
{
    std::string s = path;
    std::replace(s.begin(), s.end(), '\\', '/');
    return s;
}

void ResourceLoader::Init()
{
    PHYSFS_init(GRAVITY_NAME);

	int mounts = 0;
	mounts += bool(PHYSFS_mount("../gravity-ng/data", "/", true));
	mounts += bool(PHYSFS_mount("./data", "/", true));
	mounts += bool(PHYSFS_mount("./Debug/data", "/", true));
	mounts += bool(PHYSFS_mount("./Release/data", "/", true));

    if (!mounts) {
        LOG(fatal) << "Mounting of data directory failed";
        throw std::runtime_error("Failed to mount PhysFS directory");
	} else {
		LOG(info) << "Number of PhysFS mounts: " << mounts;
	}

    LOG(trace) << "Currently mounted search paths are:";
    const std::vector<std::string> paths = PhysFS::getSearchPath();
    for (const std::string& path : paths) {
        LOG(trace) << path;
    }
}

std::unique_ptr<PhysFS::ifstream> ResourceLoader::OpenAsStream(const std::string& path)
{
	PHYSFS_File* file = PHYSFS_openRead(FixPath(path).c_str());
	if (file) {
		return std::unique_ptr<PhysFS::ifstream>(new PhysFS::ifstream(file));
	} else {
		LOG(error) << "Resource not found: " << path;
		return nullptr;
	}
}

boost::optional<std::vector<std::uint8_t>> ResourceLoader::OpenAsBytes(const std::string& path)
{
    PHYSFS_File* file = PHYSFS_openRead(FixPath(path).c_str());
    if (file) {
        size_t fileLength = PHYSFS_fileLength(file);
        std::vector<uint8_t> data(fileLength);

        PHYSFS_read(file, &data[0], fileLength, 1);
        PHYSFS_close(file);

        return data;
    } else {
        LOG(error) << "Resource not found: " << path;
        return boost::none;
    }
}

boost::optional<YAML::Node> ResourceLoader::OpenAsYaml(const std::string& path)
{
	std::unique_ptr<PhysFS::ifstream> istream = ResourceLoader::OpenAsStream(path);

	if (istream) {
		try {
			std::istream& is = *istream;
			return YAML::Load(*istream);
		}
		catch (...) {
			LOG(error) << "Parsing yml file " << path << " failed";
		}
	}

	return boost::none;
}

std::unique_ptr<NSVGimage, NSVGimage_deleter> ResourceLoader::OpenAsSvg(const std::string& path)
{
	return OpenAsSvg(path, "px", 96);
}

std::unique_ptr<NSVGimage, NSVGimage_deleter> ResourceLoader::OpenAsSvg(const std::string& path, const std::string& units, float dpi)
{
	boost::optional<std::vector<uint8_t>> maybeBytes = ResourceLoader::OpenAsBytes(path);
	if (maybeBytes) {
		std::vector<uint8_t> bytes = *maybeBytes;
		bytes.push_back(0);
		return std::unique_ptr<NSVGimage, NSVGimage_deleter>(nsvgParse((char *)(&bytes[0]), units.c_str(), dpi));
	} else {
		return nullptr;
	}
}

void ResourceLoader::Deinit()
{
    PHYSFS_deinit();
}

} // namespace Gravity
