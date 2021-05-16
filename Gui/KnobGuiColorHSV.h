/* ***** BEGIN LICENSE BLOCK *****
 * This file is part of Natron <https://natrongithub.github.io/>,
 * (C) 2018-2021 The Natron developers
 * (C) 2013-2018 INRIA and Alexandre Gauthier-Foichat
 *
 * Natron is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Natron is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Natron.  If not, see <http://www.gnu.org/licenses/gpl-2.0.html>
 * ***** END LICENSE BLOCK ***** */

#ifndef Gui_KnobGuiColorHSV_h
#define Gui_KnobGuiColorHSV_h

#include "Global/Macros.h"

CLANG_DIAG_OFF(deprecated)
CLANG_DIAG_OFF(uninitialized)
#include <QWidget>
#include <QColor>
#include "Gui/QtColorTriangle.h"
CLANG_DIAG_ON(deprecated)
CLANG_DIAG_ON(uninitialized)

#include "Gui/ScaleSliderQWidget.h"
#include "Gui/SpinBox.h"
#include "Engine/Lut.h"

NATRON_NAMESPACE_ENTER

class KnobGuiColorHSV : public QWidget
{
    Q_OBJECT
public:

    explicit KnobGuiColorHSV(QWidget *parent = NULL);

Q_SIGNALS:
    void colorChanged(const QColor &color);

public Q_SLOTS:
    const QColor getColor();
    void setColor(const QColor &color);
    void setRealColor(double r, double g, double b, double a);
    void setH(qreal h);
    void setS(qreal s);
    void setV(qreal v);

private:
    SpinBox *spinH;
    SpinBox *spinS;
    SpinBox *spinV;

    ScaleSliderQWidget *slideH;
    ScaleSliderQWidget *slideS;
    ScaleSliderQWidget *slideV;

    QtColorTriangle *triangle;

private Q_SLOTS:
    void handleColorChanged(const QColor &color,
                            bool doEmit = true);

    void handleColorHChanged(double value);
    void handleColorSChanged(double value);
    void handleColorVChanged(double value);

    void handleSliderHMoved(double value);
    void handleSliderSMoved(double value);
    void handleSliderVMoved(double value);
};

NATRON_NAMESPACE_EXIT

#endif // Gui_KnobGuiColorHSV_h
