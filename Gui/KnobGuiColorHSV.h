#ifndef Gui_KnobGuiColorHSV_h
#define Gui_KnobGuiColorHSV_h

#include <QWidget>
#include <QColor>

#include "Global/Macros.h"

#include "Gui/QtColorTriangle.h"
#include "Gui/ScaleSliderQWidget.h"
#include "Gui/SpinBox.h"

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
