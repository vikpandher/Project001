// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-07-17


#include "SharedApplicationData.h"

#include "glm/gtc/quaternion.hpp"

#include <string>
#include <unordered_map>



struct CursorInfo
{
    unsigned int snowball_entityId = static_cast<unsigned int>(-1);

    enum class State
    {
        STATE_OPEN = 0,
        STATE_POINTING,
        STATE_MAKING,
        STATE_GRABING
    };

    static const std::unordered_map<size_t, std::string> s_stateToString;

    State state = State::STATE_OPEN;
    float makeSnowballCountDown_s = 0.0f;
    static constexpr float s_makeSnowballTime_s = 0.5f;

    State animationState = State::STATE_OPEN;
    float animationStateCountDown_s = 0.0f;

    bool animaitionFlipper = false;

    bool hoveringOverAlreadyGrabbedEntity = false;

    glm::vec2 cursorWindowPosition;
    glm::vec2 prevCursorWindowPosition;

    static constexpr size_t s_position_renderedMeshIndex = 0;
    static constexpr size_t s_press_renderedMeshIndex = 1;
    static constexpr size_t s_release_renderedMeshIndex = 2;
    static constexpr size_t s_handBase_renderedMeshIndex = 3;
    static constexpr size_t s_handOpen_renderedMeshIndex = 4;
    static constexpr size_t s_handPointer_renderedMeshIndex = 5;
    static constexpr size_t s_handGrab_renderedMeshIndex = 6;
    static constexpr size_t s_grabZone_renderedMeshIndex = 7;
    static constexpr size_t s_aimRay1_renderedMeshIndex = 8;
    static constexpr size_t s_aimRay2_renderedMeshIndex = 9;
    static constexpr size_t s_aimRay3_renderedMeshIndex = 10;
    static constexpr size_t s_renderedMeshCount = 11;

    static constexpr size_t s_position_collisionPointIndex = 0;
    static constexpr size_t s_press_collisionPointIndex = 1;
    static constexpr size_t s_release_collisionPointIndex = 2;
    static constexpr size_t s_onScreen_collisionPointIndex = 3;
    static constexpr size_t s_collisionPointCount = 4;

    static constexpr size_t s_aimRay_collisionRayIndex = 0;
    static constexpr size_t s_collisionRayCount = 1;

    static constexpr size_t s_grabZone_collisionCircleIndex = 0;
    static constexpr size_t s_collisionCircleCount = 1;
};

struct PenguinInfo
{
    size_t playerNumber = 0;

    unsigned int snowball_entityId = static_cast<unsigned int>(-1);

    enum class State
    {
        STATE_STANDING = 0,
        STATE_WALKING,
        STATE_TREADING_WATER,
        STATE_SWIMMING,
        STATE_MAKING_SNOWBALL,
        STATE_STANDING_SNOWBALL,
        STATE_WALKING_SNOWBALL,
        STATE_HITSTUN
    };

    static const std::unordered_map<size_t, std::string> s_stateToString;

    State state = State::STATE_STANDING;
    float makeSnowballCountDown_s = 0.0f;
    static constexpr float s_makeSnowballTime_s = 0.5f;
    float regrabSnowballCoolDown_s = 0.0f;
    static constexpr float s_regrabSnowballTime_s = 0.5f;
    float hitstunCoolDown_s = 0.0f;

    size_t glassesType = 0;

    bool onLand = false;

    State animationState = State::STATE_STANDING;
    float animationStateCountDown_s = 0.0f;

    bool mirrorAnimation = false;

    float positionZ = 0.0f;

    static constexpr float s_initialGrabAttractionRadius = 12.0f + 8.0f; // penguinRadius + snowballRadius
    float grabAttractionRadius = s_initialGrabAttractionRadius;

    static constexpr size_t s_body_renderedMeshIndex = 0;
    static constexpr size_t s_flipper_right_renderedMeshIndex = 1;
    static constexpr size_t s_flipper_left_renderedMeshIndex = 2;
    static constexpr size_t s_foot_right_renderedMeshIndex = 3;
    static constexpr size_t s_foot_left_renderedMeshIndex = 4;
    static constexpr size_t s_head_renderedMeshIndex = 5;
    static constexpr size_t s_eyes_renderedMeshIndex = 6;
    static constexpr size_t s_beak_renderedMeshIndex = 7;
    static constexpr size_t s_glasses_renderedMeshIndex = 8;
    static constexpr size_t s_shadow_renderedMeshIndex = 9;
    static constexpr size_t s_grabZone_renderedMeshIndex = 10;
    static constexpr size_t s_aimRay1_renderedMeshIndex = 11;
    static constexpr size_t s_aimRay2_renderedMeshIndex = 12;
    static constexpr size_t s_orientationArrow_renderedMeshIndex = 13;
    static constexpr size_t s_grabAttractorCollision_renderedMeshIndex = 14;
    static constexpr size_t s_renderedMeshCount = 15;

    static constexpr size_t s_grabAttractor_collisionPointIndex = 0;
    static constexpr size_t s_collisionPointCount = 1;

    static constexpr size_t s_aimRay_collisionRayIndex = 0;
    static constexpr size_t s_collisionRayCount = 1;

    static constexpr size_t s_body_collisionCircleIndex = 0;
    static constexpr size_t s_grabZone_collisionCircleIndex = 1;
    static constexpr size_t s_collisionCircleCount = 2;
};

