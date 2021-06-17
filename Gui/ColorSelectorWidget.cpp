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
#include "Engine/Lut.h"

NATRON_NAMESPACE_ENTER

ColorSelectorWidget::ColorSelectorWidget(QWidget *parent,
                                         int colorWheelSize)
  : QWidget(parent)
  , _spinR(0)
  , _spinG(0)
  , _spinB(0)
  , _spinH(0)
  , _spinS(0)
  , _spinV(0)
  , _spinA(0)
  , _slideR(0)
  , _slideG(0)
  , _slideB(0)
  /*, _slideH(0)
  , _slideS(0)
  , _slideV(0)*/
  , _slideA(0)
  , _triangle(0)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->setSpacing(0);

    _triangle = new QtColorTriangle(this);
    // position the triangle properly before usage
    _triangle->setColor( QColor::fromHsvF(0.0, 0.0, 0.0, 1.0) );

    _triangle->setMinimumSize(colorWheelSize, colorWheelSize);
    _triangle->setMaximumSize(colorWheelSize, colorWheelSize);
    _triangle->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);



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

    _slideR = new ScaleSliderQWidget(0.,
                                     1.,
                                     0.,
                                     false,
                                     ScaleSliderQWidget::eDataTypeDouble,
                                     NULL,
                                     eScaleTypeLinear,
                                     this);
    _slideG = new ScaleSliderQWidget(0.,
                                     1.,
                                     0.,
                                     false,
                                     ScaleSliderQWidget::eDataTypeDouble,
                                     NULL,
                                     eScaleTypeLinear,
                                     this);
    _slideB = new ScaleSliderQWidget(0.,
                                     1.,
                                     0.,
                                     false,
                                     ScaleSliderQWidget::eDataTypeDouble,
                                     NULL,
                                     eScaleTypeLinear,
                                     this);

    /*_slideH = new ScaleSliderQWidget(0.,
                                     1.,
                                     0.,
                                     false,
                                     ScaleSliderQWidget::eDataTypeDouble,
                                     NULL,
                                     eScaleTypeLinear,
                                     this);
    _slideS = new ScaleSliderQWidget(0.,
                                     1.,
                                     0.,
                                     false,
                                     ScaleSliderQWidget::eDataTypeDouble,
                                     NULL,
                                     eScaleTypeLinear,
                                     this);
    _slideV = new ScaleSliderQWidget(0.,
                                     1.,
                                     0.,
                                     false,
                                     ScaleSliderQWidget::eDataTypeDouble,
                                     NULL,
                                     eScaleTypeLinear,
                                     this);*/

    _slideA = new ScaleSliderQWidget(0.,
                                     1.,
                                     0.,
                                     false,
                                     ScaleSliderQWidget::eDataTypeDouble,
                                     NULL,
                                     eScaleTypeLinear,
                                     this);

    _slideR->setMinimumAndMaximum(0., 1.);
    _slideG->setMinimumAndMaximum(0., 1.);
    _slideB->setMinimumAndMaximum(0., 1.);

    /*_slideH->setMinimumAndMaximum(0., 1.);
    _slideS->setMinimumAndMaximum(0., 1.);
    _slideV->setMinimumAndMaximum(0., 1.);*/

    _slideA->setMinimumAndMaximum(0., 1.);

    _slideR->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _slideG->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _slideB->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    /*_slideH->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _slideS->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _slideV->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);*/

    _slideA->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    _slideR->setUseLineColor(true, QColor(200, 70, 70) );
    _slideG->setUseLineColor(true, QColor(86, 166, 66) );
    _slideB->setUseLineColor(true, QColor(83, 121, 180) );
    _slideA->setUseLineColor(true, QColor(215, 215, 215));

    _slideR->setUseSliderColor(true, Qt::white);
    _slideG->setUseSliderColor(true, Qt::white);
    _slideB->setUseSliderColor(true, Qt::white);
    _slideA->setUseSliderColor(true, Qt::white);

    _spinR = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);
    _spinG = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);
    _spinB = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);

    _spinH = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);
    _spinS = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);
    _spinV = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);

    _spinA = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);

    _spinR->decimals(3);
    _spinG->decimals(3);
    _spinB->decimals(3);

    _spinH->decimals(3);
    _spinS->decimals(3);
    _spinV->decimals(3);

    _spinA->decimals(3);

    _spinR->setIncrement(0.01);
    _spinG->setIncrement(0.01);
    _spinB->setIncrement(0.01);

    _spinH->setIncrement(0.01);
    _spinS->setIncrement(0.01);
    _spinV->setIncrement(0.01);

    _spinA->setIncrement(0.01);

    _spinR->setMaximum(1.);
    _spinR->setMinimum(0.);
    _spinG->setMaximum(1.);
    _spinG->setMinimum(0.);
    _spinB->setMaximum(1.);
    _spinB->setMinimum(0.);

    _spinH->setMaximum(1.);
    _spinH->setMinimum(0.);
    _spinS->setMaximum(1.);
    _spinS->setMinimum(0.);
    _spinV->setMaximum(1.);
    _spinV->setMinimum(0.);

    _spinA->setMaximum(1.);
    _spinA->setMinimum(0.);

    _spinR->setObjectName( QString::fromUtf8("ColorSelectorRed") );
    _spinG->setObjectName( QString::fromUtf8("ColorSelectorGreen") );
    _spinB->setObjectName( QString::fromUtf8("ColorSelectorBlue") );
    _spinA->setObjectName( QString::fromUtf8("ColorSelectorAlpha") );
    _spinH->setObjectName( QString::fromUtf8("ColorSelectorHue") );
    _spinS->setObjectName( QString::fromUtf8("ColorSelectorSat") );
    _spinV->setObjectName( QString::fromUtf8("ColorSelectorVal") );

    /*Label *labelR = new Label(QString::fromUtf8("R"), this);
    Label *labelG = new Label(QString::fromUtf8("G"), this);
    Label *labelB = new Label(QString::fromUtf8("B"), this);*/

    Label *labelH = new Label(QString::fromUtf8("H"), this);
    Label *labelS = new Label(QString::fromUtf8("S"), this);
    Label *labelV = new Label(QString::fromUtf8("V"), this);

    //Label *labelA = new Label(QString::fromUtf8("A"), this);

    /*labelR->setMinimumWidth(10);
    labelG->setMinimumWidth(10);
    labelB->setMinimumWidth(10);*/

    labelH->setMinimumWidth(10);
    labelS->setMinimumWidth(10);
    labelV->setMinimumWidth(10);

    //labelA->setMinimumWidth(10);

    //rLayout->addWidget(labelR);
    rLayout->addWidget(_spinR);
    rLayout->addWidget(_slideR);

    //gLayout->addWidget(labelG);
    gLayout->addWidget(_spinG);
    gLayout->addWidget(_slideG);

    //bLayout->addWidget(labelB);
    bLayout->addWidget(_spinB);
    bLayout->addWidget(_slideB);

    hLayout->addWidget(labelH);
    hLayout->addWidget(_spinH);
    //hLayout->addWidget(_slideH);

    sLayout->addWidget(labelS);
    sLayout->addWidget(_spinS);
    //sLayout->addWidget(_slideS);

    vLayout->addWidget(labelV);
    vLayout->addWidget(_spinV);
    //vLayout->addWidget(_slideV);

    //aLayout->addWidget(labelA);
    aLayout->addWidget(_spinA);
    aLayout->addWidget(_slideA);

    QWidget *rgbaWidget = new QWidget(this);
    QVBoxLayout *rgbaLayout = new QVBoxLayout(rgbaWidget);

    QWidget *hsvWidget = new QWidget(this);
    hsvWidget->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout *hsvLayout = new QHBoxLayout(hsvWidget);

    rgbaLayout->addWidget(rWidget);
    rgbaLayout->addWidget(gWidget);
    rgbaLayout->addWidget(bWidget);
    rgbaLayout->addWidget(aWidget);

    hsvLayout->addWidget(hWidget);
    hsvLayout->addWidget(sWidget);
    hsvLayout->addWidget(vWidget);

    QWidget *leftWidget = new QWidget(this);
    leftWidget->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);

    QWidget *rightWidget = new QWidget(this);
    rightWidget->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);

    leftLayout->addWidget(_triangle);

    rightLayout->addWidget(hsvWidget);
    rightLayout->addWidget(rgbaWidget);

    mainLayout->addWidget(leftWidget);
    mainLayout->addWidget(rightWidget);

    QObject::connect( _triangle, SIGNAL( colorChanged(QColor) ),
                      this, SLOT( handleTriangleColorChanged(QColor) ) );

    QObject::connect( _spinR, SIGNAL( valueChanged(double) ),
                      this, SLOT( manageColorRGBChanged(double) ) );
    QObject::connect( _spinG, SIGNAL( valueChanged(double) ),
                      this, SLOT( manageColorRGBChanged(double) ) );
    QObject::connect( _spinB, SIGNAL( valueChanged(double) ),
                      this, SLOT( manageColorRGBChanged(double) ) );
    QObject::connect( _spinH, SIGNAL( valueChanged(double) ),
                      this, SLOT( manageColorHSVChanged(double) ) );
    QObject::connect( _spinS, SIGNAL( valueChanged(double) ),
                      this, SLOT( manageColorHSVChanged(double) ) );
    QObject::connect( _spinV, SIGNAL( valueChanged(double) ),
                      this, SLOT( manageColorHSVChanged(double) ) );
    QObject::connect( _spinA, SIGNAL( valueChanged(double) ),
                      this, SLOT( manageColorAlphaChanged(double) ) );

    QObject::connect( _slideR, SIGNAL( positionChanged(double) ),
                      this, SLOT( handleSliderRMoved(double) ) );
    QObject::connect( _slideG, SIGNAL( positionChanged(double) ),
                      this, SLOT( handleSliderGMoved(double) ) );
    QObject::connect( _slideB, SIGNAL( positionChanged(double) ),
                      this, SLOT( handleSliderBMoved(double) ) );
    /*QObject::connect( _slideH, SIGNAL( positionChanged(double) ),
                      this, SLOT( handleSliderHMoved(double) ) );
    QObject::connect( _slideS, SIGNAL( positionChanged(double) ),
                      this, SLOT( handleSliderSMoved(double) ) );
    QObject::connect( _slideV, SIGNAL( positionChanged(double) ),
                      this, SLOT( handleSliderVMoved(double) ) );*/
    QObject::connect( _slideA, SIGNAL( positionChanged(double) ),
                      this, SLOT( handleSliderAMoved(double) ) );
}

