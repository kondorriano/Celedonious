#ifndef COMMONS_HPP
#define COMMONS_HPP
#include <VBE/VBE.hpp>
#include <VBE-Scenegraph/VBE-Scenegraph.hpp>
#include <VBE-Physics2D/VBE-Physics2D.hpp>
#include <VBE-Profiler/VBE-Profiler.hpp>
#include <iomanip>

namespace Utils {
	template<typename T>
	inline int manhattanDistance(const glm::detail::tvec3<T>& a, const glm::detail::tvec3<T>& b) {
		return std::abs(a.x-b.x)+std::abs(a.y-b.y)+std::abs(a.z-b.z);
	}
	template<typename T>
	std::string toString(T arg) {
	  std::ostringstream temp;
	  temp << arg;
	  return temp.str();
	}
	std::string toString(float f, int width, int precision, bool left);
}

#endif // COMMONS_HPP
