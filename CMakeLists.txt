cmake_minimum_required(VERSION 3.30.0)
project(Zapascape VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "" FORCE)
set(CMAKE_WARN_DEPRECATED OFF)
set(POSITION_INDEPENDENT_CODE ON)

include(FetchContent)

FetchContent_Declare(
    HUGO
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/HUGO/HUGO
)

set(HUGO_BUILD_ENGINE ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(HUGO)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})


# Source files

file(GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Game target declaration

add_library(${PROJECT_NAME} MODULE ${${PROJECT_NAME}_SRC_FILES})

# Include dirs

file(GLOB_RECURSE ${PROJECT_NAME}_DIRS CONFIGURE_DEPENDS LIST_DIRECTORIES true ${CMAKE_SOURCE_DIR}/src/*)
set(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src)

foreach(path ${${PROJECT_NAME}_DIRS})
    if(IS_DIRECTORY "${path}")
        list(APPEND ${PROJECT_NAME}_INCLUDE_DIRS "${path}")
    endif()
endforeach()

target_include_directories(${PROJECT_NAME} PRIVATE ${${PROJECT_NAME}_INCLUDE_DIRS})

# Linking

target_link_libraries(${PROJECT_NAME} PRIVATE 
    HugoAPI)

# Properties

set_target_properties(${PROJECT_NAME} PROPERTIES
    DEBUG_POSTFIX "d"
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

add_custom_target(copy_config
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/hugo.config
    ${CMAKE_SOURCE_DIR}/bin/hugo.config
    COMMAND_EXPAND_LISTS)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT HUGO)
add_dependencies(HUGO ${PROJECT_NAME} copy_config)

if(MSVC)
    set_target_properties(
        copy_config
        PROPERTIES FOLDER "Tools"
    )
endif()