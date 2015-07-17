/*
 * cgame/resource/model.cpp
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <cstddef>
#include <cstdlib>
#include <vector>
#include <string>

#include <boost/optional.hpp>

#ifdef __linux__
#include <json/json.h>
#else
#include <jsoncpp/json/json.h>
#endif

#include "gravity/logging.hpp"
#include "gravity/game/resource/resourceloader.hpp"
#include "gravity/cgame/resource/model.hpp"

namespace Gravity {
namespace CGame {

using namespace Game;
using boost::optional;

Model::Model(const std::string& key, const std::string& group)
    : Resource(key, group)
    , isLoaded(false)
{}

Model::~Model() {
    Unload();
}

std::size_t Model::GetSize() const
{
    if (objects.size()) {
        std::size_t counter;
        for (std::size_t i = 0; i < objects.size(); i++) {
            counter += (sizeof *objects[i] + objects[i]->len - sizeof *objects[i]->data);
        }

        return counter;
    } else {
        return 0L;
    }
}

void Model::LoadObjects(const Json::Value& root)
{
    Json::Value objectsValue = root["objects"];

    for (Json::ArrayIndex i = 0; i < objectsValue.size(); i++) {
        Json::Value objectValue = objectsValue[i];

        std::string typeString = objectValue.get("type", "lines").asString();
        Model::Type type;
        if (typeString == "lines") {
            type = Model::LINES;
        } else if (typeString == "circle") {
            type = Model::CIRCLE;
        } else {
            LOG(error) << "Unrecognized object type in model " << key << ": " << typeString;
            type = Model::LINES;
        }

        Json::Value data = objectValue["data"];

        std::size_t len = 0L;
        if (type == Model::LINES) {
            // multiple vertices (two floats each)
            len = std::size_t(data.size() * 2 * sizeof (float));
        } else if (type == Model::CIRCLE){
            // x, y, radius
            len = 3L * sizeof (float);
        }

        Model::Object* object;
        std::size_t allocSize = sizeof *object + len - sizeof *object->data;
        object = (Model::Object*) std::malloc(allocSize);
        object->type = type;
        object->len = len;

        if (type == Model::LINES) {
            for (Json::ArrayIndex j = 0; j < data.size(); j++) {
                object->data[j*2] = data[j][0].asFloat();
                object->data[j*2+1] = data[j][1].asFloat();
            }
        } else if (type == Model::CIRCLE){
            for (Json::ArrayIndex j = 0; j < 3; j++) {
                object->data[j] = data[j].asFloat();
            }
        }

        objects.push_back(object);
    }
}

void Model::Load(bool inBackground)
{
    Resource::Load(inBackground);

    if (!isLoaded) {
        optional<Json::Value> maybeRoot = ResourceLoader::LoadJson(key);
        if (!maybeRoot) {
            LOG(fatal) << "Model does not exist: " << key;
            std::exit(1);
        }

        LoadObjects(*maybeRoot);

        isLoaded = true;
    }
}

void Model::Unload()
{
    Resource::Unload();

    if (isLoaded) {
        for (std::size_t i = 0; i < objects.size(); i++) {
            std::free(objects[i]);
        }
        isLoaded = false;
    }
}

const std::vector<Model::Object*>& Model::GetObjects()
{
    if (!isLoaded) {
        Load(false);
        LOG(trace) << "Had to implicitly load " << key;
    }
    return objects;
}

} // namespace CGame
} // namespace Gravity
