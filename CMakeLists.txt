cmake_minimum_required(VERSION 3.5)

project(QClipboardTranslator LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    src/clipboardchkr.cpp
    src/words_finder.cpp
)

set(HEADERS
    includes/maintranslatorwin.h
    includes/pugiconfig.hpp
    includes/pugixml.hpp
    includes/clipboardchkr.h
    includes/words_finder.h
    )

set(FORMS
    src/forms/maintranslatorwin.ui
    )

include_directories( ${CMAKE_SOURCE_DIR}/includes
                     ${CMAKE_SOURCE_DIR}/src)

#qt_add_executable(QClipboardTranslator
#    ${SOURCES}
#    ${HEADERS}
#    ${FORMS}
#)

add_executable(QClipboardTranslator
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
)

#if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#    qt_add_executable(QClipboardTranslator
#        ${PROJECT_SOURCES}
#    )
#else()
#    if(ANDROID)
#        add_library(QClipboardTranslator SHARED
#            ${PROJECT_SOURCES}
#        )
#    else()
#        add_executable(QClipboardTranslator
#            ${PROJECT_SOURCES}
#        )
#    endif()
#endif()

target_link_libraries(QClipboardTranslator PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
