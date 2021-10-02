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

// ***** BEGIN PYTHON BLOCK *****
// from <https://docs.python.org/3/c-api/intro.html#include-files>:
// "Since Python may define some pre-processor definitions which affect the standard headers on some systems, you must include Python.h before any standard headers are included."
#include <Python.h>
// ***** END PYTHON BLOCK *****

#include "NodeSettingsPanel.h"

#include <vector>
#include <list>
#include <string>
#include <exception>
#include <fstream>
#include <stdexcept>

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtWidgets/QStyle>
#else
#include <QtGui/QStyle>
#endif

#include <QSettings>
#include <QTimer>

#include "Global/FStreamsSupport.h"

#include "Engine/EffectInstance.h"
#include "Engine/Knob.h" // KnobHolder
#include "Engine/Node.h"
#include "Engine/NodeSerialization.h"
#include "Engine/RotoLayer.h"
#include "Engine/Utils.h" // convertFromPlainText

#include "Gui/Button.h"
#include "Gui/Gui.h"
#include "Gui/GuiApplicationManager.h" // appPTR
#include "Gui/GuiDefines.h"
#include "Gui/KnobGui.h"
#include "Gui/Menu.h"
#include "Gui/MultiInstancePanel.h"
#include "Gui/NodeGraph.h"
#include "Gui/NodeGui.h"
#include "Gui/TrackerPanel.h"
#include "Gui/RotoPanel.h"

using std::make_pair;
NATRON_NAMESPACE_ENTER


NodeSettingsPanel::NodeSettingsPanel(const MultiInstancePanelPtr & multiPanel,
                                     Gui* gui,
                                     const NodeGuiPtr &NodeUi,
                                     QVBoxLayout* container,
                                     QWidget *parent)
    : DockablePanel(gui,
                    multiPanel.get() != NULL ? dynamic_cast<KnobHolder*>( multiPanel.get() ) : NodeUi->getNode()->getEffectInstance().get(),
                    container,
                    DockablePanel::eHeaderModeFullyFeatured,
                    false,
                    NodeUi->getUndoStack(),
                    QString::fromUtf8( NodeUi->getNode()->getLabel().c_str() ),
                    QString::fromUtf8( NodeUi->getNode()->getPluginDescription().c_str() ),
                    parent)
    , _nodeGUI(NodeUi)
    , _selected(false)
    , _settingsButton(0)
    , _multiPanel(multiPanel)
{
    if (multiPanel) {
        multiPanel->initializeKnobsPublic();
    }


    QObject::connect( this, SIGNAL(closeChanged(bool)), NodeUi.get(), SLOT(onSettingsPanelClosedChanged(bool)) );
    const QSize mediumBSize( TO_DPIX(NATRON_MEDIUM_BUTTON_SIZE), TO_DPIY(NATRON_MEDIUM_BUTTON_SIZE) );
    const QSize mediumIconSize( TO_DPIX(NATRON_MEDIUM_BUTTON_ICON_SIZE), TO_DPIY(NATRON_MEDIUM_BUTTON_ICON_SIZE) );
    QPixmap pixSettings;
    appPTR->getIcon(NATRON_PIXMAP_SETTINGS, TO_DPIX(NATRON_MEDIUM_BUTTON_ICON_SIZE), &pixSettings);
    _settingsButton = new Button( QIcon(pixSettings), QString(), getHeaderWidget() );
    _settingsButton->setFixedSize(mediumBSize);
    _settingsButton->setIconSize(mediumIconSize);
    _settingsButton->setToolTip( NATRON_NAMESPACE::convertFromPlainText(tr("Settings and presets."), NATRON_NAMESPACE::WhiteSpaceNormal) );
    _settingsButton->setFocusPolicy(Qt::NoFocus);
    QObject::connect( _settingsButton, SIGNAL(clicked()), this, SLOT(onSettingsButtonClicked()) );
    insertHeaderWidget(1, _settingsButton);

    QTimer::singleShot(100, this, SLOT(initMidiKnobs()));
}

