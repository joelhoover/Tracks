#pragma once
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <SFML/System/Vector2.hpp>

BOOST_GEOMETRY_REGISTER_POINT_2D(sf::Vector2f, float, cs::cartesian, x, y)
