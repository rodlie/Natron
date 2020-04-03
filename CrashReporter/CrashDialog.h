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

#ifndef Natron_CrashReporter_CrashDialog_h_
#define Natron_CrashReporter_CrashDialog_h_

#include "Global/Macros.h"

CLANG_DIAG_OFF(deprecated)
CLANG_DIAG_OFF(uninitialized)
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
CLANG_DIAG_ON(deprecated)
CLANG_DIAG_ON(uninitialized)

class QVBoxLayout;
class QLabel;
class QTextEdit;
class QGridLayout;
class QPushButton;
class QHBoxLayout;
class QFrame;
class PlaceHolderTextEdit;

class CrashDialog
    : public QDialog
{
    Q_OBJECT

public:
    enum UserChoice
    {
        eUserChoiceUpload,
        eUserChoiceSave,
        eUserChoiceIgnore
    };

    CrashDialog(const QString& filePath);

    virtual ~CrashDialog();

    QString getDescription() const;
    void setDescription(const QString &description);
    UserChoice getUserChoice() const;
    QString getGLrenderer() const;
    QString getGLversion() const;
    QString getGLvendor() const;
    QString getGLshader() const;
    QString getGLext() const;

    const QString& getOriginalDumpFilePath() const
    {
        return _filePath;
    }

public Q_SLOTS:

    void onSendClicked();
    void onDontSendClicked();

private:

    QString _filePath;
    QVBoxLayout* _mainLayout;
    QFrame* _mainFrame;
    QGridLayout* _gridLayout;
    QLabel* _iconLabel;
    QLabel* _infoLabel;
    PlaceHolderTextEdit* _descEdit;
    QFrame* _buttonsFrame;
    QHBoxLayout* _buttonsLayout;
    QPushButton* _sendButton;
    QPushButton* _dontSendButton;
    QPushButton* _pressedButton;
    QString _GLrenderer;
    QString _GLversion;
    QString _GLvendor;
    QString _GLshader;
    QString _GLext;
};

#endif // _CrashReporter_CrashDialog_h_
