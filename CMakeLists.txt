cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(luau_debugger)

# Detect whether we are the top-level build or embedded as a subdirectory.
# CMAKE_SOURCE_DIR always points to the root of the outermost project, while
# PROJECT_SOURCE_DIR points to the directory that called project().
# They differ when this CMakeLists is processed via add_subdirectory().
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  # ---- Standalone build: resolve deps from adjacent source trees ----
  if(NOT DEFINED LUAU_ROOT)
    set(LUAU_ROOT ${CMAKE_SOURCE_DIR}/../luau)
  endif()
  if(NOT DEFINED CPP_DAP_ROOT)
    set(CPP_DAP_ROOT ${CMAKE_SOURCE_DIR}/../cppdap)
  endif()
  add_subdirectory(${LUAU_ROOT} ${CMAKE_BINARY_DIR}/luau)
  add_subdirectory(${CPP_DAP_ROOT} ${CMAKE_BINARY_DIR}/cppdap)
  add_subdirectory(debugger)
  add_subdirectory(luaud)
else()
  # ---- Subdirectory / vcpkg mode: deps come from the parent build ----
  # Luau targets (unofficial::luau::Luau.VM / Luau.Compiler) must already be
  # defined by the parent via find_package(unofficial-luau CONFIG REQUIRED).
  # cppdap is found here so debugger/CMakeLists.txt can use cppdap::cppdap.
  find_package(cppdap CONFIG REQUIRED)
  add_subdirectory(debugger)
  # luaud is a standalone demo binary; skip it when embedded.
endif()