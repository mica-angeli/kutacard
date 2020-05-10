cmake_minimum_required(VERSION 3.10)
project(kutacard)

set(wxBUILD_SHARED FALSE)
set(CMAKE_CXX_STANDARD 17)

# Mac OSX Bundle Properties
set(MACOSX_BUNDLE_NAME "Kutacard")
set(MACOSX_BUNDLE_PACKAGE_NAME "rangeli.kutacard")

add_subdirectory(libs/wxWidgets)

include_directories(include)
file(GLOB SRC_FILES "src/*.cpp")

add_executable(kutacard ${SRC_FILES})
set_target_properties(kutacard PROPERTIES
        OUTPUT_NAME ${MACOSX_BUNDLE_NAME}
        MACOSX_BUNDLE ON
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/MacOSXBundleInfo.plist.in)
target_link_libraries(kutacard net core base)
