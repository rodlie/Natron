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

#include "MidiHandler.h"

CLANG_DIAG_OFF(deprecated)
CLANG_DIAG_OFF(uninitialized)
#include <QDebug>
#include <QSettings>
CLANG_DIAG_ON(deprecated)
CLANG_DIAG_ON(uninitialized)

#define MIDI_MAX_VALUE 127

NATRON_NAMESPACE_ENTER

MidiKnob::MidiKnob(QObject *parent)
    : QObject(parent)
    , _knob(0)
{

}

void
MidiKnob::setValue(int dim,
                   int key,
                   double min,
                   double max)
{
    int index = getIndex(dim);
    if (index >= 0) {
        _knob[index].key = key;
        _knob[index].min = min;
        _knob[index].max = max;
    } else {
        MidiKnob::MidiKnobItem item;
        item.dim = dim;
        item.key = key;
        item.min = min;
        item.max = max;
        _knob.append(item);
    }
}

int
MidiKnob::getIndex(int dim, int key)
{
    for (int i = 0; i < _knob.size(); ++i) {
        if (_knob.at(i).dim == dim) {
            if (key > 0) {
                if (_knob.at(i).key == key) {
                    return i;
                }
            } else {
                return i;
            }
        }
    }
    return -1;
}

QVector<int>
MidiKnob::getKeyIndex(int key)
{
    QVector<int> result;
    for (int i = 0; i < _knob.size(); ++i) {
        if (_knob.at(i).key == key) {
            result.append(i);
        }
    }
    return result;
}

QVector<MidiKnob::MidiKnobItem>
MidiKnob::getValue()
{
    return _knob;
}

void
MidiKnob::clearAll()
{
    _knob.clear();
}

MidiHandler::MidiHandler(QObject *parent)
    : QObject(parent)
    , _inputPort(-1)
    , _input(NULL)
{
    try {
        _input = new RtMidiIn();
        _input->setCallback(&MidiHandler::inputHandler, (void*)this);
        _input->ignoreTypes(false, false, false);
    } catch (RtMidiError &error) {
        error.printMessage();
    }

    checkSettings();
}

MidiHandler::~MidiHandler()
{
    if ( _input->isPortOpen() ) {
        _input->closePort();
    }
    delete _input;
}

double
MidiHandler::convertMidiValue(int value,
                              double min,
                              double max)
{
    if (value < 0 || min >= max) {
        return 0.0;
    }

    double mVal = value;
    double mMin = 0;
    double mMax = MIDI_MAX_VALUE;
    double kMin = min;
    double kMax = max;
    double kVal= ( (mVal - mMin) / (mMax - mMin) ) * (kMax - kMin) + kMin;

    return kVal;
}

bool
MidiHandler::convertMidiValueBool(int value)
{
    if ( value >= (int)(MIDI_MAX_VALUE / 2) ) {
        return true;
    }
    return false;
}

QVector<QString>
MidiHandler::getInputDevices()
{
    QVector<QString> devices;

    RtMidiIn midi;
    unsigned int ports = 0;
    try {
        ports = midi.getPortCount();
    } catch (RtMidiError &error) {
        error.printMessage();
    }
    for (unsigned int port = 0; port < ports; ++port) {
        QString portName;
        try {
            portName = QString::fromStdString( midi.getPortName(port) );
        } catch (RtMidiError &error) {
            error.printMessage();
        }
        if ( portName.isEmpty() ) {
            continue;
        }
        devices.push_back(portName);
    }

    return devices;
}

int
MidiHandler::getInputDevicePort(const QString &device)
{
    QVector<QString> devices = getInputDevices();
    for (int i = 0; i < devices.size(); ++i) {
        if (devices.at(i) == device) {
            return i;
        }
    }
    return -1;
}

bool
MidiHandler::isInputConnected()
{
    return _input->isPortOpen();
}

bool
MidiHandler::connectInput(int port)
{
    qDebug() << "MIDI connect input" << port;
    unsigned int ports = 0;
    try {
        ports = _input->getPortCount();
    } catch (RtMidiError &error) {
        error.printMessage();
    }
    if (port < 0 || ports < 1) {
        return false;
    }
    if ( _input->isPortOpen() ) {
        _input->closePort();
    }

    try {
        _input->openPort( port,
                          QString::fromUtf8("Natron").toStdString() );
    }  catch (RtMidiError &error) {
        error.printMessage();
    }

    if ( _input->isPortOpen() ) {
        _inputPort = port;
        return true;
    }
    _inputPort = -1;
    return false;
}

bool
MidiHandler::connectInput(const QString &device)
{
    int port = getInputDevicePort(device);
    if (port >= 0) {
        return connectInput(port);
    }
    return false;
}

bool
MidiHandler::disconnectInput()
{
    if ( _input->isPortOpen() ) {
        _input->closePort();
    }
    return !_input->isPortOpen();
}

const
QString MidiHandler::connectedInputDevice()
{
    if (_inputPort >= 0 && _inputPort <= getInputDevices().size() ) {
        return getInputDevices()[_inputPort];
    }
    return QString();
}

void
MidiHandler::checkSettings()
{
    QSettings settings( QString::fromUtf8(NATRON_ORGANIZATION_NAME), QString::fromUtf8(NATRON_APPLICATION_NAME) );
    QString name = QString::fromUtf8("midiIn");
    if ( !settings.contains(name) ) {
        return;
    }
    QString value = settings.value(name).toString();
    if ( value.isEmpty() || value == QString::fromUtf8("none") ) {
        if ( isInputConnected() ) {
            _input->closePort();
        }
        return;
    }
    if ( value != connectedInputDevice() ) {
        int port = getInputDevicePort(value);
        if (port >= 0) {
            connectInput(port);
        }
    }
}

void
MidiHandler::setInputDevice(const std::string &device)
{
    if (device.empty() || device == "none") {
        disconnectInput();
    } else {
        connectInput( QString::fromStdString(device) );
    }
}

void
MidiHandler::inputHandler(double /*deltatime*/,
                          std::vector<unsigned char> *message,
                          void *userData)
{
    if (message->size() < 3) {
        return;
    }

    int key = (int)message->at(1);
    int value = (int)message->at(2);

    reinterpret_cast<MidiHandler*>(userData)->setInputValue(key, value);
}

void
MidiHandler::setInputValue(int key, int value)
{
    Q_EMIT midiInputChanged(key, value);
}

NATRON_NAMESPACE_EXIT

NATRON_NAMESPACE_USING
#include "moc_MidiHandler.cpp"
