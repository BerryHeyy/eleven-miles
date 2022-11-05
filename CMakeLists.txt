cmake_minimum_required (VERSION 3.14)

project (vulkan-triangle)

set(DCMAKE_BUILD_TYPE "Debug")

add_subdirectory(dependencies/glfw/)

set(EM_SOURCES
    vulkan-triangle.cpp
    util.hpp
    util.cpp
)

list(TRANSFORM EM_SOURCES PREPEND "src/")

add_executable(${PROJECT_NAME} ${EM_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE dependencies/glfw/include/)
target_link_libraries(${PROJECT_NAME} glfw)

if(WIN32)
    message(STATUS "Generating build files specifically for windows.")
    
    message(STATUS "Attempting to locate Vulkan SDK using manual path...")
	set(VULKAN_SDK "C:/VulkanSDK")
	set(VULKAN_VERSION "1.3.231.1")
	set(VULKAN_PATH "${VULKAN_SDK}/${VULKAN_VERSION}")
	message(STATUS "Using manual specified path: ${VULKAN_PATH}")

    # Check if manual set path exists
	if(NOT EXISTS "${VULKAN_PATH}")
        message("Error: Unable to locate this Vulkan SDK path VULKAN_PATH: ${VULKAN_PATH}, please specify correct path.")
        return()
    endif()

    target_include_directories(${PROJECT_NAME} PRIVATE ${VULKAN_PATH}/Include)
    target_link_libraries(${PROJECT_NAME} ${VULKAN_PATH}/Lib/vulkan-1.lib)

    if(NOT EXISTS "${CMAKE_BINARY_DIR}/shaders")
        message(STATUS "Creating symlink for shaders.")

        file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/shaders" SYM_SHADER_LINK)
        file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/shaders" SYM_SHADER_TARGET)
    
        execute_process(
            COMMAND cmd.exe /c mklink /J "${SYM_SHADER_LINK}" "${SYM_SHADER_TARGET}"
        )
    endif()
endif(WIN32)
if (UNIX)
    message(STATUS "Generating build files specifically for linux.")

    target_link_libraries(${PROJECT_NAME} vulkan)

    if(NOT EXISTS "${CMAKE_BINARY_DIR}/shaders")
        message(STATUS "Creating symlink for shaders.")

        execute_process(
            COMMAND ln -s ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_BINARY_DIR}/shaders
        )
    endif()
endif(UNIX)