struct SnowballInfo
{
    unsigned int owner_entityId = static_cast<unsigned int>(-1);

    enum class State
    {
        STATE_REGULAR,
        STATE_EXPLODING
    };

    State state = State::STATE_REGULAR;
    static constexpr float s_explosionTime_s = 0.75f;

    State animationState = State::STATE_REGULAR;
    float animationStateCountDown_s = 0.0f;

    float animationRandomRotation = 0.0f;

    bool onLand = false;

    static constexpr float s_initialSnowballRadius = 8.0f;
    float radius = s_initialSnowballRadius;
    static constexpr float s_maxRadius = 48.0f;
    static constexpr float s_renderedMeshRadiusScaler = 1.2f;

    float positionZ = 0.0f;

    static const size_t s_snowball_renderedMeshIndex = 0;
    static const size_t s_snowball_break_01_renderedMeshIndex = 1;
    static const size_t s_snowball_break_02_renderedMeshIndex = 2;
    static const size_t s_snowball_break_03_renderedMeshIndex = 3;
    static const size_t s_snowball_break_04_renderedMeshIndex = 4;
    static const size_t s_snowball_break_05_renderedMeshIndex = 5;
    static const size_t s_snowball_break_06_renderedMeshIndex = 6;
    static const size_t s_shadow_renderedMeshIndex = 7;
    static const size_t s_orientationArrow_renderedMeshIndex = 8;
    static const size_t s_renderedMeshCount = 9;

    static const size_t s_snowball_collisionCircleIndex = 0;
    static const size_t s_collisionCircleCount = 1;
};

struct SharkInfo
{
    static constexpr glm::vec2 s_spawnPoint = glm::vec2(0.0f, 800.0f);

    enum class State
    {
        STATE_SWIMMING = 0,
        STATE_CHASING,
        STATE_HITSTUN
    };

    State state = State::STATE_SWIMMING;

    bool onLand = false;

    State animationState = State::STATE_SWIMMING;
    float animationStateCountDown_s = 0.0f;
    float hitstunCoolDown_s = 0.0f;

    bool animaitionFlipper = false;

    float positionZ = 0.0f;

    float frontRotationX = 0.0f;
    float backRotationZ = 0.0f;
    float jawRotationZ = 0.0f;

    float minAttackIntersectionScalars[4] = {
        std::numeric_limits<float>::infinity(),
        std::numeric_limits<float>::infinity(),
        std::numeric_limits<float>::infinity(),
        std::numeric_limits<float>::infinity()
    };

    bool minAttackIntersectionWithPenguins[4] = {
        false, false, false, false
    };

    static constexpr size_t s_front_renderedMeshIndex = 0;
    static constexpr size_t s_back_renderedMeshIndex = 1;
    static constexpr size_t s_jaw_renderedMeshIndex = 2;
    static constexpr size_t s_frontCollision_renderedMeshIndex = 3;
    static constexpr size_t s_backCollision_renderedMeshIndex = 4;
    static constexpr size_t s_jawCollision_renderedMeshIndex = 5;
    static constexpr size_t s_attackRay1_renderedMeshIndex = 6;
    static constexpr size_t s_attackRay2_renderedMeshIndex = 7;
    static constexpr size_t s_attackRay3_renderedMeshIndex = 8;
    static constexpr size_t s_attackRay4_renderedMeshIndex = 9;
    static constexpr size_t s_renderedMeshCount = 10;

    static constexpr size_t s_attackRay1_collisionRayIndex = 0;
    static constexpr size_t s_attackRay2_collisionRayIndex = 1;
    static constexpr size_t s_attackRay3_collisionRayIndex = 2;
    static constexpr size_t s_attackRay4_collisionRayIndex = 3;
    static constexpr size_t s_collisionRayCount = 4;

    static const size_t s_jaw_collisionCircleIndex = 0;
    static const size_t s_collisionCircleCount = 1;

    static const size_t s_body_collisionRectangleIndex = 0;
    static const size_t s_collisionRectangleCount = 1;

    static const size_t s_body_collisionTriangleIndex = 0;
    static const size_t s_collisionTriangleCount = 1;
};

struct SharkPathInfo
{
    unsigned int nextPathPoint = 0;

    static constexpr size_t s_renderedMeshCount = 8;

    static constexpr size_t s_collisionPointCount = 8;
};

struct StageInfo
{
    float groundSize = SharedApplicationData::s_ground_size;

    float groundSkrinkRate_s = 0.0f;

    static constexpr size_t s_ground_renderedMeshIndex = 0;
    static constexpr size_t s_water_renderedMeshIndex = 1;
    static constexpr size_t s_deadZone_renderedMeshIndex = 2;
    static constexpr size_t s_collisionQuadTree_renderedMeshIndex = 4;
    static constexpr size_t s_grid_renderedMeshIndex = 5;
    static constexpr size_t s_gridLabels_renderedMeshIndex = 6;
    static constexpr size_t s_renderedMeshCount = 7;

    static constexpr size_t s_ground_collisionConvexPolygonIndex = 0;
    static constexpr size_t s_collisionConvexPolygonCount = 1;
};

struct ImpactEffectInfo
{
    static constexpr size_t s_frameCount = 4;
    size_t currentFrame = 0;

    static constexpr float s_frameDuration_s = 0.0625f;
    float animationTime_s = 0.0f;

    bool deadFlag = false;
};

struct ImpactEffectCreationInfo
{
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(64.0f, 64.0f, 64.0f);
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
};