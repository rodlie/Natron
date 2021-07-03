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

#ifdef Q_OS_WIN
#include <windows.h>
#endif

NATRON_NAMESPACE_ENTER

PowerManagement::PowerManagement(QObject *parent)
    : QObject(parent)
#ifdef Q_OS_DARWIN
      , _mac(NULL)
#endif
{
#ifdef Q_OS_DARWIN
    _mac = new PowerManagementMac(this);
#endif
}

void
PowerManagement::inhibitScreenSaver(bool off)
{
#ifdef Q_OS_DARWIN
    _mac->setScreensaverDisabled(!off);
#endif
    if (off) {
#ifdef Q_OS_WIN
        SetThreadExecutionState(ES_CONTINUOUS);
#endif
    // TODO LINUX/BSD
    } else {
#if defined(Q_OS_WIN)
        SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED);
#endif
    // TODO LINUX/BSD
    }
}

void
PowerManagement::inhibitSuspend(bool off)
{
#ifdef Q_OS_DARWIN
    _mac->setSystemSleepDisabled(!off);
#elif defined(Q_OS_WIN)
    if (off) {
        SetThreadExecutionState(ES_CONTINUOUS);
    } else {
        SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED);
    }
#else
    Q_UNUSED(off)
    // TODO LINUX/BSD
#endif
}

NATRON_NAMESPACE_EXIT

NATRON_NAMESPACE_USING
#include "moc_PowerManagement.cpp"
