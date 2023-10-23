#pragma once

// STDLIB

#include <cstdint>
#include <cassert>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <variant>

// TYPES

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

// DEBUGGING

#define LOG_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define LOG_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)

#ifdef TOY_DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#else 
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#endif

#include <spdlog/spdlog.h>

// ERROR HANDLING

namespace toy
{

using RuntimeError = std::runtime_error;


class Error
{
public:
    Error(const std::string& message) : message(message) {}

    const std::string& get_message() const { return message; }

private:
    std::string message;
};

template <typename T>
class Result
{
public:
    Result(const T& value) : data(value) {}
    Result(const Error& error) : data(error) {}

    bool has_value() const {
        return std::holds_alternative<T>(data);
    }

    bool has_error() const {
        return !has_value();
    }

    T value() const {
        return std::get<T>(data);
    }

    std::string error() const {
        return std::get<Error>(data).get_message();
    }

    T unwrap() const
    {
        if (has_value()) {
            return value();
        }
        else {
            throw RuntimeError(error());
        }
    }

private:
    std::variant<T, Error> data;
};

}
