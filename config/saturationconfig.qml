/*
    SPDX-FileCopyrightText: 2026 Garward
    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.kwin.effects as KWinEffects

ColumnLayout {
    id: root
    spacing: Kirigami.Units.smallSpacing

    property alias cfg_Saturation: saturationSlider.value

    Kirigami.FormLayout {
        Layout.fillWidth: true

        RowLayout {
            Kirigami.FormData.label: i18n("Saturation:")
            spacing: Kirigami.Units.smallSpacing

            Slider {
                id: saturationSlider
                from: 0.0
                to: 4.0
                stepSize: 0.1
                Layout.fillWidth: true

                onMoved: {
                    saturationSpinBox.value = value
                }
            }

            SpinBox {
                id: saturationSpinBox
                from: 0
                to: 400
                stepSize: 10
                value: saturationSlider.value * 100

                property int decimals: 0

                textFromValue: function(value, locale) {
                    return value + "%"
                }

                valueFromText: function(text, locale) {
                    return parseInt(text)
                }

                onValueModified: {
                    saturationSlider.value = value / 100.0
                }
            }
        }

        Label {
            text: i18n("100% = Normal, 150% = 50% boost, 200% = Double saturation")
            font: Kirigami.Theme.smallFont
            opacity: 0.7
            Layout.fillWidth: true
            wrapMode: Text.Wrap
        }
    }
}
