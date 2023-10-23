#pragma once

#include <cstdint>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <unordered_map>
#include <memory>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;

template<typename T> using ptr = std::unique_ptr<T>;
template<typename T> using ref = std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr ptr<T> make_ptr(Args&& ... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename ... Args>
constexpr ref<T> make_ref(Args&& ... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

namespace toy
{

using String = std::string;
template<class T, u32 N> using Array = std::array<T, N>;
template<class T> using Vector = std::vector<T>;
template<class T> using List = std::list<T>;
template<class Key, class T> using Dictionary = std::unordered_map<Key, T>;

}