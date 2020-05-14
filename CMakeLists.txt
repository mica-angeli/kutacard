cmake_minimum_required(VERSION 3.10)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

project(kutacard)

set(wxBUILD_SHARED FALSE)
set(CMAKE_CXX_STANDARD 17)

# Mac OSX Bundle Properties
set(MACOSX_BUNDLE_NAME "Kutacard")
set(MACOSX_BUNDLE_PACKAGE_NAME "rangeli.kutacard")

add_subdirectory(libs/argparse)
add_subdirectory(libs/wxWidgets)

include_directories(include)

add_executable(kutacard src/mainapp.cpp src/mainframe.cpp src/memorycard.cpp src/savegame.cpp)
set_target_properties(kutacard PROPERTIES
        OUTPUT_NAME ${MACOSX_BUNDLE_NAME}
        MACOSX_BUNDLE ON
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/MacOSXBundleInfo.plist.in)
target_link_libraries(kutacard net core base)

add_executable(kutacli src/kutacli.cpp src/memorycard.cpp)
target_link_libraries(kutacli argparse)
