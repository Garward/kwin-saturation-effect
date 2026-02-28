/*
    SPDX-FileCopyrightText: 2026 Garward
    SPDX-License-Identifier: GPL-2.0-or-later

    Saturation Boost - A KWin effect that increases color saturation
    Similar to NVIDIA Digital Vibrance or AMD Saturation settings
*/

#include "saturation.h"
#include "saturationconfig.h"

#include "effect/effecthandler.h"
#include "opengl/glshader.h"

static void ensureResources()
{
    Q_INIT_RESOURCE(saturation);
}

namespace KWin
{

SaturationEffect::SaturationEffect()
    : OffscreenEffect()
{
    SaturationSettings::instance(effects->config());
    m_saturation = std::clamp<float>(SaturationSettings::saturation(), 0.0f, 4.0f);

    loadData();
}

SaturationEffect::~SaturationEffect()
{
}

bool SaturationEffect::supported()
{
    return effects->isOpenGLCompositing();
}

void SaturationEffect::loadData()
{
    ensureResources();

    m_shader = ShaderManager::instance()->generateShaderFromFile(
        ShaderTrait::MapTexture,
        QString(),
        QStringLiteral(":/effects/saturation/shaders/saturation.frag")
    );

    if (!m_shader->isValid()) {
        qWarning() << "SaturationEffect: Failed to load shader!";
        return;
    }

    ShaderBinder binder{m_shader.get()};
    m_shader->setUniform("saturationAmount", m_saturation);

    for (const auto windows = effects->stackingOrder(); EffectWindow *w : windows) {
        applyToWindow(w);
    }
    effects->addRepaintFull();

    connect(effects, &EffectsHandler::windowDeleted, this, &SaturationEffect::slotWindowDeleted);
    connect(effects, &EffectsHandler::windowAdded, this, &SaturationEffect::applyToWindow);
}

void SaturationEffect::applyToWindow(KWin::EffectWindow *w)
{
    if (m_windows.contains(w)) {
        return;
    }

    redirect(w);
    setShader(w, m_shader.get());
    m_windows.insert(w);
}

void SaturationEffect::slotWindowDeleted(EffectWindow *w)
{
    if (auto it = m_windows.find(w); it != m_windows.end()) {
        m_windows.erase(it);
    }
}

bool SaturationEffect::isActive() const
{
    return !m_windows.empty();
}

void SaturationEffect::reconfigure(ReconfigureFlags flags)
{
    if (flags != Effect::ReconfigureAll) {
        return;
    }

    SaturationSettings::self()->read();
    const auto newSaturation = std::clamp<float>(SaturationSettings::saturation(), 0.0f, 4.0f);
    if (qFuzzyCompare(m_saturation, newSaturation)) {
        return;
    }

    m_saturation = newSaturation;

    disconnect(effects, &EffectsHandler::windowDeleted, this, &SaturationEffect::slotWindowDeleted);
    disconnect(effects, &EffectsHandler::windowAdded, this, &SaturationEffect::applyToWindow);

    for (EffectWindow *w : m_windows) {
        unredirect(w);
    }
    m_windows.clear();

    loadData();
}

int SaturationEffect::requestedEffectChainPosition() const
{
    return 98;
}

} // namespace KWin

#include "moc_saturation.cpp"
