#ifndef COMMONS_HPP
#define COMMONS_HPP
#include "VBE/includes.hpp"
#include <Box2D/Box2D.h>

namespace Utils {
	inline vec2f B2Dv2ToGLMv2(const b2Vec2& v) {return vec2f(v.x,v.y);}
	inline b2Vec2 GLMv2ToB2Dv2(const vec2f& v) {return b2Vec2(v.x,v.y);}
}

#endif // COMMONS_HPP
