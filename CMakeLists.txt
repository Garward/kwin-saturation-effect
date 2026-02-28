# SPDX-FileCopyrightText: 2026 Garward
# SPDX-License-Identifier: GPL-2.0-or-later

cmake_minimum_required(VERSION 3.20)
project(kwin-saturation-effect VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QT_DEFAULT_MAJOR_VERSION 6)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMQtDeclareLoggingCategory)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(KF6 REQUIRED COMPONENTS Config)
find_package(KWin REQUIRED)

# Generate config class from kcfg
kconfig_add_kcfg_files(SATURATION_KCFG_SRCS saturationconfig.kcfgc)

# Create the plugin
add_library(kwin_effect_saturation MODULE
    main.cpp
    saturation.cpp
    saturation.qrc
    ${SATURATION_KCFG_SRCS}
)

target_include_directories(kwin_effect_saturation PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(kwin_effect_saturation PRIVATE
    KWin::kwin
    KF6::ConfigCore
    KF6::ConfigGui
    Qt6::Core
    Qt6::Gui
)

# Install plugin
install(TARGETS kwin_effect_saturation
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/kwin/effects/plugins
)

# Install metadata
install(FILES metadata.json
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/kwin/effects/plugins
    RENAME kwin_effect_saturation.json
)
