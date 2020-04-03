/* ***** BEGIN LICENSE BLOCK *****
 * This file is part of Natron <https://natrongithub.github.io/>,
 * Copyright (C) 2013-2018 INRIA and Alexandre Gauthier-Foichat
 * Copyright (C) 2018-2020 The Natron developers
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

#include "CrashDialog.h"

#include <algorithm>
#include <iostream>
#include <cassert>

#include <QTextStream>
#include <QFile>
#include <QFrame>
#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QApplication>
#include <QStyle>

#include <QtOpenGL/QGLWidget>
#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/gl.h>
#  include <GL/glext.h>
#endif

#define NATRON_FONT "Droid Sans"
#define NATRON_FONT_SIZE_DEFAULT 11

CrashDialog::CrashDialog(const QString &filePath)
    : QDialog(0, Qt::Dialog | Qt::WindowStaysOnTopHint)
    , _filePath(filePath)
    , _mainLayout(0)
    , _mainFrame(0)
    , _gridLayout(0)
    , _iconLabel(0)
    , _infoLabel(0)
    , _descEdit(0)
    , _buttonsFrame(0)
    , _buttonsLayout(0)
    , _sendButton(0)
    , _dontSendButton(0)
    , _pressedButton(0)
{
    QFile qss( QString::fromUtf8(":/Resources/Stylesheets/mainstyle.qss") );

    if ( qss.open(QIODevice::ReadOnly
                  | QIODevice::Text) ) {
        QTextStream in(&qss);
        QString content = QString::fromUtf8("QWidget { font-family: \"%1\"; font-size: %2px; }\n").arg( QString::fromUtf8(NATRON_FONT) ).arg( QString::number(NATRON_FONT_SIZE_DEFAULT) );
        content += in.readAll();
        setStyleSheet( content
                       .arg( QString::fromUtf8("rgb(243,149,0)") )   // %1: selection-color
                       .arg( QString::fromUtf8("rgb(50,50,50)") )   // %2: medium background
                       .arg( QString::fromUtf8("rgb(71,71,71)") )   // %3: soft background
                       .arg( QString::fromUtf8("rgb(38,38,38)") )   // %4: strong background
                       .arg( QString::fromUtf8("rgb(150,150,150)") )   // %5: text colour
                       .arg( QString::fromUtf8("rgb(86,117,156)") )   // %6: interpolated value color
                       .arg( QString::fromUtf8("rgb(21,97,248)") )   // %7: keyframe value color
                       .arg( QString::fromUtf8("rgb(200,200,200)") )    // %8: disabled editable text
                       .arg( QString::fromUtf8("rgb(180,200,100)") )    // %9: expression background color
                       .arg( QString::fromUtf8("rgb(150,150,50)") )   // %10: altered text color
                       .arg( QString::fromUtf8("rgb(255,195,120)") ) );   // %11: mouse over selection color
    }

    QPalette p = qApp->palette();
    QBrush selCol( QColor(243,149,0) );
    p.setBrush( QPalette::Link, selCol );
    p.setBrush( QPalette::LinkVisited, selCol );
    qApp->setPalette( p );

    setWindowTitle( tr("Problem Report for Natron ") + QString::fromUtf8(NATRON_VERSION_STRING) );
    setAttribute(Qt::WA_DeleteOnClose, false);
    setMinimumWidth(600);

    _mainLayout = new QVBoxLayout(this);

    _mainFrame = new QFrame(this);
    _mainFrame->setFrameShape(QFrame::Box);

    _gridLayout = new QGridLayout(_mainFrame);

    QPixmap pix( QString::fromUtf8(":Resources/Images/natronIcon256_linux.png") );
    if (std::max( pix.width(), pix.height() ) != 64) {
        pix = pix.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    _iconLabel = new QLabel(_mainFrame);
    _iconLabel->setPixmap(pix);
    _gridLayout->addWidget(_iconLabel, 0, 0, 1, 2, Qt::AlignHCenter | Qt::AlignVCenter);

    QString infoStr = tr("Natron quit unexpectedly. See details below for more information.\nSubmitting a crash report requires a valid GitHub account.");
    _infoLabel = new QLabel(Qt::convertFromPlainText(infoStr, Qt::WhiteSpaceNormal), _mainFrame);
    _infoLabel->setAlignment(Qt::AlignCenter);

    _gridLayout->addWidget(_infoLabel, 1, 0, 1, 2);

    QTextEdit* edit = new QTextEdit(_mainFrame);
    edit->setReadOnly(true);
    edit->setAcceptRichText(false);
    edit->setWordWrapMode(QTextOption::NoWrap);

    _descEdit = edit;
    _gridLayout->addWidget(_descEdit, 4, 1, 1, 1);

    _mainLayout->addWidget(_mainFrame);

    _buttonsFrame = new QFrame(this);
    _buttonsFrame->setFrameShape(QFrame::Box);
    _buttonsLayout = new QHBoxLayout(_buttonsFrame);

    _sendButton = new QPushButton(tr("Send report"), _buttonsFrame);
    _sendButton->setFocusPolicy(Qt::TabFocus);
    QObject::connect( _sendButton, SIGNAL(clicked(bool)), this, SLOT(onSendClicked()) );
    _buttonsLayout->addWidget(_sendButton);

    _dontSendButton = new QPushButton(tr("Don't send"), _buttonsFrame);
    _dontSendButton->setFocusPolicy(Qt::TabFocus);
    QObject::connect( _dontSendButton, SIGNAL(clicked(bool)), this, SLOT(onDontSendClicked()) );
    _buttonsLayout->addWidget(_dontSendButton);

    _mainLayout->addWidget(_buttonsFrame);

    _sendButton->setFocus();

    QGLWidget GLwidget;
    if ( GLwidget.isValid() ) {
        GLwidget.makeCurrent();
        _GLrenderer = QString::fromUtf8( (char *) glGetString(GL_RENDERER) );
        _GLversion = QString::fromUtf8( (char *) glGetString(GL_VERSION) );
        _GLvendor = QString::fromUtf8( (char *) glGetString(GL_VENDOR) );
        _GLshader = QString::fromUtf8( (char *) glGetString(GL_SHADING_LANGUAGE_VERSION) );
        _GLext = QString::fromUtf8( (char *) glGetString(GL_EXTENSIONS) );
    }
}

CrashDialog::~CrashDialog()
{
}

QString
CrashDialog::getDescription() const
{
    return _descEdit->toPlainText();
}

void CrashDialog::setDescription(const QString &description)
{
    _descEdit->setText(description);
}

CrashDialog::UserChoice
CrashDialog::getUserChoice() const
{
    if (_pressedButton == _sendButton) {
        return eUserChoiceUpload;
    } else {
        return eUserChoiceIgnore;
    }
}

QString CrashDialog::getGLrenderer() const
{
    return _GLrenderer;
}

QString CrashDialog::getGLversion() const
{
    return _GLversion;
}

QString CrashDialog::getGLvendor() const
{
    return _GLvendor;
}

QString CrashDialog::getGLshader() const
{
    return _GLshader;
}

QString CrashDialog::getGLext() const
{
    return _GLext;
}

void
CrashDialog::onSendClicked()
{
    _pressedButton = _sendButton;
    accept();
}

void
CrashDialog::onDontSendClicked()
{
    _pressedButton = _dontSendButton;
    reject();
}
