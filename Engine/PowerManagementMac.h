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

#ifndef POWERMANAGEMENT_MAC_H
#define POWERMANAGEMENT_MAC_H

#include "Global/Macros.h"

CLANG_DIAG_OFF(deprecated)
#include <QObject>
CLANG_DIAG_ON(deprecated)

NATRON_NAMESPACE_ENTER

class PowerManagementMac: public QObject
{
    Q_OBJECT

public:

    explicit PowerManagementMac(QObject *parent = NULL);
    ~PowerManagementMac();

    void setSystemSleepDisabled(bool disabled);

private:

    struct PMD;
    PMD *_pmd;
};

NATRON_NAMESPACE_EXIT

#endif // POWERMANAGEMENT_MAC_H
