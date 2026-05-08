# SPDX-FileCopyrightText: 2026 Garward
# SPDX-License-Identifier: GPL-2.0-or-later

cmake_minimum_required(VERSION 3.20)
project(kwin-saturation-effect VERSION 1.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(QT_DEFAULT_MAJOR_VERSION 6)

# Pin ECM to 6.0+ so KDE_INSTALL_PLUGINDIR resolves to Qt6 paths.
# Without the version, ECM can pick a Qt5 install layout on systems where both are installed.
find_package(ECM 6.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMQtDeclareLoggingCategory)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets DBus)
find_package(KF6 REQUIRED COMPONENTS Config KCMUtils I18n)
find_package(KWin REQUIRED)

# Generate config class from kcfg
kconfig_add_kcfg_files(SATURATION_KCFG_SRCS saturationconfig.kcfgc)

#####################################
# Effect plugin
#####################################
add_library(saturation MODULE
    main.cpp
    saturation.cpp
    saturation.qrc
    ${SATURATION_KCFG_SRCS}
)

set_target_properties(saturation PROPERTIES PREFIX "")

target_include_directories(saturation PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(saturation PRIVATE
    KWin::kwin
    KF6::ConfigCore
    KF6::ConfigGui
    Qt6::Core
    Qt6::Gui
)

# Install effect plugin
install(TARGETS saturation
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/kwin/effects/plugins
)

# Install metadata
install(FILES metadata.json
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/kwin/effects/plugins
    RENAME saturation.json
)

#####################################
# Config module (KCM)
#####################################
add_library(kwin_saturation_config MODULE
    saturation_config.cpp
    saturation_config.ui
    ${SATURATION_KCFG_SRCS}
)

target_include_directories(kwin_saturation_config PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(kwin_saturation_config PRIVATE
    KF6::KCMUtils
    KF6::ConfigCore
    KF6::ConfigGui
    KF6::I18n
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::DBus
)

install(TARGETS kwin_saturation_config
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/kwin/effects/configs
)
