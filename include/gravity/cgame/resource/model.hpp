/*
 * cgame/resource/model.hpp
 *
 * The 2D model to render by the client
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_CGAME_RESOURCE_MODEL_HPP
#define GRAVITY_CGAME_RESOURCE_MODEL_HPP

#include <cstddef>
#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>

#include "gravity/game/resource/resource.hpp"

namespace Gravity {
namespace CGame {

class Model : public Game::Resource {
public:
    enum Type { LINES, CIRCLE };
    struct Object {
        Type type;
        std::size_t len;
        float data[1];
    };
private:
    bool isLoaded;
    std::vector<Object*> objects;

    void LoadObjects(const Json::Value& root);
public:
    Model(const std::string& key, const std::string& group);

    virtual ~Model();

    virtual std::size_t GetSize() const;

    virtual bool IsLoaded() const
    { return isLoaded; }

    virtual void Load(bool inBackground = false);

    virtual void Unload();

    virtual const std::vector<Object*>& GetObjects();
};

typedef boost::shared_ptr<Model> ModelPtr;

} // namespace CGame
} // namespace Gravity

#endif /* GRAVITY_CGAME_RESOURCE_MODEL_HPP */
