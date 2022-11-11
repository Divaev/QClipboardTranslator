cmake_minimum_required(VERSION 3.5)

project(QClipboardTranslator LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTOUIC_SEARCH_PATHS src/forms)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#if(WIN32)
  #set(GUI_TYPE WIN32)

#add_definitions(-DWIN32 -D_WINDOWS -D_UNICODE -DUNICODE)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

#find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
#find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

set(SOURCES
    src/main.cpp
    src/maintranslatorwin.cpp
    src/pugixml.cpp
    src/words_finder.cpp
    src/selectdictionarydialog.cpp
    src/qglobalshortcut.cc
)


set(HEADERS
    includes/maintranslatorwin.h
    includes/pugiconfig.hpp
    includes/pugixml.hpp
    includes/words_finder.h
    includes/selectdictionarydialog.h
    includes/qglobalshortcut.h
    )

set(FORMS
    src/forms/maintranslatorwin.ui
    src/forms/selectdictionarydialog.ui
    #src/maintranslatorwin.ui
    #src/selectdictionarydialog.ui
    )

qt5_add_resources(RESOURCES resources/resources.qrc)

include_directories( ${CMAKE_SOURCE_DIR}/includes
                     ${CMAKE_SOURCE_DIR}/src
                     )

#qt_add_executable(QClipboardTranslator
#    ${SOURCES}
#    ${HEADERS}
#    ${FORMS}
#)

add_executable(QClipboardTranslator WIN32
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
    ${RESOURCES}
)

if (WIN32)
    target_sources(QClipboardTranslator
            PUBLIC src/qglobalshortcut_win.cc

    )
endif()

if (UNIX)
    target_sources(QClipboardTranslator
            PUBLIC src/qglobalshortcut_x11.cc
    )
endif()


target_link_libraries(QClipboardTranslator PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
