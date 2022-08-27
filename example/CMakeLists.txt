cmake_minimum_required(VERSION 3.6)

set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "build type")

project(example
		LANGUAGES C CXX
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

find_library(LIBRT rt)
find_package(OpenSSL REQUIRED)
find_package(workflow REQUIRED CONFIG HINTS ..)
find_package(biomq REQUIRED CONFIG HINTS ..)
include_directories(${OPENSSL_INCLUDE_DIR} ${WORKFLOW_INCLUDE_DIR} ${BIOMQ_INCLUDE_DIR})
link_directories(${WORKFLOW_LIB_DIR})
link_directories(${BIOMQ_LIB_DIR})

set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall -fPIC -pipe -std=gnu90")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -pipe -std=c++11 -fno-exceptions")

set(EXAMPLE_LIST
	demo_broker
	demo_pub
	demo_sub
)

set(BIOMQ_LIB biomq workflow pthread OpenSSL::SSL OpenSSL::Crypto ${LIBRT})

foreach(src ${EXAMPLE_LIST})
	string(REPLACE "-" ";" arr ${src})
	list(GET arr -1 bin_name)
	add_executable(${bin_name} ${src}.cpp)
	target_link_libraries(${bin_name} ${BIOMQ_LIB})
endforeach()
