#pragma once

#include <functional>



namespace Project001
{
    class ComponentStores;
    struct Event;
    class ResourceStores;
    struct UpdateEvent;
    class Window;

    class Scene
    {
    public:
        Scene(ComponentStores* componentStoresPtr, ResourceStores* resourceStoresPtr, Window* windowPtr);
        ~Scene();

        Scene(Scene& other) = delete;
        void operator=(const Scene&) = delete;

        void SetEventCallback(const std::function<void(Event&)>& callback);

        void OnEvent(Event& event);

    protected:
        void Update(UpdateEvent& updateEvent);

        std::function<void(Event&)> EventCallback;

        ComponentStores* componentStoresPtr_;
        ResourceStores* resourceStoresPtr_;
        Window* windowPtr_;

    private:
        void ComponentContainerTest() const;
        void ComponentStoresTest() const;
    };

    inline void Scene::SetEventCallback(const std::function<void(Event&)>& callback)
    {
        EventCallback = callback;
    }
}