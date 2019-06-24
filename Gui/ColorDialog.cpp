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

#include "ColorDialog.h"
#include <QVBoxLayout>
#include <QTabWidget>

ColorDialog::ColorDialog(QWidget *parent)
    : QColorDialog(parent)
    , triangle(NULL)
    , block(false)
{
    init();
}

ColorDialog::ColorDialog(const QColor &initial, QWidget *parent)
    : QColorDialog(initial, parent)
    , triangle(NULL)
    , block(false)
{
    init();
}

ColorDialog::~ColorDialog()
{

}

// setup dialog
void ColorDialog::init()
{
    // never use native dialog
    setOption(QColorDialog::DontUseNativeDialog);

    // get qcolordialog layout
    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>( layout() );
    if (!mainLayout) { return; }
    QHBoxLayout* topLayout = qobject_cast<QHBoxLayout*>( mainLayout->itemAt(0)->layout() );
    if (!topLayout) { return; }
    QVBoxLayout* leftLayout = qobject_cast<QVBoxLayout*>( topLayout->takeAt(0)->layout() );
    if (!leftLayout) { return; }

    // create a container for left layout (basic)
    QWidget *basic = new QWidget(this);
    leftLayout->setParent(NULL); // remove from existing layout
    basic->setLayout(leftLayout);

    // add qtcolortriangle
    triangle = new QtColorTriangle(this);
    triangle->setColor( currentColor() );

    // add tab widget
    QTabWidget *tab = new QTabWidget(this);
    tab->setMinimumSize( QSize(250, 250) );
    tab->addTab( triangle, QObject::tr("Triangle") );
    tab->addTab( basic, QObject::tr("Colors") );
    topLayout->insertWidget(0, tab); // add tab to existing layout

    // connect qcolordialog and widget(s)
    QObject::connect( triangle, SIGNAL( colorChanged(QColor) ),
                      this, SLOT( updateCurrentColor(QColor) ) );
    QObject::connect( this, SIGNAL( currentColorChanged(QColor) ),
                      this, SLOT( handleCurrentColorChanged(QColor) ) );
}

// set qcolordialog color from widget(s)
void ColorDialog::updateCurrentColor(const QColor &color)
{
    block = true; // block handleCurrentColorChanged
    setCurrentColor(color);
}

// set widget(s) color from qcolordialog
void ColorDialog::handleCurrentColorChanged(const QColor &color)
{
    if (block) { // ignore color
        block = false;
        return;
    }

    // qtcolortriangle
    triangle->blockSignals(true);
    triangle->setColor(color);
    triangle->blockSignals(false);
}
