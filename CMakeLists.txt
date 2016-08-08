cmake_minimum_required(VERSION 3.5)
project(kristvanity)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/miner.cpp src/utils.cpp src/krist.cpp src/bytetohex.cpp)
add_executable(kristvanity ${SOURCE_FILES})

find_package(OpenSSL REQUIRED)

if (OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
    target_link_libraries(kristvanity ${OPENSSL_LIBRARIES})
endif()

find_package(Threads REQUIRED)
target_link_libraries(kristvanity ${CMAKE_THREAD_LIBS_INIT})

find_package(TCLAP REQUIRED)
if (TCLAP_FOUND)
    include_directories(${TCLAP_INCLUDE_DIR})
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

install(TARGETS kristvanity RUNTIME DESTINATION bin)
