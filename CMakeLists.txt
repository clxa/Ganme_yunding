cmake_minimum_required(VERSION 3.16)
#├── Core
#│   ├── ScreenCapturer (截图模块)
#│   ├── ImageProcessor (图像处理模块)
#│   ├── DataManager (数据管理模块)
#│   ├── ModelTrainer (模型训练模块)
#│   └── DecisionEngine (决策引擎模块)
#├── AI
#│   ├── InferenceEngine (推理引擎)
#│   └── ModelWrapper (模型接口)
#└── UI
#    ├── MainWindow (主界面)
#   ├── DashboardWidget (数据仪表盘)
#    └── SettingsDialog (设置面板)
project(TingTaoHelper VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TingTaoHelper
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        core_screencapturer.cpp
        core_imageprocessor.h core_imageprocessor.cpp
        core_datamanager.h core_datamanager.cpp
        core_modeltrainer.h core_modeltrainer.cpp
        core_decisionengine.h core_decisionengine.cpp
        ai_inferenceengine.h ai_inferenceengine.cpp
        ai_modelwrapper.h ai_modelwrapper.cpp
        core_screencapturer.h

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET TingTaoHelper APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(TingTaoHelper SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(TingTaoHelper
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(TingTaoHelper PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TingTaoHelper)
endif()
set_target_properties(TingTaoHelper PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS TingTaoHelper
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TingTaoHelper)
endif()


