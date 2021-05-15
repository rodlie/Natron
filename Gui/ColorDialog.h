/* ***** BEGIN LICENSE BLOCK *****
 * This file is part of Natron <https://natrongithub.github.io/>,
 * (C) 2018-2020 The Natron developers
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

#ifndef Gui_ColorDialog_h
#define Gui_ColorDialog_h

#include "Global/Macros.h"

CLANG_DIAG_OFF(deprecated)
CLANG_DIAG_OFF(uninitialized)
#include <QtCore/QObject>
#include <QColorDialog>
#include "QtColorTriangle.h"
#include <QLineEdit>
CLANG_DIAG_ON(deprecated)
CLANG_DIAG_ON(uninitialized)

NATRON_NAMESPACE_ENTER

class ColorDialog : public QColorDialog
{
GCC_DIAG_SUGGEST_OVERRIDE_OFF
    Q_OBJECT
GCC_DIAG_SUGGEST_OVERRIDE_ON

public:
    explicit ColorDialog(QWidget* parent = NULL);
    explicit ColorDialog(const QColor& initial, QWidget* parent = NULL);
    virtual ~ColorDialog();

private:
    QtColorTriangle *triangle;
    QLineEdit *hex;
    bool blockTriangle;
    bool blockHex;
    void init();

private Q_SLOTS:
    void handleTriangleColorChanged(const QColor &color);
    void handleHexColorChanged(const QString &name);
    void handleCurrentColorChanged(const QColor &color);
};

NATRON_NAMESPACE_EXIT

#endif // Gui_ColorDialog_h