NodeSettingsPanel::~NodeSettingsPanel()
{
    NodeGuiPtr node = getNode();

    if (node) {
        node->removeSettingsPanel();
    }
}

void
NodeSettingsPanel::setSelected(bool s)
{
    if (s != _selected) {
        _selected = s;
        style()->unpolish(this);
        style()->polish(this);
    }
}

void
NodeSettingsPanel::setPyPlugUIEnabled(bool enabled)
{
    DockablePanel::setPyPlugUIEnabled(enabled);
    _settingsButton->setEnabled(enabled);
}

void
NodeSettingsPanel::centerOnItem()
{
    getNode()->centerGraphOnIt();
}

bool
NodeSettingsPanel::hasMidiKnobs()
{
    const std::vector<KnobIPtr> &knobs = getInternalKnobs();
    int totalKnobs = 0;

    for (U32 i = 0; i < knobs.size(); ++i) {
        KnobGuiPtr knobgui = getKnobGui(knobs[i]);
        if (knobgui && knobgui.get()->getMidiKnob().size() > 0) {
            totalKnobs++;
        }
    }

    if (totalKnobs > 0) {
        return true;
    }

    return false;
}

int
NodeSettingsPanel::loadMidiKnobs(const QString &filename)
{
    QSettings preset(filename, QSettings::IniFormat);

    int rKnobs = preset.value( QString::fromUtf8("knobs") ).toInt();
    if (rKnobs < 1) {
        return 0;
    }

    const std::vector<KnobIPtr> &knobs = getInternalKnobs();
    int totalPresets = 0;

    for (int i = 0; i < rKnobs; ++i) {
        preset.beginGroup( QString::fromUtf8("knob%1").arg(i) );
        if ( preset.contains( QString::fromUtf8("name") ) ) {
            QString midiName = preset.value( QString::fromUtf8("name") ).toString();
            int dims = preset.value( QString::fromUtf8("dim") ).toInt();
            if (dims < 1) {
                continue;
            }
            for (U32 y = 0; y < knobs.size(); ++y) {
                QString knobName = QString::fromStdString( knobs[y].get()->getName() );
                if ( knobName !=  midiName) {
                    continue;
                }
                KnobGuiPtr knobgui = getKnobGui(knobs[y]);
                if (!knobgui) {
                    continue;
                }
                for (int dim = 0; dim < dims; ++dim) {
                    int key = preset.value(QString::fromUtf8("dim%1/key").arg(dim), 0).toInt();
                    double min = preset.value(QString::fromUtf8("dim%1/min").arg(dim), 0.0).toDouble();
                    double max = preset.value(QString::fromUtf8("dim%1/max").arg(dim), 1.0).toDouble();
                    if (key < 1) {
                        continue;
                    }
                    qDebug() << "set/update midi knob from preset" << knobName << key << min << max;
                    knobgui.get()->setMidiKnob(dim, key, min, max);
                    totalPresets++;
                }
            }
        }
        preset.endGroup();
    }

    qDebug() << "load midi presets" << totalPresets << filename;

    return totalPresets;
}

int
NodeSettingsPanel::saveMidiKnobs(const QString &filename)
{
    if ( !hasMidiKnobs() ) {
        return 0;
    }

    QSettings preset(filename, QSettings::IniFormat);

    // we must clear existing values (QSettings will always append)
    preset.clear();

    const std::vector<KnobIPtr> &knobs = getInternalKnobs();
    int totalKnobs = 0;

    for (U32 i = 0; i < knobs.size(); ++i) {
        KnobGuiPtr knobgui = getKnobGui(knobs[i]);
        if (knobgui && knobgui.get()->getMidiKnob().size() > 0) {
            preset.beginGroup( QString::fromUtf8("knob%1").arg(totalKnobs) );
            preset.setValue( QString::fromUtf8("name"), QString::fromStdString(knobs[i].get()->getName() ) );
            preset.setValue( QString::fromUtf8("dim"), knobgui.get()->getMidiKnob().size() );
            for (int y = 0; y < knobgui.get()->getMidiKnob().size(); ++y) {
                preset.beginGroup( QString::fromUtf8("dim%1").arg(y) );
                preset.setValue(QString::fromUtf8("key"), knobgui.get()->getMidiKnob()[y].key);
                preset.setValue(QString::fromUtf8("min"), knobgui.get()->getMidiKnob()[y].min);
                preset.setValue(QString::fromUtf8("max"), knobgui.get()->getMidiKnob()[y].max);
                preset.endGroup();
            }
            preset.endGroup();
            totalKnobs++;
        }
    }

    if (totalKnobs > 0) {
        preset.setValue(QString::fromUtf8("knobs"), totalKnobs);
        preset.sync();
    }

    qDebug() << "save midi preset" << totalKnobs << filename;

    return totalKnobs;
}

