#include "KnobGuiColorHSV.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

NATRON_NAMESPACE_ENTER

KnobGuiColorHSV::KnobGuiColorHSV(QWidget *parent) : QWidget(parent)
  , spinH(NULL)
  , spinS(NULL)
  , spinV(NULL)
  , slideH(NULL)
  , slideS(NULL)
  , slideV(NULL)
  /*, _sliderH(NULL)
  , _sliderS(NULL)
  , _sliderV(NULL)*/
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

    slideH = new QSlider(this);
    slideS = new QSlider(this);
    slideV = new QSlider(this);

    slideH->setRange(0,1000);
    slideS->setRange(0,1000);
    slideV->setRange(0,1000);

    slideH->setOrientation(Qt::Horizontal);
    slideS->setOrientation(Qt::Horizontal);
    slideV->setOrientation(Qt::Horizontal);

    slideH->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    slideS->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    slideV->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    spinH = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);
    spinS = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);;
    spinV = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);;

    spinH->decimals(3);
    spinS->decimals(3);
    spinV->decimals(3);

    spinH->setIncrement(0.01);
    spinS->setIncrement(0.01);
    spinV->setIncrement(0.01);

    spinH->setMaximum(1);
    spinH->setMinimum(0);

    spinS->setMaximum(1);
    spinS->setMinimum(0);
    spinV->setMaximum(1);
    spinV->setMinimum(0);

    //spinH->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    //spinS->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    //spinV->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    /*spinH->setMinimumWidth(70);
    spinH->setMaximumWidth(70);
    spinS->setMinimumWidth(70);
    spinS->setMaximumWidth(70);
    spinV->setMinimumWidth(70);
    spinV->setMaximumWidth(70);*/

    QLabel *labelH = new QLabel(this);
    QLabel *labelS = new QLabel(this);
    QLabel *labelV = new QLabel(this);

    labelH->setText( QString::fromUtf8("H") );
    labelS->setText( QString::fromUtf8("S") );
    labelV->setText( QString::fromUtf8("V") );

    /*labelH->setMinimumWidth(10);
    labelH->setMaximumWidth(10);
    labelS->setMinimumWidth(10);
    labelS->setMaximumWidth(10);
    labelV->setMinimumWidth(10);
    labelV->setMaximumWidth(10);*/

    hLayout->addWidget(labelH);
    hLayout->addWidget(slideH);
    hLayout->addWidget(spinH);
    sLayout->addWidget(labelS);
    sLayout->addWidget(slideS);
    sLayout->addWidget(spinS);
    vLayout->addWidget(labelV);
    vLayout->addWidget(slideV);
    vLayout->addWidget(spinV);

    /*_sliderH = new ScaleSliderQWidget(0,
                                      1,
                                      0,
                                      true,
                                      ScaleSliderQWidget::eDataTypeDouble,
                                      NULL,
                                      eScaleTypeLinear,
                                      this);
    _sliderS = new ScaleSliderQWidget(0,
                                      1,
                                      0,
                                      true,
                                      ScaleSliderQWidget::eDataTypeDouble,
                                      NULL,
                                      eScaleTypeLinear,
                                      this);
    _sliderV = new ScaleSliderQWidget(0,
                                      1,
                                      0,
                                      true,
                                      ScaleSliderQWidget::eDataTypeDouble,
                                      NULL,
                                      eScaleTypeLinear,
                                      this);

    hsvLayout->addWidget(_sliderH);
    hsvLayout->addWidget(_sliderS);
    hsvLayout->addWidget(_sliderV);*/
    hsvLayout->addWidget(hWidget);
    hsvLayout->addWidget(sWidget);
    hsvLayout->addWidget(vWidget);

    mainLayout->addWidget(hsvWidget);
    mainLayout->addWidget(triangle);


    //mainLayout->addSpacing(5);

    //triangle->setColor(QColor(Qt::black));
    //handleColorChanged(triangle->color());

    connect(triangle, SIGNAL(colorChanged(QColor)),
            this, SLOT(handleColorChanged(QColor)));

    connect(spinH, SIGNAL(valueChanged(double)),
            this, SLOT(handleColorHChanged(double)));
    connect(spinS, SIGNAL(valueChanged(double)),
            this, SLOT(handleColorSChanged(double)));
    connect(spinV, SIGNAL(valueChanged(double)),
            this, SLOT(handleColorVChanged(double)));

    connect(slideH, SIGNAL(sliderMoved(int)), this, SLOT(handleSliderHMoved(int)));
    connect(slideS, SIGNAL(sliderMoved(int)), this, SLOT(handleSliderSMoved(int)));
    connect(slideV, SIGNAL(sliderMoved(int)), this, SLOT(handleSliderVMoved(int)));
}

const QColor KnobGuiColorHSV::getColor()
{
    return triangle->color();
}

void KnobGuiColorHSV::setColor(const QColor &color)
{
    triangle->blockSignals(true);
    triangle->setColor(color);
    triangle->blockSignals(false);
    handleColorChanged(color, false);
}

void KnobGuiColorHSV::setH(qreal h)
{
    spinH->blockSignals(true);
    slideH->blockSignals(true);
    spinH->setValue(h);
    int slideValue = h*1000;
    slideH->setValue(slideValue);
    spinH->blockSignals(false);
    slideH->blockSignals(false);
}

void KnobGuiColorHSV::setS(qreal s)
{
    spinS->blockSignals(true);
    slideS->blockSignals(true);
    spinS->setValue(s);
    int slideValue = s*1000;
    slideS->setValue(slideValue);
    spinS->blockSignals(false);
    slideS->blockSignals(false);
}

void KnobGuiColorHSV::setV(qreal v)
{
    spinV->blockSignals(true);
    slideV->blockSignals(true);
    spinV->setValue(v);
    int slideValue = v*1000;
    slideV->setValue(slideValue);
    spinV->blockSignals(false);
    slideV->blockSignals(false);
}

void KnobGuiColorHSV::handleColorChanged(const QColor &color, bool doEmit)
{
    setH(color.toHsv().hueF());
    setS(color.toHsv().saturationF());
    setV(color.toHsv().valueF());
    if (doEmit) {  Q_EMIT colorChanged(color); }
}

void KnobGuiColorHSV::handleColorHChanged(double value)
{
    QColor color = triangle->color();
    color.setHsvF(value, spinS->value(), spinV->value());
    triangle->setColor(color);
}

void KnobGuiColorHSV::handleColorSChanged(double value)
{
    QColor color = triangle->color();
    color.setHsvF(spinH->value(), value, spinV->value());
    triangle->setColor(color);
}

void KnobGuiColorHSV::handleColorVChanged(double value)
{
    QColor color = triangle->color();
    color.setHsvF(spinH->value(), spinS->value(), value);
    triangle->setColor(color);
}

void KnobGuiColorHSV::handleSliderHMoved(int value)
{
    double newValue = (qreal)value/1000;
    spinH->setValue(newValue);
}

void KnobGuiColorHSV::handleSliderSMoved(int value)
{
    double newValue = (qreal)value/1000;
    spinS->setValue(newValue);
}

void KnobGuiColorHSV::handleSliderVMoved(int value)
{
    double newValue = (qreal)value/1000;
    spinV->setValue(newValue);
}

NATRON_NAMESPACE_EXIT

NATRON_NAMESPACE_USING
#include "moc_KnobGuiColorHSV.cpp"
