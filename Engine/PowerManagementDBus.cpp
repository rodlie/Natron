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

#define SCREENSAVER_SERVICE "org.freedesktop.ScreenSaver"
#define SCREENSAVER_PATH "/ScreenSaver"
#define SCREENSAVER_FULL_PATH "/org/freedesktop/ScreenSaver"

#define CONSOLEKIT_SERVICE "org.freedesktop.ConsoleKit"
#define CONSOLEKIT_PATH "/org/freedesktop/ConsoleKit/Manager"
#define CONSOLEKIT_MANAGER "org.freedesktop.ConsoleKit.Manager"

#define LOGIND_SERVICE "org.freedesktop.login1"
#define LOGIND_PATH "/org/freedesktop/login1"
#define LOGIND_MANAGER "org.freedesktop.login1.Manager"

NATRON_NAMESPACE_ENTER

PowerManagementDBus::PowerManagementDBus(QObject *parent):
    QObject(parent)
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
    releaseSuspendLock();
}

bool
PowerManagementDBus::setScreenSaverDisabled(bool disabled)
{
    qDebug() << "setScreenSaverDisabled (DBus)" << disabled;
    Q_UNUSED(disabled)
    return false;
}

bool
PowerManagementDBus::setSystemSleepDisabled(bool disabled)
{
    qDebug() << "setSystemSleepDisabled (DBus)" << disabled;
    if (disabled) {
        return registerSuspendLock();
    } else {
        releaseSuspendLock();
    }
    return false;
}

bool
PowerManagementDBus::isServiceAvailable(const QString &service,
                                        const QString &path,
                                        const QString &interface,
                                        bool session)
{
    if (!session) {
        // check if we have access to the system dbus
        // iface errors don't indicate if the issue is missing (system) dbus
        // so it's best to check for dbus before setting up iface
        QDBusConnection system = QDBusConnection::systemBus();
        if ( !system.isConnected() ) {
            qWarning() << system.lastError();
            return false;
        }
    }
    QDBusInterface iface(service,
                         path,
                         interface,
                         session ? QDBusConnection::sessionBus() : QDBusConnection::systemBus() );
    qDebug() << "is DBus service available?" << service << path << interface << iface.isValid();
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
PowerManagementDBus::hasScreenSaver()
{
    return isServiceAvailable(QString::fromUtf8(SCREENSAVER_SERVICE),
                              QString::fromUtf8(SCREENSAVER_PATH),
                              QString::fromUtf8(SCREENSAVER_SERVICE),
                              true);
}

bool
PowerManagementDBus::registerSuspendLock()
{
    if (_suspendLock) {
        // we already have a suspend lock
        return true;
    }
    if ( _service.isEmpty() ) {
        // DBus and/or service (Logind/ConsoleKit) not available
        return false;
    }
    qDebug() << "try to register suspend lock";
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
        qDebug() << "registered suspend lock success";
        _suspendLock.reset( new QDBusUnixFileDescriptor( reply.value() ) );
        return true;
    } else {
        qWarning() << reply.error();
    }
    return false;
}

void PowerManagementDBus::releaseSuspendLock()
{
    qDebug() << "release suspend lock";
   _suspendLock.reset(NULL);
}


NATRON_NAMESPACE_EXIT

NATRON_NAMESPACE_USING
#include "moc_PowerManagementDBus.cpp"