void ColorSelectorWidget::getColor(float *r,
                                   float *g,
                                   float *b,
                                   float *a)
{
    *r = _spinR->value();
    *g = _spinG->value();
    *b = _spinB->value();
    *a = _spinA->value();
}

void
ColorSelectorWidget::setColor(float r,
                              float g,
                              float b,
                              float a)
{
    float h, s, v;
    Color::rgb_to_hsv(r, g, b, &h, &s, &v);
    setRedChannel(r);
    setGreenChannel(g);
    setBlueChannel(b);
    setHueChannel(h);
    setSaturationChannel(s);
    setValueChannel(v);
    setAlphaChannel(a);
    setTriangle(r, g, b, a);
}

void
ColorSelectorWidget::setRedChannel(float value)
{
    _spinR->blockSignals(true);
    _slideR->blockSignals(true);
    _spinR->setValue(value);
    _slideR->seekScalePosition(value);
    _spinR->blockSignals(false);
    _slideR->blockSignals(false);
}

void
ColorSelectorWidget::setGreenChannel(float value)
{
    _spinG->blockSignals(true);
    _slideG->blockSignals(true);
    _spinG->setValue(value);
    _slideG->seekScalePosition(value);
    _spinG->blockSignals(false);
    _slideG->blockSignals(false);
}

