cmake_minimum_required(VERSION 3.24.2)
project(fl_studio_plugin_organizer
        VERSION 1.2.0
        DESCRIPTION "FL Studio Plugin Organizer"
        HOMEPAGE_URL "https://github.com/laurence-myers/flspo"
        LANGUAGES CXX
        )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POLICY_DEFAULT_CMP0135 NEW) # Avoid a CMake warning

################
# Platform config

# Fix DLL not found problem on Windows
# Note: Windows shared libraries do not have RPATH or RUNPATH
# DLLs are searched first at current directories and then in directories
# listed in $PATH variable.
# @see https://caiorss.github.io/C-Cpp-Notes/building-system-cmake.html#orgfdf5850
if (WIN32)
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
endif ()

# Statically link the Windows runtime dependencies
if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY  "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()

###################
# Install CPM.cmake

set(CPM_DOWNLOAD_VERSION 0.35.0)

if (CPM_SOURCE_CACHE)
    set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
elseif (DEFINED ENV{CPM_SOURCE_CACHE})
    set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
else ()
    set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
endif ()

if (NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
    file(DOWNLOAD
            https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
            ${CPM_DOWNLOAD_LOCATION}
            )
endif ()

include(${CPM_DOWNLOAD_LOCATION})

##############
# Dependencies

CPMAddPackage(
        NAME nowide          # The unique name of the dependency (should be the exported target's name)
        VERSION 11.1.4       # The minimum version of the dependency (optional, defaults to 0)
        #    OPTIONS       # Configuration options passed to the dependency (optional)
        #    DOWNLOAD_ONLY # If set, the project is downloaded, but not configured (optional)
        URL "https://github.com/boostorg/nowide/releases/download/v11.1.4/nowide_standalone_v11.1.4.tar.gz"         # Origin parameters forwarded to FetchContent_Declare
)

##############
# App settings

add_executable(flspo main.cpp read.cpp read.h common.h write.cpp write.h error.h)

# Set the output exe name
set_target_properties(flspo PROPERTIES OUTPUT_NAME "flspo")

###################
# Link dependencies

target_link_libraries(flspo nowide)

################
# Package as zip

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
install(
        FILES
        "${CMAKE_SOURCE_DIR}/COPYING"
        ${CPACK_RESOURCE_FILE_LICENSE}
        ${CPACK_RESOURCE_FILE_README}
        DESTINATION
        .
        COMPONENT
        flspo
)

include(CPackComponent)
cpack_add_component(
        flspo
        DISPLAY_NAME "FL Studio Plugin Organizer"
        REQUIRED
)

install(
        TARGETS
        flspo
        RUNTIME
        DESTINATION
        .
        COMPONENT
        flspo
)

set(CPACK_BINARY_NSIS "OFF")
set(CPACK_BINARY_ZIP "ON")
set(CPACK_PACKAGE_NAME "flspo")
set(CPACK_PACKAGE_VENDOR "Laurence Dougal Myers")

set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR};flspo;flspo;/")

include(CPack)
