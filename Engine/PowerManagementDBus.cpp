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

#include "PowerManagementDBus.h"

CLANG_DIAG_OFF(deprecated)
#include <QDebug>
#include <QDBusInterface>
#include <QDBusReply>
CLANG_DIAG_ON(deprecated)

#define CONSOLEKIT_SERVICE "org.freedesktop.ConsoleKit"
#define CONSOLEKIT_PATH "/org/freedesktop/ConsoleKit/Manager"
#define CONSOLEKIT_MANAGER "org.freedesktop.ConsoleKit.Manager"

#define LOGIND_SERVICE "org.freedesktop.login1"
#define LOGIND_PATH "/org/freedesktop/login1"
#define LOGIND_MANAGER "org.freedesktop.login1.Manager"

NATRON_NAMESPACE_ENTER

PowerManagementDBus::PowerManagementDBus(QObject *parent)
    : QObject(parent)
{
    if ( hasLogind() ) {
        _service = QString::fromUtf8(LOGIND_SERVICE);
        _path = QString::fromUtf8(LOGIND_PATH);
        _interface = QString::fromUtf8(LOGIND_MANAGER);
    } else if ( hasConsoleKit() ) {
        _service = QString::fromUtf8(CONSOLEKIT_SERVICE);
        _path = QString::fromUtf8(CONSOLEKIT_PATH);
        _interface = QString::fromUtf8(CONSOLEKIT_MANAGER);
    }
}

PowerManagementDBus::~PowerManagementDBus()
{
    releaseSleepLock();
}

bool
PowerManagementDBus::setSystemSleepDisabled(bool disabled)
{
    if (disabled) {
        return registerSleepLock();
    } else {
        releaseSleepLock();
    }
    return false;
}

bool
PowerManagementDBus::isServiceAvailable(const QString &service,
                                        const QString &path,
                                        const QString &interface,
                                        bool session)
{
    QDBusConnection dbus = session? QDBusConnection::sessionBus() : QDBusConnection::systemBus();
    if ( !dbus.isConnected() ) {
        qWarning() << dbus.lastError();
        return false;
    }
    QDBusInterface iface(service, path, interface, dbus);
    if ( iface.isValid() ) {
        return true;
    } else {
        qWarning() << iface.lastError();
    }
    return false;
}

bool
PowerManagementDBus::hasLogind()
{
    return isServiceAvailable( QString::fromUtf8(LOGIND_SERVICE),
                               QString::fromUtf8(LOGIND_PATH),
                               QString::fromUtf8(LOGIND_MANAGER) );
}

bool
PowerManagementDBus::hasConsoleKit()
{
    return isServiceAvailable( QString::fromUtf8(CONSOLEKIT_SERVICE),
                               QString::fromUtf8(CONSOLEKIT_PATH),
                               QString::fromUtf8(CONSOLEKIT_MANAGER) );
}

bool
PowerManagementDBus::registerSleepLock()
{
    if (_sleepLock) {
        // we already have a suspend lock, ignore
        return true;
    }
    if ( _service.isEmpty() ) {
        // service not found, nothing we can do
        return false;
    }
    QDBusInterface iface( _service,
                          _path,
                          _interface,
                          QDBusConnection::systemBus() );
    if ( !iface.isValid() ) {
        qWarning() << iface.lastError();
        return false;
    }
    QDBusReply<QDBusUnixFileDescriptor> reply;
    reply = iface.call( QString::fromUtf8("Inhibit"),
                        QString::fromUtf8("sleep"),
                        QString::fromUtf8("Natron"),
                        QString::fromUtf8("Rendering"),
                        QString::fromUtf8("block") );
    if ( reply.isValid() ) {
        _sleepLock.reset( new QDBusUnixFileDescriptor( reply.value() ) );
        return true;
    } else {
        qWarning() << reply.error();
    }
    return false;
}

void
PowerManagementDBus::releaseSleepLock()
{
    _sleepLock.reset(NULL);
}

NATRON_NAMESPACE_EXIT

NATRON_NAMESPACE_USING
#include "moc_PowerManagementDBus.cpp"
