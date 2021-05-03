#pragma once

#include "glm/glm.hpp"



// NOTES:
// Using an array to store components types forces you to iterate over them to
// find a combination. Using a bitmask on the entity/system would allow you to
// do a bit-wise comparison between them which is simpler to implement and has
// better performance.
// 
// Sacrifice some memory to store groups of components as tupples instead of
// iterating over entities. A few arrays of pointers/indexes won't make much
// difference and it should make it easier on the CPU.
// 
// Look into using Variadic templates

namespace Project001
{
	typedef glm::uint32 ComponentIndex; // Index into ComponentMemory
	typedef std::vector<glm::uint8> ComponentMemory;
	typedef glm::uint32 ComponentType;
	typedef void* EntityHandle;
}