void
NodeSettingsPanel::initMidiKnobs()
{
    QString pluginID;
    if ( getNode() ) {
        pluginID  = QString::fromStdString( getNode()->getNode()->getPluginID() );
    }
    if ( pluginID.isEmpty() ) {
        return;
    }

    QString presetFile = QString::fromUtf8("%1/.%2/midi-presets/%3.%4")
                         .arg( QDir::homePath() )
                         .arg( QString::fromUtf8(NATRON_APPLICATION_NAME) )
                         .arg(pluginID)
                         .arg( QString::fromUtf8(NATRON_MIDI_PRESETS_FILE_EXT) );

    if ( !QFile::exists(presetFile) ) {
        return;
    }

    loadMidiKnobs(presetFile);
}

RotoPanel*
NodeSettingsPanel::initializeRotoPanel()
{
    if ( getNode()->getNode()->isRotoPaintingNode() ) {
        return new RotoPanel(_nodeGUI.lock(), this);
    } else {
        return NULL;
    }
}

TrackerPanel*
NodeSettingsPanel::initializeTrackerPanel()
{
    if ( getNode()->getNode()->getEffectInstance()->isBuiltinTrackerNode() ) {
        return new TrackerPanel(_nodeGUI.lock(), this);
    } else {
        return NULL;
    }
}

QColor
NodeSettingsPanel::getCurrentColor() const
{
    return getNode()->getCurrentColor();
}

void
NodeSettingsPanel::initializeExtraGui(QVBoxLayout* layout)
{
    if ( _multiPanel && !_multiPanel->isGuiCreated() ) {
        _multiPanel->createMultiInstanceGui(layout);
    }
}

void
NodeSettingsPanel::onSettingsButtonClicked()
{
    Menu menu(this);
    //menu.setFont(QFont(appFont,appFontSize));
    NodeGuiPtr node = getNode();
    NodePtr master = node->getNode()->getMasterNode();
    QAction* importPresets = new QAction(tr("Import presets"), &menu);
    QObject::connect( importPresets, SIGNAL(triggered()), this, SLOT(onImportPresetsActionTriggered()) );
    QAction* exportAsPresets = new QAction(tr("Export as presets"), &menu);
    QObject::connect( exportAsPresets, SIGNAL(triggered()), this, SLOT(onExportPresetsActionTriggered()) );

    menu.addAction(importPresets);
    menu.addAction(exportAsPresets);
    menu.addSeparator();

    QAction* importMidiPresets = new QAction(tr("Import Midi presets"), &menu);
    QObject::connect( importMidiPresets, SIGNAL(triggered()), this, SLOT(onMidiImportPresetsActionTriggered()) );
    menu.addAction(importMidiPresets);

    if ( hasMidiKnobs() ) {
        QAction* exportMidiPresets = new QAction(tr("Export Midi presets"), &menu);
        QObject::connect( exportMidiPresets, SIGNAL(triggered()), this, SLOT(onMidiExportPresetsActionTriggered()) );
        menu.addAction(exportMidiPresets);

        QAction* setDefaultMidiPresets = new QAction(tr("Set default Midi presets"), &menu);
        QObject::connect( setDefaultMidiPresets, SIGNAL(triggered()), this, SLOT(onMidiSetDefaultPresetsActionTriggered()) );
        menu.addAction(setDefaultMidiPresets);
    }

    menu.addSeparator();

    QAction* manageUserParams = new QAction(tr("Manage user parameters..."), &menu);
    QObject::connect( manageUserParams, SIGNAL(triggered()), this, SLOT(onManageUserParametersActionTriggered()) );
    menu.addAction(manageUserParams);

    menu.addSeparator();


    QAction* setKeyOnAll = new QAction(tr("Set key on all parameters"), &menu);
    QObject::connect( setKeyOnAll, SIGNAL(triggered()), this, SLOT(setKeyOnAllParameters()) );
    QAction* removeAnimationOnAll = new QAction(tr("Remove animation on all parameters"), &menu);
    QObject::connect( removeAnimationOnAll, SIGNAL(triggered()), this, SLOT(removeAnimationOnAllParameters()) );
    menu.addAction(setKeyOnAll);
    menu.addAction(removeAnimationOnAll);

    if ( master || !node->getDagGui() || !node->getDagGui()->getGui() || node->getDagGui()->getGui()->isGUIFrozen() ) {
        importPresets->setEnabled(false);
        exportAsPresets->setEnabled(false);
        setKeyOnAll->setEnabled(false);
        removeAnimationOnAll->setEnabled(false);
    }

    menu.exec( _settingsButton->mapToGlobal( _settingsButton->pos() ) );
}

