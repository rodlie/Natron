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

#ifndef Gui_MidiHandler_h
#define Gui_MidiHandler_h

#include "Global/Macros.h"

CLANG_DIAG_OFF(deprecated)
CLANG_DIAG_OFF(uninitialized)
#include <QObject>
#include <QVector>
#include "Gui/RtMidi.h"
CLANG_DIAG_ON(deprecated)
CLANG_DIAG_ON(uninitialized)

NATRON_NAMESPACE_ENTER

class MidiKnob : public QObject
{
    Q_OBJECT

public:

    struct MidiKnobItem
    {
       int dim;
       int key;
       double min;
       double max;
    };

    explicit MidiKnob(QObject *parent = NULL);
    void setValue(int dim, int key, double min, double max);
    int getIndex(int dim, int key = 0);
    QVector<int> getKeyIndex(int key);
    QVector<MidiKnob::MidiKnobItem> getValue();
    void clearAll();

private:

    QVector<MidiKnob::MidiKnobItem> _knob;
};

class MidiHandler : public QObject
{
    Q_OBJECT

public:

    explicit MidiHandler(QObject *parent = NULL);
    ~MidiHandler();

    static double convertMidiValue(int value, double min, double max);
    static bool convertMidiValueBool(int value);
    static QVector<QString> getInputDevices();
    static int getInputDevicePort(const QString &device);
    bool isInputConnected();
    bool connectInput(int port);
    bool connectInput(const QString &device);
    bool disconnectInput();
    const QString connectedInputDevice();
    void checkSettings();

public Q_SLOTS:
    void setInputDevice(const std::string &device);

Q_SIGNALS:

    void midiInputChanged(int key, int value);

private:

    int _inputPort;
    RtMidiIn *_input;

    static void inputHandler(double deltatime,
                             std::vector< unsigned char > *message,
                             void *userData);
    void setInputValue(int key, int value);
};

NATRON_NAMESPACE_EXIT

#endif // Gui_MidiHandler_h
