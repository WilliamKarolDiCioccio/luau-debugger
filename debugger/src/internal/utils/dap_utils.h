#pragma once
#include <concepts>
#include <string>

#include <dap/protocol.h>
#include <dap/typeof.h>

#include <Luau/BytecodeBuilder.h>
#include <Luau/Common.h>
#include <Luau/Compiler.h>
#include <lua.h>

#include <internal/log.h>

namespace luau::debugger::dap_utils {

template <class T>
concept Serializable =
    std::derived_from<T, dap::Request> || std::derived_from<T, dap::Event> ||
    std::derived_from<T, dap::Response>;

// Logging-only helper: returns the DAP type name.
// Full JSON serialization is intentionally omitted — NlohmannSerializer is a
// cppdap-internal header not available via vcpkg.
template <Serializable T>
inline std::string toString(const T& t) {
  return dap::TypeOf<T>::type()->name();
}

inline int clamp(std::size_t value) {
  return value & 0x7FFFFFFF;
}

}  // namespace luau::debugger::dap_utils