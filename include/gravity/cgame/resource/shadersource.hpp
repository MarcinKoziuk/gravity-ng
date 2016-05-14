/*
 * cgame/resource/shadersource.hpp
 *
 * Source of a vertex/fragment shader.
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_CGAME_RESOURCE_SHADERSOURCE_HPP
#define GRAVITY_CGAME_RESOURCE_SHADERSOURCE_HPP

#include <cstddef>
#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>

#include "gravity/game/resource/resource.hpp"

namespace Gravity {

class ShaderSource : public Resource {
private:
    bool isLoaded;
    std::vector<char> source;

public:
    ShaderSource(const std::string& key, const std::string& group);

    virtual ~ShaderSource();

    virtual std::size_t GetSize() const;

    virtual bool IsLoaded() const
    { return isLoaded; }

    virtual void Load(bool inBackground = false);

    virtual void Unload();

    virtual const std::vector<char>& GetSource();
};

typedef boost::shared_ptr<ShaderSource> ShaderSourcePtr;

} // namespace Gravity

#endif /* GRAVITY_CGAME_RESOURCE_SHADERSOURCE_HPP */