void
NodeSettingsPanel::onImportPresetsActionTriggered()
{
    std::vector<std::string> filters;

    filters.push_back(NATRON_PRESETS_FILE_EXT);
    std::string filename = getGui()->popOpenFileDialog(false, filters, getGui()->getLastLoadProjectDirectory().toStdString(), false);
    if ( filename.empty() ) {
        return;
    }


    FStreamsSupport::ifstream ifile;
    FStreamsSupport::open(&ifile, filename);
    if (!ifile) {
        Dialogs::errorDialog( tr("Presets").toStdString(), tr("Failed to open file: ").toStdString() + filename, false );

        return;
    }

    std::list<NodeSerializationPtr> nodeSerialization;
    try {
        int nNodes;
        boost::archive::xml_iarchive iArchive(ifile);
        iArchive >> boost::serialization::make_nvp("NodesCount", nNodes);
        for (int i = 0; i < nNodes; ++i) {
            NodeSerializationPtr node( new NodeSerialization() );
            iArchive >> boost::serialization::make_nvp("Node", *node);
            nodeSerialization.push_back(node);
        }
    } catch (const std::exception & e) {
        Dialogs::errorDialog( "Presets", e.what() );

        return;
    }

    NodeGuiPtr node = getNode();
    if ( nodeSerialization.front()->getPluginID() != node->getNode()->getPluginID() ) {
        QString err = tr("You cannot load %1 which are presets for the plug-in %2 on the plug-in %3.")
                      .arg( QString::fromUtf8( filename.c_str() ) )
                      .arg( QString::fromUtf8( nodeSerialization.front()->getPluginID().c_str() ) )
                      .arg( QString::fromUtf8( node->getNode()->getPluginID().c_str() ) );
        Dialogs::errorDialog( tr("Presets").toStdString(), err.toStdString() );

        return;
    }

    node->restoreInternal(node, nodeSerialization);
}

static bool
endsWith(const std::string &str,
         const std::string &suffix)
{
    return ( ( str.size() >= suffix.size() ) &&
             (str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0) );
}

