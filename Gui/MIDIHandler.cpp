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

#include "MIDIHandler.h"

CLANG_DIAG_OFF(deprecated)
CLANG_DIAG_OFF(uninitialized)
#include <QDebug>
CLANG_DIAG_ON(deprecated)
CLANG_DIAG_ON(uninitialized)

NATRON_NAMESPACE_ENTER

MIDIHandler::MIDIHandler(QObject *parent)
    : QObject(parent)
    , _input(NULL)
{
    QVector<QString> devices = getInputDevices();
    qDebug() << "MIDI handler input init" << devices;
    _input = new RtMidiIn();
    _input->setCallback(&MIDIHandler::inputHandler, (void*)this);
    _input->ignoreTypes(false, false, false);
    if (devices.size() > 0) {
        // connect to first available device
        // just for TESTING
        connectInput(0);
    }
}

MIDIHandler::~MIDIHandler()
{
    qDebug() << "MIDI handler exit";
    if ( _input->isPortOpen() ) {
        _input->closePort();
    }
    delete _input;
}

QVector<QString>
MIDIHandler::getInputDevices()
{
    QVector<QString> devices;

    RtMidiIn midi;
    unsigned int ports = midi.getPortCount();
    for (unsigned int port = 0; port < ports; ++port) {
        QString portName = QString::fromStdString( midi.getPortName(port) );
        if ( portName.isEmpty() ) {
            continue;
        }
        devices.push_back(portName);
    }

    return devices;
}

int
MIDIHandler::getInputDevicePort(const QString &device)
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
MIDIHandler::isInputConnected()
{
    return _input->isPortOpen();
}

bool
MIDIHandler::connectInput(int port)
{
    qDebug() << "MIDI connect input" << port;
    unsigned int ports = _input->getPortCount();
    if (port < 0 || ports < 1) {
        return false;
    }
    if ( _input->isPortOpen() ) {
        _input->closePort();
    }

    _input->openPort(port);
    return _input->isPortOpen();
}

bool
MIDIHandler::connectInput(const QString &device)
{
    int port = getInputDevicePort(device);
    if (port >= 0) {
        return connectInput(port);
    }
    return false;
}

void
MIDIHandler::inputHandler(double deltatime,
                          std::vector<unsigned char> *message,
                          void *userData)
{
    if (message->size() < 3) {
        return;
    }

    int key = (int)message->at(1);
    int value = (int)message->at(2);

    qDebug() << "MIDI inputHandler" << key << value;

    Q_UNUSED(deltatime)
    Q_UNUSED(userData)
}

NATRON_NAMESPACE_EXIT

NATRON_NAMESPACE_USING
#include "moc_MIDIHandler.cpp"
