/* ***** BEGIN LICENSE BLOCK *****
 * This file is part of Natron <https://natrongithub.github.io/>,
 * Copyright (C) 2013-2018 INRIA and Alexandre Gauthier-Foichat
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

#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include <QObject>
#include <QColorDialog>

// qtcolortriangle from Nokia Qt Solutions (LGPL2.1+/GPL2+/COMMERCIAL)
#include "QtColorTriangle.h"

class ColorDialog : public QColorDialog
{
    Q_OBJECT

public:
    explicit ColorDialog(QWidget* parent = NULL);
    explicit ColorDialog(const QColor& initial, QWidget* parent = NULL);
    virtual ~ColorDialog();

private:
    QtColorTriangle *triangle;
    void init();
    bool block;

private Q_SLOTS:
    void updateCurrentColor(const QColor &color);
    void handleCurrentColorChanged(const QColor &color);
};

#endif // COLORDIALOG_H
