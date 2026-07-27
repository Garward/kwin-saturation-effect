/*
    SPDX-FileCopyrightText: 2026 Garward
    SPDX-License-Identifier: GPL-2.0-or-later

    Saturation Boost - A KWin effect that increases color saturation
    Similar to NVIDIA Digital Vibrance or AMD Saturation settings
*/

#include "saturation.h"

#include "effect/effecthandler.h"
#include "opengl/glshader.h"

#include <KConfig>
#include <KConfigGroup>

static void ensureResources()
{
    Q_INIT_RESOURCE(saturation);
}

namespace KWin
{

namespace
{

template<typename Shader>
bool isShaderUsable(const std::unique_ptr<Shader> &shader)
{
    if (!shader) {
        return false;
    }
    if constexpr (requires(const Shader &candidate) { candidate.isValid(); }) {
        return shader->isValid();
    }
    return true;
}

}

SaturationEffect::SaturationEffect()
    : OffscreenEffect()
{
    KConfig config(QStringLiteral("kwinrc"));
    KConfigGroup group = config.group(QStringLiteral("Effect-saturation"));
    int saturationPercent = group.readEntry("Saturation", 150);
    m_saturation = std::clamp<float>(saturationPercent / 100.0f, 0.0f, 4.0f);
    m_mode = group.readEntry("Mode", 0);

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

    if (!isShaderUsable(m_shader)) {
        qWarning() << "SaturationEffect: Failed to load shader!";
        return;
    }

    updateShaderUniforms();

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
    Q_UNUSED(flags)

    KConfig config(QStringLiteral("kwinrc"));
    KConfigGroup group = config.group(QStringLiteral("Effect-saturation"));
    int saturationPercent = group.readEntry("Saturation", 150);
    m_saturation = std::clamp<float>(saturationPercent / 100.0f, 0.0f, 4.0f);
    m_mode = group.readEntry("Mode", 0);

    // Update shader uniforms
    updateShaderUniforms();

    // Force re-redirect all windows to apply the new shader uniform
    for (EffectWindow *w : m_windows) {
        unredirect(w);
    }
    m_windows.clear();

    for (EffectWindow *w : effects->stackingOrder()) {
        redirect(w);
        setShader(w, m_shader.get());
        m_windows.insert(w);
    }

    effects->addRepaintFull();
}

int SaturationEffect::requestedEffectChainPosition() const
{
    return 98;
}

void SaturationEffect::updateShaderUniforms()
{
    if (isShaderUsable(m_shader)) {
        ShaderBinder binder{m_shader.get()};
        m_shader->setUniform("saturationAmount", m_saturation);
        m_shader->setUniform("mode", m_mode);
    }
}

} // namespace KWin

#include "moc_saturation.cpp"
