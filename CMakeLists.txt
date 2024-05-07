cmake_minimum_required(VERSION 3.5)

project(SoftWorld VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        UI/sceneview.cpp
        UI/sceneview.h
        UI/palleteitem.h
        UI/palleteitem.cpp
        UI/pallete.h
        UI/pallete.cpp
        Physics/physicalbody.h
        Physics/physicalbody.cpp
        Physics/ghostbody.h
        Physics/ghostbody.cpp


        Utils/fileworks.h
        Utils/fileworks.cpp
        Serialize/ser_class_enums.h
        Serialize/deser_helpers.h
        Serialize/desertab.h
        Serialize/deserialize.h
        Serialize/deserialize.cpp
        Serialize/saveid.cpp
        Serialize/serialize_special.h
        Serialize/serialize_special.cpp
        Serialize/serialize.h
        Serialize/serialize_ui.h
        Serialize/serialize_ui.cpp
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SoftWorld
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}


        Physics/softscene.h Physics/softscene.cpp
        Inspector/action.h
        icons.qrc
        Inspector/inspectornumericfield.h
        Inspector/inspectoritem.h
        Inspector/inspectorheader.h Inspector/inspectorheader.cpp
        Inspector/inspectorbutton.h Inspector/inspectorbutton.cpp
        Inspector/inspectorcolorfield.h Inspector/inspectorcolorfield.cpp
        Inspector/slidermanagers.h Inspector/slidermanagers.cpp
        Inspector/coloricon.h Inspector/coloricon.cpp
        Inspector/inspector.h Inspector/inspector.cpp
        Inspector/inspectoritem.cpp
        Physics/editonlybody.h Physics/editonlybody.cpp

        Utils/cstringcomparator.h Utils/cstringcomparator.cpp
        Utils/icons.cpp Utils/icons.h
        Utils/colorutility.h Utils/colorutility.cpp
        Inspector/editingmanager.h
        Inspector/inspectorstringfield.h Inspector/inspectorstringfield.cpp
        Threads/physicalthread.h Threads/physicalthread.cpp
        Threads/simulationthreadscontroller.h Threads/simulationthreadscontroller.cpp
        Threads/drawingthread.h Threads/drawingthread.cpp
        Inspector/inspectorlabel.h Inspector/inspectorlabel.cpp
        Inspector/inspectorlabelmanager.h Inspector/inspectorlabelmanager.cpp
        Utils/drawingstyle.cpp Utils/drawingstyle.h
        CozyWidgets/statebutton.h CozyWidgets/statebutton.cpp
        Physics/polypoint.h Physics/polypoint.cpp
        Physics/polygonphysicalshape.h Physics/polygonphysicalshape.cpp
        Physics/testpolybody.h Physics/testpolybody.cpp
        Physics/igball.h Physics/igball.cpp
        Physics/linesintersectioninfo.h Physics/linesintersectioninfo.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET SoftWorld APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(SoftWorld SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(SoftWorld
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(SoftWorld PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SoftWorld)
endif()
target_include_directories(SoftWorld PRIVATE ./)

set_target_properties(SoftWorld PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SoftWorld
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SoftWorld)
endif()
