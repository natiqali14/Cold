#pragma once



#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <time.h>



#include <iostream>
#include <cstdint>
#include <memory>
#include <utility>
#include <stdexcept>
#include <vector>
#include <array>
#include <functional>
#include <vector>
#include <queue>
#include <unordered_map>
#include <stack>
#include <unordered_set>

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8 = uint8_t;

using i64 = int64_t;
using i32 = int32_t;
using i16 = int16_t;
using i8 = int8_t;

using f32 = float;
using f64 = double;


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

#define DEBUG 1

#if defined(_WIN32) || defined(_WIN64)
#define WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
#define APPLE
#endif