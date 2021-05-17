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

#include "ColorDialog.h"

CLANG_DIAG_OFF(deprecated)
CLANG_DIAG_OFF(uninitialized)
#include <QVBoxLayout>
#include <QTabWidget>
#include <QLabel>
CLANG_DIAG_ON(deprecated)
CLANG_DIAG_ON(uninitialized)

NATRON_NAMESPACE_ENTER

ColorDialog::ColorDialog(QWidget *parent)
    : QColorDialog(parent)
    , triangle(0)
    , hex(0)
    , blockTriangle(false)
    , blockHex(false)
{
    init();
}

ColorDialog::ColorDialog(const QColor &initial, QWidget *parent)
    : QColorDialog(initial, parent)
    , triangle(0)
    , hex(0)
    , blockTriangle(false)
    , blockHex(false)
{
    init();
}

ColorDialog::~ColorDialog()
{
}

void
ColorDialog::init()
{
    // never use native dialog!
    setOption(QColorDialog::DontUseNativeDialog);

    // get native qcolordialog layout
    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>( layout() );
    if (!mainLayout) {
        return;
    }
    QHBoxLayout* topLayout = qobject_cast<QHBoxLayout*>( mainLayout->itemAt(0)->layout() );
    if (!topLayout) {
        return;
    }
    QVBoxLayout* leftLayout = qobject_cast<QVBoxLayout*>( topLayout->takeAt(0)->layout() );
    if (!leftLayout) {
        return;
    }

    // create a container for left layout (basic)
    QWidget *basic = new QWidget(this);
    leftLayout->setParent(NULL); // remove from existing layout
    basic->setLayout(leftLayout); // add left layout to basic

    // add color triangle
    triangle = new QtColorTriangle(this);
    triangle->setColor( currentColor() );

    // add hex editor
    QWidget *hexWidget = new QWidget(this);
    QHBoxLayout *hexLayout = new QHBoxLayout(hexWidget);
    hexLayout->setContentsMargins(0, 0, 0, 0);
    QLabel *hexLabel = new QLabel(this);
    hexLabel->setText( tr("Hex code :") );
    hex = new QLineEdit(this);
    hexLayout->addWidget(hexLabel);
    hexLayout->addWidget(hex);
    leftLayout->addWidget(hexWidget);

    // add tab widget
    QTabWidget *tab = new QTabWidget(this);
    tab->setMinimumSize( QSize(250, 250) );
    tab->addTab( triangle, QObject::tr("Triangle") );
    tab->addTab( basic, QObject::tr("Colors") );
    topLayout->insertWidget(0, tab); // add tab to existing layout

    QObject::connect( triangle, SIGNAL( colorChanged(QColor) ),
                      this, SLOT( handleTriangleColorChanged(QColor) ) );

    QObject::connect( this, SIGNAL( currentColorChanged(QColor) ),
                      this, SLOT( handleCurrentColorChanged(QColor) ) );

    QObject::connect( hex, SIGNAL( textChanged(QString) ),
                      this, SLOT( handleHexColorChanged(QString) ) );
}

void
ColorDialog::handleTriangleColorChanged(const QColor &color)
{
    blockTriangle = true; // block triangle color update
    setCurrentColor(color);
}

void
ColorDialog::handleHexColorChanged(const QString &name)
{
    if ( name.isEmpty() || !QColor::isValidColor(name) ) {
        return;
    }
    blockHex = true; // block hex color update
    setCurrentColor( QColor(name) );
}

void
ColorDialog::handleCurrentColorChanged(const QColor &color)
{
    // triangle
    if (blockTriangle) { // ignore color since triangle triggered this
        blockTriangle = false;
    } else { // set triangle color
        triangle->blockSignals(true);
        triangle->setColor(color);
        triangle->blockSignals(false);
    }
    // hex
    if (blockHex) { // ignore color since hex triggered this
        blockHex = false;
    } else { // set hex color
        hex->blockSignals(true);
        hex->setText( color.name() );
        hex->blockSignals(false);
    }
}

NATRON_NAMESPACE_EXIT

NATRON_NAMESPACE_USING
#include "moc_ColorDialog.cpp"
