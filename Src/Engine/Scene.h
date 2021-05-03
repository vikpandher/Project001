#pragma once

#include <functional>



namespace Project001
{
    class ComponentStores;
    class ModelStores;
    class TextureStores;
    class Window;

    struct Event;

    class Scene
    {
    public:
        Scene() = default;

        Scene(Scene& other) = delete;
        void operator=(const Scene&) = delete;

        virtual const char* Name() = 0;

        virtual void Initialize(
            ComponentStores* componentStoresPtr,
            ModelStores* modelStoresPtr,
            TextureStores* textureStoresPtr,
            Window* windowPtr) = 0;

        virtual void Deinitialize() = 0;

        virtual void OnEvent(Event& event) = 0;

        void SetEventCallback(const std::function<void(Event&)>& callback);

    protected:
        std::function<void(Event&)> EventCallback;

    private:

    };

    inline void Scene::SetEventCallback(const std::function<void(Event&)>& callback)
    {
        EventCallback = callback;
    }
}