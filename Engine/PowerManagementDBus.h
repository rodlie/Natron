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

#ifndef POWERMANAGEMENT_DBUS_H
#define POWERMANAGEMENT_DBUS_H

#include "Global/Macros.h"

CLANG_DIAG_OFF(deprecated)
#include <QObject>
#include <QDBusUnixFileDescriptor>
CLANG_DIAG_ON(deprecated)

NATRON_NAMESPACE_ENTER

class PowerManagementDBus : public QObject
{
    Q_OBJECT

public:

    explicit PowerManagementDBus(QObject *parent = NULL);
    ~PowerManagementDBus();

    bool setSystemSleepDisabled(bool disabled);

private:

    QScopedPointer<QDBusUnixFileDescriptor> _sleepLock;
    QString _service;
    QString _path;
    QString _interface;

    bool isServiceAvailable(const QString &service,
                            const QString &path,
                            const QString &interface,
                            bool session = false);

    bool hasLogind();
    bool hasConsoleKit();

    bool registerSleepLock();
    void releaseSleepLock();
};

NATRON_NAMESPACE_EXIT

#endif // POWERMANAGEMENT_DBUS_H
