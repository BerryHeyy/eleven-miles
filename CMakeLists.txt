cmake_minimum_required (VERSION 3.12)

project (eleven-miles)

add_subdirectory(dependencies/glfw/)

add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC dependencies/glfw/include/)
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

    target_include_directories(${PROJECT_NAME} PUBLIC ${VULKAN_PATH}/Include)
    link_libraries(${VULKAN_PATH}/Bin ${VULKAN_PATH}/Lib)
endif(WIN32)
if (UNIX)
    message(STATUS "Generating build files specifically for gcc.")


endif(UNIX)
