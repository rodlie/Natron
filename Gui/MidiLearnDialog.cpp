#include "MidiLearnDialog.h"

CLANG_DIAG_OFF(deprecated)
CLANG_DIAG_OFF(uninitialized)
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
CLANG_DIAG_ON(deprecated)
CLANG_DIAG_ON(uninitialized)

#include "Gui/Label.h"
#include "Gui/Button.h"

NATRON_NAMESPACE_ENTER

MidiLearnDialog::MidiLearnDialog(int key,
                                 double min,
                                 double max,
                                 MidiHandler *handler,
                                 QWidget *parent)
    : QDialog(parent)
    , _key(NULL)
    , _min(NULL)
    , _max(NULL)
{
    setWindowTitle( tr("Midi Learn") );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QWidget *keyWidget = new QWidget(this);
    QWidget *minWidget = new QWidget(this);
    QWidget *maxWidget = new QWidget(this);
    QWidget *btnWidget = new QWidget(this);

    QHBoxLayout *keyLayout = new QHBoxLayout(keyWidget);
    QHBoxLayout *minLayout = new QHBoxLayout(minWidget);
    QHBoxLayout *maxLayout = new QHBoxLayout(maxWidget);
    QHBoxLayout *btnLayout = new QHBoxLayout(btnWidget);

    _key = new SpinBox(this, SpinBox::eSpinBoxTypeInt);
    _min = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);
    _max = new SpinBox(this, SpinBox::eSpinBoxTypeDouble);

    Label *keyLabel = new Label(tr("Input key"), this);
    Label *minLabel = new Label(tr("Input minimum"), this);
    Label *maxLabel = new Label(tr("Input maximum"), this);

    Button *cancelButton = new Button(this);
    Button *clearButton = new Button(this);
    Button *applyButton = new Button(this);

    keyWidget->setContentsMargins(0, 0, 0, 0);
    minWidget->setContentsMargins(0, 0, 0, 0);
    maxWidget->setContentsMargins(0, 0, 0, 0);
    btnWidget->setContentsMargins(0, 0, 0, 0);

    keyLayout->setContentsMargins(0, 0, 0, 0);
    minLayout->setContentsMargins(0, 0, 0, 0);
    maxLayout->setContentsMargins(0, 0, 0, 0);
    btnLayout->setContentsMargins(0, 0, 0, 0);

    cancelButton->setText( tr("Cancel") );
    clearButton->setText( tr("Clear") );
    applyButton->setText( tr("Apply") );

    keyLabel->setToolTip( tr("Midi key to accept input from. Change the value on your Midi device to set.") );
    minLabel->setToolTip( tr("Minimum value input can set.") );
    maxLabel->setToolTip( tr("Maximum value input can set.") );
    clearButton->setToolTip( tr("Clear Midi input.") );

    keyLayout->addWidget(keyLabel);
    keyLayout->addWidget(_key);

    minLayout->addWidget(minLabel);
    minLayout->addWidget(_min);

    maxLayout->addWidget(maxLabel);
    maxLayout->addWidget(_max);

    btnLayout->addWidget(cancelButton);
    btnLayout->addWidget(clearButton);
    btnLayout->addWidget(applyButton);

    mainLayout->addWidget(keyWidget);
    mainLayout->addWidget(minWidget);
    mainLayout->addWidget(maxWidget);
    mainLayout->addStretch();
    mainLayout->addWidget(btnWidget);

    clearOptions(key, min, max);

    QObject::connect( applyButton, SIGNAL( clicked() ),
                      this, SLOT( accept() ) );
    QObject::connect( cancelButton, SIGNAL( clicked() ),
                      this, SLOT( reject() ) );
    QObject::connect( clearButton, SIGNAL( clicked() ),
                      this, SLOT(resetOptions() ) );
    if (handler) {
        QObject::connect( handler, SIGNAL( newInputValue(int,int) ),
                          this, SLOT( onMidiInputChanged(int,int) ) );
    }
}

void
MidiLearnDialog::getOptions(int *key,
                            double *min,
                            double *max)
{
    *key = _key->value();
    *min = _min->value();
    *max = _max->value();
}

void
MidiLearnDialog::onMidiInputChanged(int key,
                                    int /*value*/)
{
    _key->setValue(key);
}

void
MidiLearnDialog::clearOptions(int key = 0,
                              double min = 0.0,
                              double max = 1.0)
{
    _key->setValue(key);
    _min->setValue(min);
    _max->setValue(max);
}

void
MidiLearnDialog::resetOptions()
{
    clearOptions();
}

NATRON_NAMESPACE_EXIT

NATRON_NAMESPACE_USING
#include "moc_MidiLearnDialog.cpp"
