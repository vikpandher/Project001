#include "Scene.h"

#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/ResourceStores.h"
#include "Engine/Window.h"



namespace Project001
{
    const int COMPONENT_TYPE_ID_DEATH_FLAG = 100;
    const int COMPONENT_TYPE_ID_TEST = 101;

    struct DeathFlagComponent : Component<COMPONENT_TYPE_ID_DEATH_FLAG>
    {
        DeathFlagComponent()
            : dead(false)
        {}

        bool dead;
    };

    struct TestComponent : Component<COMPONENT_TYPE_ID_TEST>
    {
        TestComponent(int a, int b, int c)
            : a(a)
            , b(b)
            , c(c)
        {}

        inline int GetSum()
        {
            return a + b + c;
        }

        int a;
        int b;
        int c;
    };

    Scene::Scene(ComponentStores* componentStoresPtr, ResourceStores* resourceStoresPtr, Window* windowPtr)
        : componentStoresPtr_(componentStoresPtr)
        , resourceStoresPtr_(resourceStoresPtr)
        , windowPtr_(windowPtr)
    {
        ComponentContainerTest();
        ComponentStoresTest();
    }

    Scene::~Scene()
    {
    }

    void Scene::OnEvent(Event& event)
    {
        DispatchEvent<UpdateEvent>(event, std::bind(&Scene::Update, this, std::placeholders::_1));
    }

    void Scene::Update(UpdateEvent& updateEvent)
    {
        double timestep = updateEvent.timestep_s;

        // Update script components

        // Delete all entities with marked for deletion component

        // Render all renderable components

        // Play all audio components???

        updateEvent.handled = true;
    }

    void Scene::ComponentContainerTest() const
    {
        bool testBool = false;

        ComponentContainer testComponentContainer;

        testBool = testComponentContainer.CreateComponent<TestComponent>(0, 10, 10, 10);
        testBool = testComponentContainer.CreateComponent<TestComponent>(0, 10, 10, 10);
        testBool = testComponentContainer.CreateComponent<TestComponent>(1, 20, 20, 20);
        testBool = testComponentContainer.CreateComponent<TestComponent>(2, 30, 30, 30);

        TestComponent* testComponent0 = nullptr;
        TestComponent* testComponent1 = nullptr;
        TestComponent* testComponent2 = nullptr;

        testBool = testComponentContainer.GetComponent<TestComponent>(0, testComponent0);
        testBool = testComponentContainer.GetComponent<TestComponent>(1, testComponent1);
        testBool = testComponentContainer.GetComponent<TestComponent>(2, testComponent2);

        TestComponent* allTestComponents = nullptr;
        size_t count = 0;

        testBool = testComponentContainer.GetAllComponents<TestComponent>(allTestComponents, count);

        testBool = testComponentContainer.DeleteComponent(0);
        testBool = testComponentContainer.DeleteComponent(0);
        testBool = testComponentContainer.GetComponent<TestComponent>(0, testComponent0);
    }

    void Scene::ComponentStoresTest() const
    {
        bool testBool = false;

        ComponentStores testComponentStores;

        testBool = testComponentStores.CreateComponent<TestComponent>(0, 0, 0, 0);

        unsigned int entity0;
        testBool = testComponentStores.CreateEntity(entity0);
        testBool = testComponentStores.CreateComponent<TestComponent>(entity0, 10, 10, 10);

        unsigned int entity1;
        testBool = testComponentStores.CreateEntity(entity1);
        testBool = testComponentStores.CreateComponent<TestComponent>(entity1, 11, 11, 11);

        unsigned int entity2;
        testBool = testComponentStores.CreateEntity(entity2);
        testBool = testComponentStores.CreateComponent<TestComponent>(entity2, 12, 12, 12);

        testBool = testComponentStores.DeleteEntity(9);
        testBool = testComponentStores.DeleteEntity(entity0);
        testBool = testComponentStores.DeleteEntity(entity0);

        unsigned int entity3;
        testBool = testComponentStores.CreateEntity(entity3);
        testBool = testComponentStores.CreateComponent<TestComponent>(entity3, 13, 13, 13);

        TestComponent* testComponent0 = nullptr;
        testBool = testComponentStores.GetComponent<TestComponent>(entity1, testComponent0);
        int sum = testComponent0->GetSum();
    }
}