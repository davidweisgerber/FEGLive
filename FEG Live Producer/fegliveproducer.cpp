#include "casparconnection.h"
#include "fegliveproducer.h"

FEGLiveProducer::FEGLiveProducer(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	m_casparCon = new CasparConnection(this);
	m_casparCon->connectToHost("localhost", 5250);

	ui.backgroundProgramWidget->setCasparConnection(m_casparCon);

	connect(ui.backgroundTransitionComboBox, SIGNAL(activated(const QString &)), ui.backgroundProgramWidget, SLOT(setBackgroundTransitionStyle(const QString &)));
	connect(ui.backgroundTransitionTime, SIGNAL(valueChanged(int)), ui.backgroundProgramWidget, SLOT(setBackgroundTransitionTime(int)));

	ui.backgroundProgramWidget->setBackgroundTransitionStyle(ui.backgroundTransitionComboBox->currentText());
	ui.backgroundProgramWidget->setBackgroundTransitionTime(ui.backgroundTransitionTime->value());
}

FEGLiveProducer::~FEGLiveProducer()
{

}
