/*
    SPDX-FileCopyrightText: 2026 Garward
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KCModule>
#include "ui_saturation_config.h"

class SaturationEffectConfig : public KCModule
{
    Q_OBJECT

public:
    explicit SaturationEffectConfig(QObject *parent, const KPluginMetaData &data);

public Q_SLOTS:
    void load() override;
    void save() override;
    void defaults() override;

private:
    Ui::SaturationEffectConfig m_ui;
};
