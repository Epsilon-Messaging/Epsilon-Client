cmake_minimum_required(VERSION 3.10)

project(EpsilonClient)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

# Download cpp-httplib
FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.20.0
)
FetchContent_MakeAvailable(httplib)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Add executable
add_executable(hello src/main.cpp)

# Link libraries
target_link_libraries(hello PRIVATE httplib::httplib OpenSSL::SSL OpenSSL::Crypto)