void
ColorSelectorWidget::setBlueChannel(float value)
{
    _spinB->blockSignals(true);
    _slideB->blockSignals(true);
    _spinB->setValue(value);
    _slideB->seekScalePosition(value);
    _spinB->blockSignals(false);
    _slideB->blockSignals(false);
}

void
ColorSelectorWidget::setHueChannel(float value)
{
    _spinH->blockSignals(true);
    //_slideH->blockSignals(true);
    _spinH->setValue(value);
    //_slideH->seekScalePosition(value);
    _spinH->blockSignals(false);
    //_slideH->blockSignals(false);
}

void
ColorSelectorWidget::setSaturationChannel(float value)
{
    _spinS->blockSignals(true);
    //_slideS->blockSignals(true);
    _spinS->setValue(value);
    //_slideS->seekScalePosition(value);
    _spinS->blockSignals(false);
    //_slideS->blockSignals(false);
}

void
ColorSelectorWidget::setValueChannel(float value)
{
    _spinV->blockSignals(true);
    //_slideV->blockSignals(true);
    _spinV->setValue(value);
    //_slideV->seekScalePosition(value);
    _spinV->blockSignals(false);
    //_slideV->blockSignals(false);
}

void
ColorSelectorWidget::setAlphaChannel(float value)
{
    _spinA->blockSignals(true);
    _slideA->blockSignals(true);
    _spinA->setValue(value);
    _slideA->seekScalePosition(value);
    _spinA->blockSignals(false);
    _slideA->blockSignals(false);
}

