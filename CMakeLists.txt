cmake_minimum_required(VERSION 3.10)

project(EpsilonClient)

set(CMAKE_CXX_STANDARD 23)

# Find Boost
find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Add executable
add_executable(epsilon src/main.cpp)

# Link libraries
target_link_libraries(epsilon Boost::program_options OpenSSL::SSL OpenSSL::Crypto)

