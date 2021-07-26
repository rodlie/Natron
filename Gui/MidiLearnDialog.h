#ifndef MIDILEARNDIALOG_H
#define MIDILEARNDIALOG_H

#include "Global/Macros.h"

CLANG_DIAG_OFF(deprecated)
CLANG_DIAG_OFF(uninitialized)
#include <QDialog>
CLANG_DIAG_ON(deprecated)
CLANG_DIAG_ON(uninitialized)

#include "Gui/MIDIHandler.h"
#include "Gui/SpinBox.h"

NATRON_NAMESPACE_ENTER

class MidiLearnDialog
        : public QDialog
{
    Q_OBJECT

public:

    MidiLearnDialog(int key = 0,
                    double min = 0.0,
                    double max = 1.0,
                    MIDIHandler *handler = NULL,
                    QWidget* parent = NULL);

    void getOptions(int *key, double *min, double *max);

private Q_SLOTS:

    void onMidiInputChanged(int key, int value);
    void clearOptions(int key, double min, double max);
    void resetOptions();

private:

    SpinBox *_key;
    SpinBox *_min;
    SpinBox *_max;
};

NATRON_NAMESPACE_EXIT

#endif // MIDILEARNDIALOG_H
