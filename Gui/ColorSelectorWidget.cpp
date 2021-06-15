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

#include "ColorSelectorWidget.h"

CLANG_DIAG_OFF(deprecated)
CLANG_DIAG_OFF(uninitialized)
#include <QHBoxLayout>
#include <QVBoxLayout>
CLANG_DIAG_ON(deprecated)
CLANG_DIAG_ON(uninitialized)

#include "Gui/Label.h"

NATRON_NAMESPACE_ENTER

ColorSelectorWidget::ColorSelectorWidget(QWidget *parent,
                                         int colorWheelSize) : QWidget(parent)
  , spinR(0)
  , spinG(0)
  , spinB(0)
  , spinH(0)
  , spinS(0)
  , spinV(0)
  , spinA(0)
  , slideR(0)
  , slideG(0)
  , slideB(0)
  , slideH(0)
  , slideS(0)
  , slideV(0)
  , slideA(0)
  , triangle(0)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->setSpacing(0);

    triangle = new QtColorTriangle(this);
    triangle->setColor( QColor::fromHsvF(0.0, 0.0, 0.0, 1.0) ); // position the triangle properly

    // static or dynamic triangle size?
    if (colorWheelSize > 0) {
        triangle->setMinimumSize(colorWheelSize, colorWheelSize);
        triangle->setMaximumSize(colorWheelSize, colorWheelSize);
        triangle->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    } else {
        triangle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

    QWidget *colorWidget = new QWidget(this);
    QVBoxLayout *colorLayout = new QVBoxLayout(colorWidget);

    QWidget *rWidget = new QWidget(this);
    QHBoxLayout *rLayout = new QHBoxLayout(rWidget);
    QWidget *gWidget = new QWidget(this);
    QHBoxLayout *gLayout = new QHBoxLayout(gWidget);
    QWidget *bWidget = new QWidget(this);
    QHBoxLayout *bLayout = new QHBoxLayout(bWidget);

    QWidget *hWidget = new QWidget(this);
    QHBoxLayout *hLayout = new QHBoxLayout(hWidget);
    QWidget *sWidget = new QWidget(this);
    QHBoxLayout *sLayout = new QHBoxLayout(sWidget);
    QWidget *vWidget = new QWidget(this);
    QHBoxLayout *vLayout = new QHBoxLayout(vWidget);

    QWidget *aWidget = new QWidget(this);
    QHBoxLayout *aLayout = new QHBoxLayout(aWidget);

    rLayout->setContentsMargins(0, 0, 0, 0);
    gLayout->setContentsMargins(0, 0, 0, 0);
    bLayout->setContentsMargins(0, 0, 0, 0);

    hLayout->setContentsMargins(0, 0, 0, 0);
    sLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setContentsMargins(0, 0, 0, 0);

    aLayout->setContentsMargins(0, 0, 0, 0);

    slideR = new ScaleSliderQWidget(0.,
                                    1.,
                                    0.,
                                    false,
                                    ScaleSliderQWidget::eDataTypeDouble,
                                    NULL,
                                    eScaleTypeLinear,
                                    this);
    slideG = new ScaleSliderQWidget(0.,
                                    1.,
                                    0.,
                                    false,
                                    ScaleSliderQWidget::eDataTypeDouble,
                                    NULL,
                                    eScaleTypeLinear,
                                    this);
    slideB = new ScaleSliderQWidget(0.,
                                    1.,
                                    0.,
                                    false,
                                    ScaleSliderQWidget::eDataTypeDouble,
                                    NULL,
                                    eScaleTypeLinear,
                                    this);

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

    slideA = new ScaleSliderQWidget(0.,
                                    1.,
                                    0.,
                                    false,
                                    ScaleSliderQWidget::eDataTypeDouble,
                                    NULL,
                                    eScaleTypeLinear,
                                    this);

    slideR->setMinimumAndMaximum(0., 1.);
    slideG->setMinimumAndMaximum(0., 1.);
    slideB->setMinimumAndMaximum(0., 1.);

    slideH->setMinimumAndMaximum(0., 1.);
    slideS->setMinimumAndMaximum(0., 1.);
    slideV->setMinimumAndMaximum(0., 1.);

    slideA->setMinimumAndMaximum(0., 1.);

    slideR->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    slideG->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    slideB->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    slideH->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    slideS->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    slideV->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    slideA->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    spinR = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);
    spinG = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);
    spinB = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);

    spinH = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);
    spinS = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);
    spinV = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);

    spinA = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);

    spinR->decimals(3);
    spinG->decimals(3);
    spinB->decimals(3);

    spinH->decimals(3);
    spinS->decimals(3);
    spinV->decimals(3);

    spinA->decimals(3);

    spinR->setIncrement(0.01);
    spinG->setIncrement(0.01);
    spinB->setIncrement(0.01);

    spinH->setIncrement(0.01);
    spinS->setIncrement(0.01);
    spinV->setIncrement(0.01);

    spinA->setIncrement(0.01);

    spinR->setMaximum(1.);
    spinR->setMinimum(0.);
    spinG->setMaximum(1.);
    spinG->setMinimum(0.);
    spinB->setMaximum(1.);
    spinB->setMinimum(0.);

    spinH->setMaximum(1.);
    spinH->setMinimum(0.);
    spinS->setMaximum(1.);
    spinS->setMinimum(0.);
    spinV->setMaximum(1.);
    spinV->setMinimum(0.);

    spinA->setMaximum(1.);
    spinA->setMinimum(0.);

    Label *labelR = new Label(QString::fromUtf8("R"), this);
    Label *labelG = new Label(QString::fromUtf8("G"), this);
    Label *labelB = new Label(QString::fromUtf8("B"), this);

    Label *labelH = new Label(QString::fromUtf8("H"), this);
    Label *labelS = new Label(QString::fromUtf8("S"), this);
    Label *labelV = new Label(QString::fromUtf8("V"), this);

    Label *labelA = new Label(QString::fromUtf8("A"), this);

    labelR->setMinimumWidth(10);
    labelG->setMinimumWidth(10);
    labelB->setMinimumWidth(10);

    labelH->setMinimumWidth(10);
    labelS->setMinimumWidth(10);
    labelV->setMinimumWidth(10);

    labelA->setMinimumWidth(10);

    rLayout->addWidget(labelR);
    rLayout->addWidget(spinR);
    rLayout->addWidget(slideR);

    gLayout->addWidget(labelG);
    gLayout->addWidget(spinG);
    gLayout->addWidget(slideG);

    bLayout->addWidget(labelB);
    bLayout->addWidget(spinB);
    bLayout->addWidget(slideB);

    hLayout->addWidget(labelH);
    hLayout->addWidget(spinH);
    hLayout->addWidget(slideH);

    sLayout->addWidget(labelS);
    sLayout->addWidget(spinS);
    sLayout->addWidget(slideS);

    vLayout->addWidget(labelV);
    vLayout->addWidget(spinV);
    vLayout->addWidget(slideV);

    aLayout->addWidget(labelA);
    aLayout->addWidget(spinA);
    aLayout->addWidget(slideA);

    colorLayout->addWidget(rWidget);
    colorLayout->addWidget(gWidget);
    colorLayout->addWidget(bWidget);

    colorLayout->addWidget(hWidget);
    colorLayout->addWidget(sWidget);
    colorLayout->addWidget(vWidget);

    colorLayout->addWidget(aWidget);

    mainLayout->addWidget(triangle);
    mainLayout->addWidget(colorWidget);

    QObject::connect( triangle, SIGNAL( colorChanged(QColor) ),
                      this, SLOT( handleColorChanged(QColor) ) );

    QObject::connect( spinR, SIGNAL( valueChanged(double) ),
                      this, SLOT( handleColorRChanged(double) ) );
    QObject::connect( spinG, SIGNAL( valueChanged(double) ),
                      this, SLOT( handleColorGChanged(double) ) );
    QObject::connect( spinB, SIGNAL( valueChanged(double) ),
                      this, SLOT( handleColorBChanged(double) ) );

    QObject::connect( spinH, SIGNAL( valueChanged(double) ),
                      this, SLOT( handleColorHChanged(double) ) );
    QObject::connect( spinS, SIGNAL( valueChanged(double) ),
                      this, SLOT( handleColorSChanged(double) ) );
    QObject::connect( spinV, SIGNAL( valueChanged(double) ),
                      this, SLOT( handleColorVChanged(double) ) );

    QObject::connect( spinA, SIGNAL( valueChanged(double) ),
                      this, SLOT( handleColorAChanged(double) ) );

    QObject::connect( slideR, SIGNAL( positionChanged(double) ),
                      this, SLOT( handleSliderRMoved(double) ) );
    QObject::connect( slideG, SIGNAL( positionChanged(double) ),
                      this, SLOT( handleSliderGMoved(double) ) );
    QObject::connect( slideB, SIGNAL( positionChanged(double) ),
                      this, SLOT( handleSliderBMoved(double) ) );

    QObject::connect( slideH, SIGNAL( positionChanged(double) ),
                      this, SLOT( handleSliderHMoved(double) ) );
    QObject::connect( slideS, SIGNAL( positionChanged(double) ),
                      this, SLOT( handleSliderSMoved(double) ) );
    QObject::connect( slideV, SIGNAL( positionChanged(double) ),
                      this, SLOT( handleSliderVMoved(double) ) );

    QObject::connect( slideA, SIGNAL( positionChanged(double) ),
                      this, SLOT( handleSliderAMoved(double) ) );
}

