#pragma once

#include <vector>
#include <spob/spob.h>

bool pointInPolygon(const std::vector<spob::vec2>& poly, spob::vec2 p);