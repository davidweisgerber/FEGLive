#include "casparconnection.h"
#include "fegliveproducer.h"

FEGLiveProducer::FEGLiveProducer(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	m_casparCon = new CasparConnection(this);
	m_casparCon->connectToHost("localhost", 5250);

	ui.backgroundProgramWidget->setCasparConnection(m_casparCon);
}

FEGLiveProducer::~FEGLiveProducer()
{

}
