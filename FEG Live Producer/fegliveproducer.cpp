#include "casparconnection.h"
#include "songfileparser.h"
#include "startdialog.h"
#include "configurationparser.h"
#include "fegliveproducer.h"

FEGLiveProducer::FEGLiveProducer(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	QCoreApplication::setApplicationName("FEG Live Producer");
	QCoreApplication::setOrganizationName("FEGMM");
	
	m_casparCon = new CasparConnection(this);
	m_casparCon->connectToHost("localhost", 5250);

	ui.backgroundProgramWidget->setCasparConnection(m_casparCon);
	ui.lowerThirdsTextSelect->setCasparConnection(m_casparCon);

	connect(ui.backgroundTransitionComboBox, SIGNAL(activated(const QString &)), ui.backgroundProgramWidget, SLOT(setBackgroundTransitionStyle(const QString &)));
	connect(ui.backgroundTransitionTime, SIGNAL(valueChanged(int)), ui.backgroundProgramWidget, SLOT(setBackgroundTransitionTime(int)));
	connect(ui.lowerThirdsSelect, SIGNAL(lowerThirdChanged(const LowerThird &)), ui.lowerThirdsTextSelect, SLOT(setLowerThird(const LowerThird &)));

	ui.backgroundProgramWidget->setBackgroundTransitionStyle(ui.backgroundTransitionComboBox->currentText());
	ui.backgroundProgramWidget->setBackgroundTransitionTime(ui.backgroundTransitionTime->value());

	m_startDialog = new StartDialog(this);
	m_startDialog->exec();

	m_config = new ConfigurationParser();
	m_config->parseFromFile("../testfiles/testconfig.js");

	foreach (QString song, m_startDialog->getSongs()) {
		SongFileParser songFile;
		songFile.parseFromFile(song);

		ui.lowerThirdsSelect->addLowerThird(songFile.toLowerThird(m_config->getSongLowerThird()));
	}

	foreach (SelectData *data, m_config->getProgram()) {
		ui.backgroundProgramWidget->addSelectData(data);
	}
	ui.backgroundProgramWidget->setDefault(m_config->getProgram()[m_config->getDefaultClip()]);
}

FEGLiveProducer::~FEGLiveProducer()
{

}
