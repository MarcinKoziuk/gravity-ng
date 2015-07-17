/*
 * game/resource/resourceloader.cpp
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <cstddef>
#include <cstdlib>
#include <iostream>

#include <boost/optional.hpp>
#include <boost/none.hpp>

#include <physfs.h>
#include <physfs.hpp>

#ifdef __linux__
#include <json/reader.h>
#else
#include <jsoncpp/json/reader.h>
#endif

#include "gravity/gravity.hpp"
#include "gravity/logging.hpp"
#include "gravity/game/resource/resourceloader.hpp"

namespace Gravity {
namespace Game {

using boost::optional;
using boost::none;
using std::string;
using std::vector;
using std::size_t;

void ResourceLoader::Init()
{
    PHYSFS_init(GRAVITY_NAME);

    bool success = PHYSFS_mount("../gravity-ng/data", "/", true);
    if (!success) {
        LOG(fatal) << "Mounting of data directory failed";
    }

    LOG(info) << "Resource loader initialization successful";

    char** paths = PHYSFS_getSearchPath();
    LOG(trace) << "Currently mounted search paths are:";
    for (char** p = &paths[0]; *p != NULL; p++) {
        LOG(trace) << *p;
    }
    PHYSFS_freeList(paths);
}

optional<Json::Value> ResourceLoader::LoadJson(const string& key)
{
    optional<vector<char> > maybeData = LoadData(key);
    if (maybeData) {
        vector<char> data = maybeData.get();
        Json::Value root;
        Json::Reader reader;
        reader.parse(&data[0], &data[data.size()], root);
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

} // namespace Game
} // namespace Gravity
