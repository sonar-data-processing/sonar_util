cmake_minimum_required (VERSION 2.6)
project (sonar_util)

file ( GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp )
file ( GLOB HEADERS ${PROJECT_SOURCE_DIR}/src/*.hpp )

include_directories (
    ${CMAKE_INSTALL_PREFIX}/include/sonar_toolkit
)

add_library (
    sonar_util STATIC
    ${SOURCES}
)

install (
    FILES ${HEADERS}
    DESTINATION include/sonar_toolkit/${PROJECT_NAME}
)

install (
    TARGETS sonar_util
    DESTINATION lib/sonar_toolkit
)
