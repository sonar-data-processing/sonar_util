cmake_minimum_required (VERSION 2.6)
project (sonar_util)

set (
    ${PROJECT_NAME}_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/include
    CACHE PATH "Sonar Util Include Directory"
    FORCE
)

file ( GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp )

include_directories ( 
    ${base_INCLUDE_DIR}
    ${sonar_util_INCLUDE_DIR}
)

add_library ( sonar_util STATIC ${SOURCES} )
