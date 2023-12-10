#pragma once

// Defines
#ifdef GEOFRAME_EXPORT
#define D_GEOFRAME_API __declspec(dllexport)
#else
#define D_GEOFRAME_API __declspec(dllimport)
#endif // GEOFRAME_EXPORT

#define D_GLAD <glad/gl.h>
#define D_GLFW <GLFW/glfw3.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include D_GLAD
#include D_GLFW

namespace GeoFrame {
// Typedefs
template <typename S, typename T> using Map = std::unordered_map<S, T>;
template <typename T> using Pair = std::pair<T, T>;
template <typename T> using Set = std::unordered_set<T>;
template <typename T> using Unique = std::unique_ptr<T>;
template <typename T> using Vec = std::vector<T>;
using ID = unsigned;
using Index = unsigned long long;
using Str = std::string;
} // namespace GeoFrame