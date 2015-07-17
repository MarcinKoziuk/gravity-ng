/*
 * game/resource/inline/resourcemanager.inl
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

template<class T>
boost::shared_ptr<T> ResourceManager::CreateResource(const std::string& key,
                                                     const std::string& group)
{
    if (resourcePtrs.count(key) && !resourcePtrs[key].expired()) {
        boost::weak_ptr<Resource> basePtr = resourcePtrs[key];
        boost::weak_ptr<T> ptr = *reinterpret_cast<boost::weak_ptr<T>*>(&basePtr);
        return boost::shared_ptr<T>(ptr);
    } else {
        boost::shared_ptr<T> resourceSharedPtr(new T(key, group));
        boost::weak_ptr<Resource> resourcePtr(resourceSharedPtr);
        resourcePtrs[key] = resourcePtr;
        return resourceSharedPtr;
    }
}

template<class T>
boost::shared_ptr<T> ResourceManager::Load(const std::string& key,
                                           const std::string& group,
                                           bool inBackground)
{
    boost::shared_ptr<T> resourcePtr = CreateResource<T>(key, group);
    resourcePtr->Load(inBackground);
    return resourcePtr;
}
