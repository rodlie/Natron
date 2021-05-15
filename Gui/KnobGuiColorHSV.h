#ifndef Gui_KnobGuiColorHSV_h
#define Gui_KnobGuiColorHSV_h

#include <QWidget>
#include <QColor>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSlider>

#include "Global/Macros.h"
//#include "Global/GlobalDefines.h"

#include "Gui/QtColorTriangle.h"
#include "Gui/ScaleSliderQWidget.h"
#include "Gui/SpinBox.h"
//#include "Gui/SpinBoxValidator.h"

NATRON_NAMESPACE_ENTER

class KnobGuiColorHSV : public QWidget
{
    Q_OBJECT
public:

    explicit KnobGuiColorHSV(QWidget *parent = NULL);

Q_SIGNALS:
    void colorChanged(const QColor &color);
    /*void colorRChanged(const QColor &color);
    void colorGChanged(const QColor &color);
    void colorBChanged(const QColor &color);*/

public Q_SLOTS:
    const QColor getColor();
    void setColor(const QColor &color);
    void setH(qreal h);
    void setS(qreal s);
    void setV(qreal v);

private:
    /*QSpinBox *spinR;
    QSpinBox *spinG;
    QSpinBox *spinB;

    QSlider *slideR;
    QSlider *slideG;
    QSlider *slideB;

    QSpinBox *spinC;
    QSpinBox *spinM;
    QSpinBox *spinY;
    QSpinBox *spinK;

    QSlider *slideC;
    QSlider *slideM;
    QSlider *slideY;
    QSlider *slideK;*/

    SpinBox *spinH;
    SpinBox *spinS;
    SpinBox *spinV;

    QSlider *slideH;
    QSlider *slideS;
    QSlider *slideV;

    /*ScaleSliderQWidget *_sliderH;
    ScaleSliderQWidget *_sliderS;
    ScaleSliderQWidget *_sliderV;*/

    QtColorTriangle *triangle;

private Q_SLOTS:
    void handleColorChanged(const QColor &color,
                            bool doEmit = true);

    /*void handleColorRChanged(int value);
    void handleColorGChanged(int value);
    void handleColorBChanged(int value);

    void handleColorCChanged(int value);
    void handleColorMChanged(int value);
    void handleColorYChanged(int value);
    void handleColorKChanged(int value);*/

    void handleColorHChanged(double value);
    void handleColorSChanged(double value);
    void handleColorVChanged(double value);

    void handleSliderHMoved(int value);
    void handleSliderSMoved(int value);
    void handleSliderVMoved(int value);
};

NATRON_NAMESPACE_EXIT

#endif // Gui_KnobGuiColorHSV_h
