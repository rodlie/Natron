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

#ifndef Gui_MidiLearnDialog_h
#define Gui_MidiLearnDialog_h

#include "Global/Macros.h"

CLANG_DIAG_OFF(deprecated)
CLANG_DIAG_OFF(uninitialized)
#include <QDialog>
#include <QPlainTextEdit>
CLANG_DIAG_ON(deprecated)
CLANG_DIAG_ON(uninitialized)

#include "Engine/MidiHandler.h"
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
                    MidiHandler *handler = NULL,
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
    QPlainTextEdit *_log;
};

NATRON_NAMESPACE_EXIT

#endif // Gui_MidiLearnDialog_h
