#include "Engine/SceneComponents.h"



namespace Project001
{
    // TestComponent: ----------------------------------------------------------

    TestComponent::TestComponent(int a, int b, int c)
        : a(a)
        , b(b)
        , c(c)
    {}

    // SceneDataComponent: -----------------------------------------------------

    SceneDataComponent::SceneDataComponent()
        : previousCursorDownPosition(0.0f, 0.0f)
    {}

    // DeathFlagComponent: -----------------------------------------------------

    DeathFlagComponent::DeathFlagComponent()
        : dead(false)
    {}

    // CameraComponent: --------------------------------------------------------

    CameraComponent::CameraComponent()
        : turnedOn(true)
        , position(0.0f, 0.0f, 2.0f)
        , orientation(1.0f, 0.0f, 0.0f, 0.0f)
        , orthographicProjection(false)
        , fieldOfVisionDegrees(45.0f)
        , aspectRatio(1.0f)
        , leftCutOff(-5.0f)
        , rightCutOff(5.0f)
        , bottomCutOff(-5.0f)
        , topCutOff(5.0f)
        , nearCutOff(0.1f)
        , farCutOff(10.0f)
    {}

    glm::vec3 CameraComponent::s_worldForward(0.0f, 0.0f, -1.0f); // used for Roll "right hand rule"
    glm::vec3 CameraComponent::s_worldUp(0.0f, 1.0f, 0.0f); // used for Yaw "right hand rule"
    glm::vec3 CameraComponent::s_worldRight(1.0f, 0.0f, 0.0f); // used for Pitch "right hand rule"

    // LightComponent: ---------------------------------------------------------

    LightComponent::LightComponent()
        : turnedOn(true)
        , lightType(LightType::LIGHT_TYPE_POINT_LIGHT)
        , position(0.0f, 0.0f, 0.0f)
        , direction(0.0f, 0.0f, 0.0f)
        , cutoff(0.99f)
        , outerCutoff(0.97f)
        , constant(1.0f)
        , linear(0.0f)
        , quadratic(0.0f)
        , ambient(0.0f, 0.0f, 0.0f)
        , diffuse(1.0f, 1.0f, 1.0f)
        , specular(0.0f, 0.0f, 0.0f)
    {}

    // RenderedModelComponent: -------------------------------------------------

    RenderedModelComponent::RenderedModelComponent()
        : modelIndex((unsigned int)-1)
        , textureIndex((unsigned int)-1)
        , specularIndex((unsigned int)-1)
        , shininess(32.0f)
        , color(1.0f, 1.0f, 1.0f, 1.0f)
        , scale(1.0f, 1.0f, 1.0f)
        , position(0.0f, 0.0f, 0.0f)
        , orientation(1.0f, 0.0f, 0.0f, 0.0f)
    {}

    // PhysicsBodyComponent: ---------------------------------------------------

    PhysicsBodyComponent::PhysicsBodyComponent()
        : modelIndex((unsigned int)-1)
        , position(0.0f, 0.0f, 0.0f)
        , orientation(0.0f, 0.0f, 0.0f, 1.0f)
        , velocity(0.0f, 0.0f, 0.0f)
    {}

}