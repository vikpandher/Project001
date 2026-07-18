// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-07-17

#include "ActorInfo.h"



const std::unordered_map<size_t, std::string> CursorInfo::s_stateToString =
{
    {0, "STATE_OPEN"},
    {1, "STATE_POINTING"},
    {2, "STATE_MAKING"},
    {3, "STATE_GRABING"}
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