void
NodeSettingsPanel::onExportPresetsActionTriggered()
{
    std::vector<std::string> filters;

    filters.push_back(NATRON_PRESETS_FILE_EXT);
    std::string filename = getGui()->popSaveFileDialog(false, filters, getGui()->getLastSaveProjectDirectory().toStdString(), false);
    if ( filename.empty() ) {
        return;
    }

    if ( !endsWith(filename, "." NATRON_PRESETS_FILE_EXT) ) {
        filename.append("." NATRON_PRESETS_FILE_EXT);
    }


    FStreamsSupport::ofstream ofile;
    FStreamsSupport::open(&ofile, filename);
    if (!ofile) {
        Dialogs::errorDialog( tr("Presets").toStdString(),
                              tr("Failed to open file %1.").arg( QString::fromUtf8( filename.c_str() ) ).toStdString(), false );

        return;
    }

    NodeGuiPtr node = getNode();
    std::list<NodeSerializationPtr> nodeSerialization;
    node->serializeInternal(nodeSerialization);
    try {
        int nNodes = nodeSerialization.size();
        boost::archive::xml_oarchive oArchive(ofile);
        oArchive << boost::serialization::make_nvp("NodesCount", nNodes);
        for (std::list<NodeSerializationPtr>::iterator it = nodeSerialization.begin();
             it != nodeSerialization.end(); ++it) {
            oArchive << boost::serialization::make_nvp("Node", **it);
        }
    }  catch (const std::exception & e) {
        Dialogs::errorDialog( "Presets", e.what() );

        return;
    }
}

void
NodeSettingsPanel::onMidiImportPresetsActionTriggered()
{
    std::vector<std::string> filters;
    filters.push_back(NATRON_MIDI_PRESETS_FILE_EXT);
    std::string filename = getGui()->popOpenFileDialog(false,
                                                       filters,
                                                       getGui()->getLastLoadProjectDirectory().toStdString(),
                                                       false);
    if ( filename.empty() ) {
        return;
    }

    int importedPresets = loadMidiKnobs( QString::fromStdString(filename) );

    QString dialogMsg = tr("Imported a total of %1 preset(s).").arg(importedPresets);
    if (importedPresets < 1) {
        dialogMsg.append( tr(" Unable to find any knobs that matches imported presets.") );
    }
    Dialogs::informationDialog(tr("Midi import preset").toStdString(),
                               dialogMsg.toStdString(),
                               false);
}

void
NodeSettingsPanel::onMidiExportPresetsActionTriggered()
{
    if ( !hasMidiKnobs() ) {
        Dialogs::warningDialog(tr("Midi preset export").toStdString(),
                               tr("Found nothing to export.").toStdString(),
                               false);
        return;
    }

    std::vector<std::string> filters;
    filters.push_back(NATRON_MIDI_PRESETS_FILE_EXT);

    std::string filename = getGui()->popSaveFileDialog(false,
                                                       filters,
                                                       getGui()->getLastSaveProjectDirectory().toStdString(),
                                                       false);
    if ( filename.empty() ) {
        return;
    }

    if ( !endsWith(filename, "." NATRON_MIDI_PRESETS_FILE_EXT) ) {
        filename.append("." NATRON_MIDI_PRESETS_FILE_EXT);
    }

    int exportedKnobs = saveMidiKnobs( QString::fromStdString(filename) );

    Dialogs::informationDialog(tr("Midi export preset").toStdString(),
                               tr("Exported presets for %1 knob(s)").arg(exportedKnobs).toStdString(),
                               false);
}

void
NodeSettingsPanel::onMidiSetDefaultPresetsActionTriggered()
{
    QString pluginID;
    if ( getNode() ) {
        pluginID  = QString::fromStdString( getNode()->getNode()->getPluginID() );
    }
    if ( pluginID.isEmpty() ) {
        return;
    }

    QString presetPath = QString::fromUtf8("%1/.%2/midi-presets")
                         .arg( QDir::homePath() )
                         .arg( QString::fromUtf8(NATRON_APPLICATION_NAME) );
    QDir presetDir(presetPath);
    if ( !presetDir.exists() ) {
        if ( !presetDir.mkpath(presetPath) ) {
            return;
        }
    }

    QString presetFile = QString::fromUtf8("%1/%2.%3")
                         .arg(presetPath)
                         .arg(pluginID)
                         .arg( QString::fromUtf8(NATRON_MIDI_PRESETS_FILE_EXT) );

    saveMidiKnobs(presetFile);
}

NATRON_NAMESPACE_EXIT

NATRON_NAMESPACE_USING
#include "moc_NodeSettingsPanel.cpp"
