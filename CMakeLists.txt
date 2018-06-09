cmake_minimum_required(VERSION 3.0.0)

project(CANdevStudio CXX)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
#libnodes.so will be installed to CANdevStudio dir to not override other installations. Set RPATH accordingly
SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib/CANDevStudio)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

option(WITH_COVERAGE "Build with coverage" OFF)
option(WITH_TOOLS "Build tools directory" OFF)
option(DEV_BUILD "Development build will include GIT hash and branch to versioning" ON)

if(NOT WIN32)
    option(WITH_TESTS "Build with test" ON)
else()
    # TODO: MSVC does not like "#define private public" need to find a way to disable it.
    option(WITH_TESTS "Build with test" OFF)
    set(GUI_TYPE WIN32)
endif()

set(QT_REQUIRED_VERSION "5.6")
find_package(Qt5Core ${QT_REQUIRED_VERSION} REQUIRED)
find_package(Qt5SerialBus ${QT_REQUIRED_VERSION} REQUIRED)
find_package(Qt5Widgets ${QT_REQUIRED_VERSION} REQUIRED)
find_package(Qt5Test ${QT_REQUIRED_VERSION} REQUIRED)
message(STATUS "Qt Version found: ${Qt5Core_VERSION}")

set_property(TARGET Qt5::SerialBus PROPERTY INTERFACE_COMPILE_FEATURES "")
set_property(TARGET Qt5::Core PROPERTY INTERFACE_COMPILE_FEATURES "")

if(NOT MSVC)
    add_definitions(-Wall -Wextra)
else()
    add_definitions( /bigobj)
endif()

include(CompilerVersion)

if(WITH_COVERAGE)
    include(CodeCoverage)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_COVERAGE})
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS_COVERAGE})
    setup_target_for_coverage(NAME coverage EXECUTABLE ctest DEPENDENCIES candevice_test)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(SPDLOG_BUILD_TESTING CACHE FORCE OFF)
set(BUILD_EXAMPLES CACHE FORCE OFF)

# Version handling
if(EXISTS "${CMAKE_SOURCE_DIR}/.git" AND DEV_BUILD)
  execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  execute_process(
    COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
else()
  set(GIT_BRANCH "")
  set(GIT_COMMIT_HASH "")
endif()

message(STATUS "Git current branch: ${GIT_BRANCH}")
message(STATUS "Git commit hash: ${GIT_COMMIT_HASH}")

message(STATUS "Generating version.h")

configure_file(
  ${CMAKE_SOURCE_DIR}/src/common/version.h.in
  ${CMAKE_BINARY_DIR}/generated/version.h
)
include_directories(${CMAKE_BINARY_DIR}/generated)
# end of version Handling

include_directories(3rdParty/readerwriterqueue)
include_directories(3rdParty/cxxopts/include)

add_subdirectory(3rdParty/nodeeditor)
add_subdirectory(src/common)
add_subdirectory(src/gui)
add_subdirectory(src/components)

if(WITH_TESTS OR WITH_COVERAGE)
    enable_testing()
    add_subdirectory(3rdParty/fakeit)
    add_subdirectory(tests)
endif()

if(WITH_TOOLS)
    find_package(Boost COMPONENTS system filesystem REQUIRED)

    add_subdirectory(tools)
endif()

#Cannelloni is supported on Linux only
if(UNIX)
    add_subdirectory(3rdParty/qtCannelloniCanBus)

    install(PROGRAMS ${CMAKE_BINARY_DIR}/3rdParty/qtCannelloniCanBus/libqtCannelloniCanBusPlugin.so 
        DESTINATION lib/qt/plugins/canbus
    )
    install(PROGRAMS ${CMAKE_BINARY_DIR}/lib/libnodes.so
        DESTINATION lib/CANDevStudio
    )
else()
    set(SYS_LIBS
        libgcc_s_dw2-1.dll
        libstdc++-6.dll
        libwinpthread-1.dll
        Qt5Core.dll
        Qt5Gui.dll
        Qt5Network.dll
        Qt5SerialBus.dll
        Qt5SerialPort.dll
        Qt5Svg.dll
        Qt5Widgets.dll
        )

    get_target_property(QtCore_location Qt5::Core LOCATION)
    get_filename_component(QtPath ${QtCore_location} DIRECTORY)

    foreach(SYS_LIB ${SYS_LIBS})
        set(SYS_LIBS_ABS ${SYS_LIBS_ABS} ${QtPath}/${SYS_LIB})
    endforeach()

    install(PROGRAMS ${CMAKE_BINARY_DIR}/bin/libnodes.dll ${SYS_LIBS_ABS}
        DESTINATION CANDevStudio
    )
endif()

include("packaging.cmake")
