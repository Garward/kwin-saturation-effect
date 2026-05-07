/*
    SPDX-FileCopyrightText: 2026 Garward
    SPDX-License-Identifier: GPL-2.0-or-later

    Saturation Boost - A KWin effect that increases color saturation
    Similar to NVIDIA Digital Vibrance or AMD Saturation settings
*/

#pragma once

#include <unordered_set>

#include "effect/offscreeneffect.h"
#include "opengl/glshadermanager.h"

namespace KWin
{

class SaturationEffect : public OffscreenEffect
{
    Q_OBJECT

public:
    explicit SaturationEffect();
    ~SaturationEffect() override;

    bool isActive() const override;
    void reconfigure(ReconfigureFlags flags) override;
    int requestedEffectChainPosition() const override;

    static bool supported();

public Q_SLOTS:
    void slotWindowDeleted(KWin::EffectWindow *w);

private Q_SLOTS:
    void applyToWindow(KWin::EffectWindow *w);

private:
    void loadData();
    void updateShaderUniforms();

    float m_saturation = 1.5f;
    int m_mode = 0;  // 0 = AMD (linear), 1 = NVIDIA (non-linear)

    std::unordered_set<KWin::EffectWindow *> m_windows;
    std::unique_ptr<GLShader> m_shader;
};

} // namespace KWin