const QColor
ColorSelectorWidget::getColor()
{
    return triangle->color();
}

void
ColorSelectorWidget::setColor(const QColor &color)
{
    setA( color.alphaF() );
    triangle->blockSignals(true);
    triangle->setColor(color);
    triangle->blockSignals(false);
    handleColorChanged(color, false);
}

void
ColorSelectorWidget::setR(qreal r)
{

}

void
ColorSelectorWidget::setG(qreal g)
{

}

void
ColorSelectorWidget::setB(qreal b)
{

}

void
ColorSelectorWidget::setH(qreal h)
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
ColorSelectorWidget::setS(qreal s)
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
ColorSelectorWidget::setV(qreal v)
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
ColorSelectorWidget::setA(qreal a)
{
    double value = a;
    if (value < 0.) {
        value = 0.;
    }
    spinA->blockSignals(true);
    slideA->blockSignals(true);
    spinA->setValue(value);
    slideA->seekScalePosition(value);
    spinA->blockSignals(false);
    slideA->blockSignals(false);
}

void
ColorSelectorWidget::handleColorChanged(const QColor &color, bool doEmit)
{
    setH( color.toHsv().hueF() );
    setS( color.toHsv().saturationF() );
    setV( color.toHsv().valueF() );
    if (doEmit) {
        QColor newColor = color;
        newColor.setAlphaF( spinA->value() );
        Q_EMIT colorChanged(newColor);
    }
}

