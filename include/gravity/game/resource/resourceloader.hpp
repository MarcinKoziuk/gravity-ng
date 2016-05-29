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

namespace Gravity {

class ResourceLoader {
public:
    class StreamWrapper {
    private:
        PHYSFS_File* file;
        PhysFS::ifstream stream;

    public:
        StreamWrapper(PHYSFS_File* file);
        ~StreamWrapper();
        StreamWrapper(const StreamWrapper&) = delete;
        StreamWrapper& operator=(const StreamWrapper&) = delete;

        PhysFS::ifstream& operator*();
    };

    typedef std::shared_ptr<StreamWrapper> StreamWrapperPtr;

    static void Init();

    static void Deinit();

    static boost::optional<Json::Value>        LoadJson(const std::string& key);

    static boost::optional<std::vector<char>>  LoadData(const std::string& key);

    static boost::optional<StreamWrapperPtr>   OpenAsStream(const std::string& path);

    static boost::optional<std::vector<std::uint8_t>>  OpenAsBytes(const std::string& path);
};

} // namespace Gravity

#endif /* GRAVITY_GAME_RESOURCE_RESOURCELOADER_HPP */
