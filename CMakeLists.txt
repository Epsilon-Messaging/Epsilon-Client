cmake_minimum_required(VERSION 3.10)

project(EpsilonClient)

set(CMAKE_CXX_STANDARD 23)

# Find Boost
find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Add FTXUI
include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v6.0.2
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Add executable
add_executable(epsilon src/main.cpp)

# Link libraries
target_link_libraries(epsilon 
    Boost::program_options 
    OpenSSL::SSL 
    OpenSSL::Crypto 
    ftxui::screen 
    ftxui::dom 
    ftxui::component
)

