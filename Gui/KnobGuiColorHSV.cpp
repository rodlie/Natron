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

#include "KnobGuiColorHSV.h"

CLANG_DIAG_OFF(deprecated)
CLANG_DIAG_OFF(uninitialized)
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
CLANG_DIAG_ON(deprecated)
CLANG_DIAG_ON(uninitialized)

#include "Gui/Label.h"

NATRON_NAMESPACE_ENTER

KnobGuiColorHSV::KnobGuiColorHSV(QWidget *parent) : QWidget(parent)
  , spinH(NULL)
  , spinS(NULL)
  , spinV(NULL)
  , slideH(NULL)
  , slideS(NULL)
  , slideV(NULL)
  , triangle(NULL)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->setSpacing(0);

    triangle = new QtColorTriangle(this);
    triangle->setMinimumSize(120, 120);
    triangle->setMaximumSize(120, 120);
    triangle->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QWidget *hsvWidget = new QWidget(this);
    QVBoxLayout *hsvLayout = new QVBoxLayout(hsvWidget);

    QWidget *hWidget = new QWidget(this);
    QHBoxLayout *hLayout = new QHBoxLayout(hWidget);
    QWidget *sWidget = new QWidget(this);
    QHBoxLayout *sLayout = new QHBoxLayout(sWidget);
    QWidget *vWidget = new QWidget(this);
    QHBoxLayout *vLayout = new QHBoxLayout(vWidget);

    hLayout->setContentsMargins(0, 0, 0, 0);
    sLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setContentsMargins(0, 0, 0, 0);

    slideH = new ScaleSliderQWidget(0.,
                                    1.,
                                    0.,
                                    false,
                                    ScaleSliderQWidget::eDataTypeDouble,
                                    NULL,
                                    eScaleTypeLinear,
                                    this);
    slideS = new ScaleSliderQWidget(0.,
                                    1.,
                                    0.,
                                    false,
                                    ScaleSliderQWidget::eDataTypeDouble,
                                    NULL,
                                    eScaleTypeLinear,
                                    this);
    slideV = new ScaleSliderQWidget(0.,
                                    1.,
                                    0.,
                                    false,
                                    ScaleSliderQWidget::eDataTypeDouble,
                                    NULL,
                                    eScaleTypeLinear,
                                    this);

    slideH->setMinimumAndMaximum(0., 1.);
    slideS->setMinimumAndMaximum(0., 1.);
    slideV->setMinimumAndMaximum(0., 1.);

    slideH->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    slideS->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    slideV->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    spinH = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);
    spinS = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);
    spinV = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);

    spinH->decimals(3);
    spinS->decimals(3);
    spinV->decimals(3);

    spinH->setIncrement(0.01);
    spinS->setIncrement(0.01);
    spinV->setIncrement(0.01);

    //spinH->setMaximum(1.);
    spinH->setMinimum(0.);
    //spinS->setMaximum(1.);
    spinS->setMinimum(0.);
    //spinV->setMaximum(1.);
    spinV->setMinimum(0.);

    Label *labelH = new Label(QString::fromUtf8("H"), this);
    Label *labelS = new Label(QString::fromUtf8("S"), this);
    Label *labelV = new Label(QString::fromUtf8("V"), this);

    hLayout->addWidget(labelH);
    hLayout->addWidget(spinH);
    hLayout->addWidget(slideH);

    sLayout->addWidget(labelS);
    sLayout->addWidget(spinS);
    sLayout->addWidget(slideS);

    vLayout->addWidget(labelV);
    vLayout->addWidget(spinV);
    vLayout->addWidget(slideV);

    hsvLayout->addWidget(hWidget);
    hsvLayout->addWidget(sWidget);
    hsvLayout->addWidget(vWidget);

    mainLayout->addWidget(triangle);
    mainLayout->addWidget(hsvWidget);

    QObject::connect( triangle, SIGNAL( colorChanged(QColor) ),
                      this, SLOT( handleColorChanged(QColor) ) );

    QObject::connect( spinH, SIGNAL( valueChanged(double) ),
                      this, SLOT( handleColorHChanged(double) ) );
    QObject::connect( spinS, SIGNAL( valueChanged(double) ),
                      this, SLOT( handleColorSChanged(double) ) );
    QObject::connect( spinV, SIGNAL( valueChanged(double) ),
                      this, SLOT( handleColorVChanged(double) ) );

    QObject::connect( slideH, SIGNAL( positionChanged(double) ),
                      this, SLOT( handleSliderHMoved(double) ) );
    QObject::connect( slideS, SIGNAL( positionChanged(double) ),
                      this, SLOT( handleSliderSMoved(double) ) );
    QObject::connect( slideV, SIGNAL( positionChanged(double) ),
                      this, SLOT( handleSliderVMoved(double) ) );
}

