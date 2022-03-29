cmake_minimum_required(VERSION 3.17)
project(Project-Space)


set(CMAKE_CXX_STANDARD 17)

get_filename_component(PARENTDIR ${PROJECT_BINARY_DIR} DIRECTORY)

file(GLOB_RECURSE SOURCES 
./src/*.cpp
./src/*.h
./src/*.hpp
)

add_executable(Project-Space ${SOURCES})
set_property(DIRECTORY ${PROJECT_BINARY_DIR} PROPERTY VS_STARTUP_PROJECT Project-Space)

add_compile_definitions(GLFW_INCLUDE_NONE)

include_directories("${PROJECT_BINARY_DIR}/libraries/glad/include" "${PROJECT_BINARY_DIR}/libraries/glfw3/include" "${PROJECT_BINARY_DIR}/libraries/glm/include" "${PROJECT_BINARY_DIR}/libraries/stb_image/include")

# target_link_libraries(Project-Space PRIVATE opengl glad glfw3 glm stb_image)
find_package(OpenGL REQUIRED)
if (CMAKE_SIZEOF_VOID_P EQUAL 4)
    target_link_libraries(Project-Space PRIVATE 
        "opengl32.lib"
        "${PROJECT_BINARY_DIR}/libraries/glad/WIN32/glad.lib"
        "${PROJECT_BINARY_DIR}/libraries/glfw3/WIN32/glfw3.lib"
        "${PROJECT_BINARY_DIR}/libraries/glm/WIN32/glm.lib"
        "${PROJECT_BINARY_DIR}/libraries/stb_image/WIN32/stb_image.lib"
		"${PROJECT_BINARY_DIR}/libraries/freetype/WIN32/freetype.lib"
    )
    target_include_directories(Project-Space PRIVATE
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_BINARY_DIR}/libraries/glad/include"
        "${PROJECT_BINARY_DIR}/libraries/glfw3/WIN32/include"
        "${PROJECT_BINARY_DIR}/libraries/glm/include"
        "${PROJECT_BINARY_DIR}/libraries/stb_image/include"
        "${PROJECT_BINARY_DIR}/libraries/freetype/include"
    )
else()
    target_link_libraries(Project-Space PRIVATE 
        "opengl32.lib"
        "${PROJECT_BINARY_DIR}/libraries/glad/WIN64/glad.lib"
        "${PROJECT_BINARY_DIR}/libraries/glfw3/WIN64/glfw3.lib"
        "${PROJECT_BINARY_DIR}/libraries/glm/WIN64/glm.lib"
        "${PROJECT_BINARY_DIR}/libraries/stb_image/WIN64/stb_image.lib"
		"${PROJECT_BINARY_DIR}/libraries/freetype/WIN64/freetype.lib"
    )
    target_include_directories(Project-Space PRIVATE
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_BINARY_DIR}/libraries/glad/include"
        "${PROJECT_BINARY_DIR}/libraries/glfw3/WIN64/include"
        "${PROJECT_BINARY_DIR}/libraries/glm/include"
        "${PROJECT_BINARY_DIR}/libraries/stb_image/include"
        "${PROJECT_BINARY_DIR}/libraries/freetype/include"
    )
endif()
