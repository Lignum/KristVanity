cmake_minimum_required(VERSION 3.5)
project(kristvanity)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Taken from: https://cmake.org/pipermail/cmake/2013-November/056336.html
macro(group_sources curdir)
	file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)
	foreach(child ${children})
		if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
			group_sources(${curdir}/${child})
		else()
			string(REPLACE "/" "\\" groupname ${curdir})
			source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
		endif()
	endforeach()
endmacro()

group_sources("src")
group_sources("include")

file(GLOB_RECURSE SOURCE_FILES "include/*.h" "include/*.hpp" "src/*.cpp")
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