const QColor
KnobGuiColorHSV::getColor()
{
    return triangle->color();
}

void
KnobGuiColorHSV::setColor(const QColor &color)
{
    qDebug() << "setColor" << color;
    triangle->blockSignals(true);
    triangle->setColor(color);
    triangle->blockSignals(false);
    handleColorChanged(color, false);
}

// testing, ignore for now
void KnobGuiColorHSV::setRealColor(double r, double g, double b, double a)
{
    Q_UNUSED(a)
    qDebug() << "set real color" << r << g << b << a;
    float h = 0.;
    float s = 0.;
    float v = 0.;
    Color::rgb_to_hsv(r, g, b, &h, &s, &v);
    qDebug() << "hsv value?" << h << s << v;
}

void
KnobGuiColorHSV::setH(qreal h)
{
    double value = h;
    if (value < 0.) {
        value = 0.;
    }
    spinH->blockSignals(true);
    slideH->blockSignals(true);
    spinH->setValue(value);
    slideH->seekScalePosition(value);
    spinH->blockSignals(false);
    slideH->blockSignals(false);
}

void
KnobGuiColorHSV::setS(qreal s)
{
    double value = s;
    if (value < 0.) {
        value = 0.;
    }
    spinS->blockSignals(true);
    slideS->blockSignals(true);
    spinS->setValue(value);
    slideS->seekScalePosition(value);
    spinS->blockSignals(false);
    slideS->blockSignals(false);
}

void
KnobGuiColorHSV::setV(qreal v)
{
    double value = v;
    if (value < 0.) {
        value = 0.;
    }
    spinV->blockSignals(true);
    slideV->blockSignals(true);
    spinV->setValue(value);
    slideV->seekScalePosition(value);
    spinV->blockSignals(false);
    slideV->blockSignals(false);
}

void
KnobGuiColorHSV::handleColorChanged(const QColor &color, bool doEmit)
{
    qDebug() << "handle color changed" << color << color.toHsv().hueF() << color.toHsv().saturationF() << color.toHsv().valueF();
    setH( color.toHsv().hueF() );
    setS( color.toHsv().saturationF() );
    setV( color.toHsv().valueF() );
    if (doEmit) {
        Q_EMIT colorChanged(color);
    }
}

void
KnobGuiColorHSV::handleColorHChanged(double value)
{
    QColor color = triangle->color();
    color.setHsvF( value, spinS->value(), spinV->value() );
    triangle->setColor(color);
}

void
KnobGuiColorHSV::handleColorSChanged(double value)
{
    QColor color = triangle->color();
    color.setHsvF( spinH->value(), value, spinV->value() );
    triangle->setColor(color);
}

void
KnobGuiColorHSV::handleColorVChanged(double value)
{
    QColor color = triangle->color();
    color.setHsvF( spinH->value(), spinS->value(), value );
    triangle->setColor(color);
}

void
KnobGuiColorHSV::handleSliderHMoved(double value)
{
    spinH->setValue(value);
    handleColorHChanged(value);
}

void
KnobGuiColorHSV::handleSliderSMoved(double value)
{
    spinS->setValue(value);
    handleColorSChanged(value);
}

void
KnobGuiColorHSV::handleSliderVMoved(double value)
{
    spinV->setValue(value);
    handleColorSChanged(value);
}

NATRON_NAMESPACE_EXIT

NATRON_NAMESPACE_USING
#include "moc_KnobGuiColorHSV.cpp"
