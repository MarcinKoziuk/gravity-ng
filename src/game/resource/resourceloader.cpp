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

#include <gravity/gravity.hpp>
#include <gravity/game/logging.hpp>
#include <gravity/game/resource/resourceloader.hpp>

#include <iterator>
#include <algorithm>

namespace Gravity {

using boost::optional;
using boost::none;
using std::string;
using std::vector;
using std::size_t;

static std::string FixPath(const std::string& path)
{
    std::string s = path;
    std::replace(s.begin(), s.end(), '\\', '/');
    return s;
}

ResourceLoader::StreamWrapper::StreamWrapper(PHYSFS_File *file)
    : file(file)
    , stream(file)
{}

ResourceLoader::StreamWrapper::~StreamWrapper()
{
    PHYSFS_close(file);
}

PhysFS::ifstream& ResourceLoader::StreamWrapper::operator*()
{
    return stream;
}

void ResourceLoader::Init()
{
    PHYSFS_init(GRAVITY_NAME);

	int mounts = 0;
	mounts += !!PHYSFS_mount("../gravity-ng/data", "/", true);
	mounts += !!PHYSFS_mount("./data", "/", true);
	mounts += !!PHYSFS_mount("./Debug/data", "/", true);
	mounts += !!PHYSFS_mount("./Release/data", "/", true);

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

optional<ResourceLoader::StreamWrapperPtr> ResourceLoader::OpenAsStream(const std::string& path)
{
    PHYSFS_File* file = PHYSFS_openRead(FixPath(path).c_str());
    if (file) {
        return std::make_shared<ResourceLoader::StreamWrapper>(file);
    } else {
        LOG(error) << "Resource not found: " << path;
        return boost::none;
    }
}

optional<std::vector<std::uint8_t>> ResourceLoader::OpenAsBytes(const std::string& path)
{
    PHYSFS_File* file = PHYSFS_openRead(FixPath(path).c_str());
    if (file) {
        size_t fileLength = PHYSFS_fileLength(file);
        vector<uint8_t> data(fileLength);

        PHYSFS_read(file, &data[0], fileLength, 1);
        PHYSFS_close(file);

        return data;
    } else {
        LOG(error) << "Resource not found: " << path;
        return none;
    }
}

optional<Json::Value> ResourceLoader::LoadJson(const string& key)
{
    optional<vector<char> > maybeData = LoadData(key);
    if (maybeData) {
        vector<char> data = maybeData.get();
        Json::Value root;
        Json::Reader reader;
        reader.parse(&data.front(), &data.back(), root);
        return root;
    } else {
        return none;
    }
}

optional<vector<char> > ResourceLoader::LoadData(const string& key)
{
    PHYSFS_File* file = PHYSFS_openRead(key.c_str());
    if (file) {
        size_t fileLength = PHYSFS_fileLength(file);
        vector<char> data(fileLength);

        PHYSFS_read(file, &data[0], fileLength, 1);
        PHYSFS_close(file);

        return data;
    } else {
        LOG(error) << "Resource not found: " << key;
        return none;
    }
}

void ResourceLoader::Deinit()
{
    PHYSFS_deinit();
}

} // namespace Gravity
