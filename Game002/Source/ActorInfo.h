// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-06-29

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

    bool animationBool = false;

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
    static constexpr size_t s_renderedMeshIndices = 11;

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

const std::unordered_map<size_t, std::string> CursorInfo::s_stateToString =
{
    {0, "STATE_OPEN"},
    {1, "STATE_POINTING"},
    {2, "STATE_MAKING"},
    {3, "STATE_GRABING"}
};

struct PenguinInfo
{
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
    static constexpr size_t s_renderedMeshIndices = 15;

    static constexpr size_t s_grabAttractor_collisionPointIndex = 0;
    static constexpr size_t s_collisionPointCount = 1;

    static constexpr size_t s_aimRay_collisionRayIndex = 0;
    static constexpr size_t s_collisionRayCount = 1;

    static constexpr size_t s_body_collisionCircleIndex = 0;
    static constexpr size_t s_grabZone_collisionCircleIndex = 1;
    static constexpr size_t s_collisionCircleCount = 2;
};

const std::unordered_map<size_t, std::string> PenguinInfo::s_stateToString =
{
    {0, "STATE_STANDING"},
    {1, "STATE_WALKING"},
    {2, "STATE_TREADING_WATER"},
    {3, "STATE_SWIMMING"},
    {4, "STATE_MAKING_SNOWBALL"},
    {5, "STATE_STANDING_SNOWBALL"},
    {6, "STATE_WALKING_SNOWBALL"},
    {7, "STATE_HITSTUN"}
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
    static const size_t s_renderedMeshIndices = 9;

    static const size_t s_snowball_collisionCircleIndex = 0;
    static const size_t s_collisionCircleCount = 1;
};

struct SharkInfo
{
    enum class State
    {
        STATE_SWIMMING = 0,
        STATE_CHASING,
        STATE_HITSTUN
    };

    State state = State::STATE_SWIMMING;

    bool onLand = false;

    float backRotationZ = 0.0f;

    bool mirrorAnimation = false;

    State animationState = State::STATE_SWIMMING;
    float animationStateCountDown_s = 0.0f;

    float positionZ = 0.0f;

    static constexpr size_t s_front_renderedMeshIndex = 0;
    static constexpr size_t s_back_renderedMeshIndex = 1;
    static constexpr size_t s_jaw_renderedMeshIndex = 2;
    static constexpr size_t s_renderedMeshIndices = 3;
};