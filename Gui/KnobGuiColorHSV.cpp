#include "KnobGuiColorHSV.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

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
    mainLayout->setContentsMargins(5,5,5,5);
    mainLayout->setSpacing(0);

    triangle = new QtColorTriangle(this);
    triangle->setMinimumSize(120, 120);
    triangle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget *hsvWidget = new QWidget(this);
    QVBoxLayout *hsvLayout = new QVBoxLayout(hsvWidget);

    QWidget *hWidget = new QWidget(this);
    QHBoxLayout *hLayout = new QHBoxLayout(hWidget);
    QWidget *sWidget = new QWidget(this);
    QHBoxLayout *sLayout = new QHBoxLayout(sWidget);
    QWidget *vWidget = new QWidget(this);
    QHBoxLayout *vLayout = new QHBoxLayout(vWidget);

    hLayout->setContentsMargins(0,0,0,0);
    sLayout->setContentsMargins(0,0,0,0);
    vLayout->setContentsMargins(0,0,0,0);

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

    spinH->setMaximum(1.);
    spinH->setMinimum(0.);
    spinS->setMaximum(1.);
    spinS->setMinimum(0.);
    spinV->setMaximum(1.);
    spinV->setMinimum(0.);

    Label *labelH = new Label(QString::fromUtf8("H"), this);
    Label *labelS = new Label(QString::fromUtf8("S"), this);;
    Label *labelV = new Label(QString::fromUtf8("V"), this);;

    hLayout->addWidget(labelH);
    hLayout->addWidget(slideH);
    hLayout->addWidget(spinH);
    sLayout->addWidget(labelS);
    sLayout->addWidget(slideS);
    sLayout->addWidget(spinS);
    vLayout->addWidget(labelV);
    vLayout->addWidget(slideV);
    vLayout->addWidget(spinV);

    hsvLayout->addWidget(hWidget);
    hsvLayout->addWidget(sWidget);
    hsvLayout->addWidget(vWidget);

    mainLayout->addWidget(hsvWidget);
    mainLayout->addWidget(triangle);

    connect(triangle, SIGNAL(colorChanged(QColor)),
            this, SLOT(handleColorChanged(QColor)));

    connect(spinH, SIGNAL(valueChanged(double)),
            this, SLOT(handleColorHChanged(double)));
    connect(spinS, SIGNAL(valueChanged(double)),
            this, SLOT(handleColorSChanged(double)));
    connect(spinV, SIGNAL(valueChanged(double)),
            this, SLOT(handleColorVChanged(double)));

    connect(slideH, SIGNAL(positionChanged(double)),
            this, SLOT(handleSliderHMoved(double)));
    connect(slideS, SIGNAL(positionChanged(double)),
            this, SLOT(handleSliderSMoved(double)));
    connect(slideV, SIGNAL(positionChanged(double)),
            this, SLOT(handleSliderVMoved(double)));
}

const QColor KnobGuiColorHSV::getColor()
{
    return triangle->color();
}

void KnobGuiColorHSV::setColor(const QColor &color)
{
    qDebug() << "set color" << color;
    triangle->blockSignals(true);
    triangle->setColor(color);
    triangle->blockSignals(false);
    handleColorChanged(color, false);
}

void KnobGuiColorHSV::setH(qreal h)
{
    qDebug() << "set H" << h;
    double value = h;
    if (value < 0.) {
        value = 0.;
        qDebug() << "adjusted H to 0";
    }
    spinH->blockSignals(true);
    slideH->blockSignals(true);
    spinH->setValue(value);
    slideH->seekScalePosition(value);
    spinH->blockSignals(false);
    slideH->blockSignals(false);
}

void KnobGuiColorHSV::setS(qreal s)
{
    qDebug() << "set S" << s;
    double value = s;
    if (value < 0.) {
        value = 0.;
        qDebug() << "adjusted S to 0";
    }
    spinS->blockSignals(true);
    slideS->blockSignals(true);
    spinS->setValue(value);
    slideS->seekScalePosition(value);
    spinS->blockSignals(false);
    slideS->blockSignals(false);
}

void KnobGuiColorHSV::setV(qreal v)
{
    qDebug() << "set V" << v;
    double value = v;
    if (value < 0.) {
        value = 0.;
        qDebug() << "adjusted V to 0";
    }
    spinV->blockSignals(true);
    slideV->blockSignals(true);
    spinV->setValue(value);
    slideV->seekScalePosition(value);
    spinV->blockSignals(false);
    slideV->blockSignals(false);
}

void KnobGuiColorHSV::handleColorChanged(const QColor &color, bool doEmit)
{
    qDebug() << "handle color changed" << color << color.toHsv().hueF() << color.toHsv().saturationF() << color.toHsv().valueF();
    setH( color.toHsv().hueF() );
    setS( color.toHsv().saturationF() );
    setV( color.toHsv().valueF() );
    if (doEmit) {  Q_EMIT colorChanged(color); }
}

void KnobGuiColorHSV::handleColorHChanged(double value)
{
    qDebug() << "handle color H changed" << value;
    QColor color = triangle->color();
    color.setHsvF( value, spinS->value(), spinV->value() );
    triangle->setColor(color);
}

void KnobGuiColorHSV::handleColorSChanged(double value)
{
    qDebug() << "handle color S changed" << value;
    QColor color = triangle->color();
    color.setHsvF( spinH->value(), value, spinV->value() );
    triangle->setColor(color);
}

void KnobGuiColorHSV::handleColorVChanged(double value)
{
    qDebug() << "handle color V changed" << value;
    QColor color = triangle->color();
    color.setHsvF( spinH->value(), spinS->value(), value );
    triangle->setColor(color);
}

void KnobGuiColorHSV::handleSliderHMoved(double value)
{
    qDebug() << "handle slider H moved" << value;
    spinH->setValue(value);
    handleColorHChanged(value);
}

void KnobGuiColorHSV::handleSliderSMoved(double value)
{
    qDebug() << "handle slider S moved" << value;
    spinS->setValue(value);
    handleColorSChanged(value);
}

void KnobGuiColorHSV::handleSliderVMoved(double value)
{
    qDebug() << "handle slider V moved" << value;
    spinV->setValue(value);
    handleColorSChanged(value);
}

NATRON_NAMESPACE_EXIT

NATRON_NAMESPACE_USING
#include "moc_KnobGuiColorHSV.cpp"
