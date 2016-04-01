project(Tracks)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-local-typedefs") #suppress warning in Aurora in Thor

find_package(SFML 2 REQUIRED graphics window system)
find_package(Thor 2 REQUIRED)
find_package(SFGUI REQUIRED)

include_directories(${SFML_INCLUDE_DIR} ${THOR_INCLUDE_DIR} ${SFGUI_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR})

set(HDRS
    Application.hpp
    Point.hpp
    ResourcePath.hpp
    Tracks.hpp
    TrackChunk.hpp
    FrameCounter.hpp
    Wagon.hpp
    Window.hpp
    Score.hpp
)

set(SRCS
    Application.cpp
    main.cpp
    Tracks.cpp
    TrackChunk.cpp
    FrameCounter.cpp
    Wagon.cpp
    Score.cpp
)

if(APPLE)
    set(SRCS ${SRCS} osx/ResourcePath.mm)
else()
    set(SRCS ${SRCS} other/ResourcePath.cpp)
endif()

if(APPLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${HDRS} ${SRCS})
else()
    add_executable(${PROJECT_NAME} ${HDRS} ${SRCS})
endif()

target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${THOR_LIBRARY} ${SFGUI_LIBRARY})

# Create a stand-alone bundle on OS X
if(APPLE)
    set(MAKE_INSTALL_NAME_DIR @rpath)

    # Add Apple libraries
    FIND_LIBRARY(FOUNDATION_LIBRARY Foundation)
    target_link_libraries(${PROJECT_NAME} ${FOUNDATION_LIBRARY})

    # Add Info.plist to bundle
    set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/osx/Info.plist.in)

    # Copy assets into the bundle
    set(BUNDLE_PATH ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.app)
    install(FILES ${PROJECT_SOURCE_DIR}/explosion00.png DESTINATION ${BUNDLE_PATH}/Contents/Resources)
    install(FILES ${PROJECT_SOURCE_DIR}/KBZipaDeeDooDah.ttf DESTINATION ${BUNDLE_PATH}/Contents/Resources)
    install(FILES ${PROJECT_SOURCE_DIR}/osx/Icon.icns DESTINATION ${BUNDLE_PATH}/Contents/Resources)

    # Make the bundle drag-n-drop
    get_filename_component(SFML_LIBRARY_DIR ${SFML_SYSTEM_LIBRARY} DIRECTORY)
    get_filename_component(THOR_LIBRARY_DIR ${THOR_LIBRARY_RELEASE} DIRECTORY)
    get_filename_component(SFGUI_LIBRARY_DIR ${SFGUI_LIBRARY_DYNAMIC_RELEASE} DIRECTORY)
    set(LIBRARIES_DIR ${THOR_LIBRARY_DIR};${SFML_LIBRARY_DIR})
    # Copy freetype manually, since it otherwise just causes problems (I think because it uses @executable_path instead of @rpath)
    install(DIRECTORY ${SFML_LIBRARY_DIR}/freetype.framework DESTINATION ${BUNDLE_PATH}/Contents/Frameworks)
    # Let BundleUtilities do everything else automaitcally
    install(CODE "
        include(BundleUtilities)
        fixup_bundle(\"${BUNDLE_PATH}\" \"\" \"${LIBRARIES_DIR}\")
        " COMPONENT Runtime)
else()
    install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
    FILE(GLOB images "*.png")
    install(FILES ${images} DESTINATION ${CMAKE_INSTALL_PREFIX})
    install(FILES KBZipaDeeDooDah.ttf DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()
