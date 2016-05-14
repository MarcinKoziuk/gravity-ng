/*
 * lib/casteljau.hpp
 *
 * Generic resource manager supporting deferred loading.
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_LIB_CASTELJAU_HPP
#define GRAVITY_LIB_CASTELJAU_HPP

#include <vector>

#include <glm/vec2.hpp>

namespace Gravity {

std::vector<glm::vec2> Casteljau(const std::vector<glm::vec2>& curve);

} // namespace Gravity

#endif /* GRAVITY_LIB_CASTELJAU_HPP */
