set(library_name Luau.Debugger)
add_library(${library_name} STATIC)

target_sources(${library_name}
  PRIVATE
  src/debugger.cpp
  src/internal/breakpoint.cpp
  src/internal/file.cpp
  src/internal/debug_bridge.cpp
  src/internal/lua_statics.cpp
  src/internal/variable.cpp
  src/internal/variable_registry.cpp
  src/internal/vm_registry.cpp
  src/internal/utils/lua_utils.cpp
  src/internal/utils/lua_types.cpp
)

target_include_directories(${library_name} PRIVATE
  include
  src
)

target_compile_features(${library_name} PRIVATE cxx_std_20)
target_compile_definitions(${library_name} PUBLIC _DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  # ---- Standalone build: deps from adjacent source trees ----
  # cppdap and Luau come from add_subdirectory in the root CMakeLists.
  target_compile_definitions(cppdap PUBLIC _DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR)
  target_link_libraries(${library_name} PRIVATE cppdap)
  target_link_libraries(${library_name} PUBLIC Luau.VM Luau.Compiler)
  target_include_directories(${library_name} PRIVATE
    ${CPP_DAP_ROOT}/include
    ${CPP_DAP_ROOT}/src
    ${CPP_DAP_ROOT}/third_party/json/include
    ${LUAU_ROOT}/VM/src
  )
else()
  # ---- Subdirectory / vcpkg mode: use proper imported targets ----
  # cppdap::cppdap already found by parent CMakeLists.txt.
  target_link_libraries(${library_name} PRIVATE cppdap::cppdap)
  target_link_libraries(${library_name} PUBLIC
    unofficial::luau::Luau.VM
    unofficial::luau::Luau.Compiler)
  # vcpkg installs Luau private headers (lstate.h, lapi.h, lobject.h, …) under
  # <prefix>/include/luau/.  Adding that directory lets the debugger source use
  # #include <lstate.h> etc. without any source changes.
  find_path(LUAU_INCLUDE_DIR "luau/lua.h" REQUIRED)
  target_include_directories(${library_name} PRIVATE "${LUAU_INCLUDE_DIR}/luau")
endif()
