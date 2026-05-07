/*
    SPDX-FileCopyrightText: 2026 Garward
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "saturation_config.h"

#include <QComboBox>
#include <KPluginFactory>
#include <KConfig>
#include <KConfigGroup>
#include <KSharedConfig>
#include <QDBusConnection>
#include <QDBusMessage>

K_PLUGIN_CLASS(SaturationEffectConfig)

SaturationEffectConfig::SaturationEffectConfig(QObject *parent, const KPluginMetaData &data)
    : KCModule(parent, data)
{
    m_ui.setupUi(widget());

    connect(m_ui.kcfg_Saturation, &QSlider::valueChanged, this, [this](int value) {
        m_ui.saturationLabel->setText(QString::number(value) + QStringLiteral("%"));
        setNeedsSave(true);
    });

    connect(m_ui.kcfg_Mode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int) {
        setNeedsSave(true);
    });

    load();
}

void SaturationEffectConfig::load()
{
    KCModule::load();

    KConfig config(QStringLiteral("kwinrc"));
    KConfigGroup group = config.group(QStringLiteral("Effect-saturation"));
    int saturation = group.readEntry("Saturation", 150);
    int mode = group.readEntry("Mode", 0);

    m_ui.kcfg_Saturation->setValue(saturation);
    m_ui.saturationLabel->setText(QString::number(saturation) + QStringLiteral("%"));
    m_ui.kcfg_Mode->setCurrentIndex(mode);

    setNeedsSave(false);
}

void SaturationEffectConfig::save()
{
    KSharedConfigPtr config = KSharedConfig::openConfig(QStringLiteral("kwinrc"));
    KConfigGroup group = config->group(QStringLiteral("Effect-saturation"));
    group.writeEntry("Saturation", m_ui.kcfg_Saturation->value());
    group.writeEntry("Mode", m_ui.kcfg_Mode->currentIndex());
    group.sync();

    // Notify KWin to reconfigure the effect
    QDBusMessage message = QDBusMessage::createMethodCall(
        QStringLiteral("org.kde.KWin"),
        QStringLiteral("/Effects"),
        QStringLiteral("org.kde.kwin.Effects"),
        QStringLiteral("reconfigureEffect")
    );
    message << QStringLiteral("saturation");
    QDBusConnection::sessionBus().send(message);

    setNeedsSave(false);
}

void SaturationEffectConfig::defaults()
{
    m_ui.kcfg_Saturation->setValue(150);
    m_ui.saturationLabel->setText(QStringLiteral("150%"));
    m_ui.kcfg_Mode->setCurrentIndex(0);
    setNeedsSave(true);
}

#include "saturation_config.moc"