void ColorSelectorWidget::setTriangle(float r,
                                      float g,
                                      float b,
                                      float a)
{
    QColor color;
    color.setRgbF(Color::to_func_srgb(r),
                  Color::to_func_srgb(g),
                  Color::to_func_srgb(b),
                  a);
    _triangle->blockSignals(true);
    _triangle->setColor(color);
    _triangle->blockSignals(false);
}

void ColorSelectorWidget::announceColorChange()
{
    Q_EMIT colorChanged( _spinR->value(),
                         _spinG->value(),
                         _spinB->value(),
                         _spinA->value() );
}

void
ColorSelectorWidget::handleTriangleColorChanged(const QColor &color, bool announce)
{
    setRedChannel( Color::from_func_srgb( color.redF() ) );
    setGreenChannel( Color::from_func_srgb( color.greenF() ) );
    setBlueChannel( Color::from_func_srgb( color.blueF() ) );
    setHueChannel( Color::from_func_srgb( color.toHsv().hueF() ) );
    setSaturationChannel( Color::from_func_srgb( color.toHsv().saturationF() ) );
    setValueChannel( Color::from_func_srgb( color.toHsv().valueF() ) );
    if (announce) {
        announceColorChange();
    }
}

void ColorSelectorWidget::manageColorRGBChanged(double /*value*/, bool announce)
{
    float r = _spinR->value();
    float g = _spinG->value();
    float b = _spinB->value();
    float a = _spinA->value();
    float h, s, v;
    Color::rgb_to_hsv(r, g, b, &h, &s, &v);

    setHueChannel(h);
    setSaturationChannel(s);
    setValueChannel(v);
    setTriangle(r, g, b, a);

    if (announce) {
        announceColorChange();
    }
}

void ColorSelectorWidget::manageColorHSVChanged(double /*value*/, bool announce)
{
    float h = _spinH->value();
    float s = _spinS->value();
    float v = _spinV->value();
    float a = _spinA->value();
    float r, g, b;
    Color::hsv_to_rgb(h, s, v, &r, &g, &b);

    setRedChannel(r);
    setGreenChannel(g);
    setBlueChannel(g);
    setTriangle(r, g, b, a);

    if (announce) {
        announceColorChange();
    }
}

void ColorSelectorWidget::manageColorAlphaChanged(double /*value*/)
{
    // no widget depend on alpha, so just announce color change
    announceColorChange();
}

void
ColorSelectorWidget::handleSliderRMoved(double value)
{
    _spinR->setValue(value);
    manageColorRGBChanged(value);
}

void
ColorSelectorWidget::handleSliderGMoved(double value)
{
    _spinG->setValue(value);
    manageColorRGBChanged(value);
}

void
ColorSelectorWidget::handleSliderBMoved(double value)
{
    _spinB->setValue(value);
    manageColorRGBChanged(value);
}

void
ColorSelectorWidget::handleSliderHMoved(double value)
{
    _spinH->setValue(value);
    manageColorHSVChanged(value);
}

void
ColorSelectorWidget::handleSliderSMoved(double value)
{
    _spinS->setValue(value);
    manageColorHSVChanged(value);
}

void
ColorSelectorWidget::handleSliderVMoved(double value)
{
    _spinV->setValue(value);
    manageColorHSVChanged(value);
}

void
ColorSelectorWidget::handleSliderAMoved(double value)
{
    _spinA->setValue(value);
    manageColorAlphaChanged(value);
}

NATRON_NAMESPACE_EXIT

NATRON_NAMESPACE_USING
#include "moc_ColorSelectorWidget.cpp"