void
ColorSelectorWidget::handleColorRChanged(double value)
{

}

void
ColorSelectorWidget::handleColorGChanged(double value)
{

}

void
ColorSelectorWidget::handleColorBChanged(double value)
{

}

void
ColorSelectorWidget::handleColorHChanged(double value)
{
    QColor color = triangle->color();
    color.setHsvF( value, spinS->value(), spinV->value() );
    color.setAlphaF( spinA->value() );
    triangle->setColor(color);
}

void
ColorSelectorWidget::handleColorSChanged(double value)
{
    QColor color = triangle->color();
    color.setHsvF( spinH->value(), value, spinV->value() );
    color.setAlphaF( spinA->value() );
    triangle->setColor(color);
}

void
ColorSelectorWidget::handleColorVChanged(double value)
{
    QColor color = triangle->color();
    color.setHsvF( spinH->value(), spinS->value(), value );
    color.setAlphaF( spinA->value() );
    triangle->setColor(color);
}

void
ColorSelectorWidget::handleColorAChanged(double value)
{
    QColor color = triangle->color();
    color.setAlphaF(value);
    triangle->setColor(color);
}

void
ColorSelectorWidget::handleSliderRMoved(double value)
{

}

void
ColorSelectorWidget::handleSliderGMoved(double value)
{

}

void
ColorSelectorWidget::handleSliderBMoved(double value)
{

}

void
ColorSelectorWidget::handleSliderHMoved(double value)
{
    spinH->setValue(value);
    handleColorHChanged(value);
}

void
ColorSelectorWidget::handleSliderSMoved(double value)
{
    spinS->setValue(value);
    handleColorSChanged(value);
}

void
ColorSelectorWidget::handleSliderVMoved(double value)
{
    spinV->setValue(value);
    handleColorVChanged(value);
}

void
ColorSelectorWidget::handleSliderAMoved(double value)
{
    spinA->setValue(value);
    handleColorAChanged(value);
}

NATRON_NAMESPACE_EXIT

NATRON_NAMESPACE_USING
#include "moc_ColorSelectorWidget.cpp"
