cmake_minimum_required(VERSION 3.16)

project(LaboratoryWork3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LIB_SOURCES
    src/markdown.cpp
    src/userdata.cpp
)

add_library(lab3_static STATIC ${LIB_SOURCES})
target_include_directories(lab3_static PUBLIC include)
target_compile_definitions(lab3_static PUBLIC LAB3_STATIC)
set_target_properties(lab3_static PROPERTIES OUTPUT_NAME "lab3_static")

add_library(lab3_dynamic SHARED ${LIB_SOURCES})
target_include_directories(lab3_dynamic PUBLIC include)
target_compile_definitions(lab3_dynamic PRIVATE LAB3_BUILD)
set_target_properties(lab3_dynamic PROPERTIES OUTPUT_NAME "lab3_dynamic")

add_executable(StaticDemo
    demo/main.cpp
    demo/application.cpp
)
target_include_directories(StaticDemo PRIVATE include demo)
target_link_libraries(StaticDemo PRIVATE lab3_static)

add_executable(DynamicDemo
    demo/main.cpp
    demo/application.cpp
)
target_include_directories(DynamicDemo PRIVATE include demo)
target_link_libraries(DynamicDemo PRIVATE lab3_dynamic)

foreach(target StaticDemo DynamicDemo)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/text.md"
        "$<TARGET_FILE_DIR:${target}>/text.md"
    )
endforeach()
