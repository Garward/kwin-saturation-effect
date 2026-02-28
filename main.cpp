/*
    SPDX-FileCopyrightText: 2024
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "saturation.h"

namespace KWin
{

KWIN_EFFECT_FACTORY_SUPPORTED(SaturationEffect, "metadata.json", return SaturationEffect::supported();)

} // namespace KWin

#include "main.moc"
