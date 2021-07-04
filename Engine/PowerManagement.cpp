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

#include "PowerManagement.h"

CLANG_DIAG_OFF(deprecated)
#include <QDebug>
CLANG_DIAG_ON(deprecated)

#ifdef Q_OS_WIN
#include <windows.h>
#endif

NATRON_NAMESPACE_ENTER

PowerManagement::PowerManagement(QObject *parent)
    : QObject(parent)
#ifdef Q_OS_DARWIN
      , _mac(NULL)
#endif
    , _inhibitedScreenSaver(false)
    , _inhibitedSuspend(false)
{
#ifdef Q_OS_DARWIN
    _mac = new PowerManagementMac(this);
#endif
}

void
PowerManagement::inhibitScreenSaver(bool inhibit)
{
    if (_inhibitedScreenSaver == inhibit) {
        qDebug() << "inhibit screen saver request ignored";
        return;
    }
    qDebug() << "Inhibit screen saver" << inhibit;
#ifdef Q_OS_DARWIN
    _mac->setScreenSaverDisabled(inhibit);
#elif defined(Q_OS_WIN)
    if (!inhibit) {
        SetThreadExecutionState(ES_CONTINUOUS);
    } else {
        SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED);
    }
#else
    Q_UNUSED(inhibit)
    // TODO LINUX/BSD
#endif
    _inhibitedScreenSaver = inhibit;
}

void
PowerManagement::inhibitSuspend(bool inhibit)
{
    if (_inhibitedSuspend == inhibit) {
        qDebug() << "inhibit suspend request ignored";
        return;
    }
    qDebug() << "Inhibit computer sleep" << inhibit;
#ifdef Q_OS_DARWIN
    _mac->setSystemSleepDisabled(inhibit);
#elif defined(Q_OS_WIN)
    if (!inhibit) {
        SetThreadExecutionState(ES_CONTINUOUS);
    } else {
        SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED);
    }
#else
    Q_UNUSED(inhibit)
    // TODO LINUX/BSD
#endif
    _inhibitedSuspend = inhibit;
}

NATRON_NAMESPACE_EXIT

NATRON_NAMESPACE_USING
#include "moc_PowerManagement.cpp"
