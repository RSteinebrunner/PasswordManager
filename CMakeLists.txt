# CMakeLists.txt (project root)

cmake_minimum_required(VERSION 3.10)
project(PasswordManager VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Core library (vault, crypto later, etc.) ----
add_library(pm_core STATIC
    src/vault.cpp
)

target_include_directories(pm_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pm
)

# ---- CLI app ----
add_executable(pm_cli
    apps/cli/src/main.cpp
) 

target_link_libraries(pm_cli PRIVATE pm_core)

# Keep your original executable name if you want
set_target_properties(pm_cli PROPERTIES OUTPUT_NAME "pm")


include(CTest)
enable_testing()
