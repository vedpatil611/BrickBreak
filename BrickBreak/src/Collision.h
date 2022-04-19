#pragma once

#include <glm/glm.hpp>
#include <tuple>

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

Direction VectorDirection(glm::vec2 target);

typedef std::tuple<bool, Direction, glm::vec2> Collision;
