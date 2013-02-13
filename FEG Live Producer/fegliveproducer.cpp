#include <QKeyEvent>
#include "casparconnection.h"
#include "songfileparser.h"
#include "startdialog.h"
#include "configurationparser.h"
#include "optionsmenu.h"
#include "atemcontrol.h"
#include "fegliveproducer.h"

FEGLiveProducer::FEGLiveProducer(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	installEventFilter(this);
	ui.setupUi(this);
	

	QCoreApplication::setApplicationName("FEG Live Producer");
	QCoreApplication::setOrganizationName("FEGMM");
	
	m_atem = new ATEMControl(this);
	m_atem->setTransitionFramesSpinBox(ui.takeFrameSpinBox);
	m_atem->setTransitionStyleComboBox(ui.takeStyleBox);
	m_atem->addButtonInfo(ui.preview1Button, ATEMControl::ButtonInfo(1, false, ""));
	m_atem->addButtonInfo(ui.preview2Button, ATEMControl::ButtonInfo(2, false, ""));
	m_atem->addButtonInfo(ui.preview3Button, ATEMControl::ButtonInfo(3, false, ""));
	m_atem->addButtonInfo(ui.preview4Button, ATEMControl::ButtonInfo(4, false, ""));
	m_atem->addButtonInfo(ui.preview5Button, ATEMControl::ButtonInfo(5, false, ""));
	m_atem->addButtonInfo(ui.preview6Button, ATEMControl::ButtonInfo(6, false, ""));
	m_atem->addButtonInfo(ui.program1Button, ATEMControl::ButtonInfo(1, true, ""));
	m_atem->addButtonInfo(ui.program2Button, ATEMControl::ButtonInfo(2, true, ""));
	m_atem->addButtonInfo(ui.program3Button, ATEMControl::ButtonInfo(3, true, ""));
	m_atem->addButtonInfo(ui.program4Button, ATEMControl::ButtonInfo(4, true, ""));
	m_atem->addButtonInfo(ui.program5Button, ATEMControl::ButtonInfo(5, true, ""));
	m_atem->addButtonInfo(ui.program6Button, ATEMControl::ButtonInfo(6, true, ""));
	m_atem->addButtonInfo(ui.previewBlackButton, ATEMControl::ButtonInfo(0, false, ""));
	m_atem->addButtonInfo(ui.programBlackButton, ATEMControl::ButtonInfo(0, true, ""));

	connect(ui.takeButton, SIGNAL(clicked()), m_atem, SLOT(take()));
	connect(ui.takeFrameSpinBox, SIGNAL(valueChanged(int)), m_atem, SLOT(transitionFramesChanged(int)));
	connect(ui.autoTakeButton, SIGNAL(clicked(bool)), m_atem, SLOT(autoTakeChanged(bool)));
	connect(ui.takeStyleBox, SIGNAL(currentIndexChanged(int)), m_atem, SLOT(transitionChanged(int)));

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

	setWindowState(Qt::WindowMaximized);

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

	m_atem->start();

	grabKeyboard();
}

FEGLiveProducer::~FEGLiveProducer()
{

}


bool FEGLiveProducer::eventFilter( QObject *target, QEvent *e)
{
	//!TODO 

	switch (e->type())
	{
	case QEvent::ChildAdded:
		{
			QChildEvent* ce = (QChildEvent*)e;
			if (ce->child()->inherits("QTextEdit")) 
			{
				ce->child()->installEventFilter(this);
			}
			else
			{
				qDebug("TextEdit");
			}
		}
		break;

	case QEvent::ChildRemoved:
		{
			QChildEvent* ce = (QChildEvent*)e;
			ce->child()->removeEventFilter(this);
		}
		break;
	case QEvent::KeyPress:
		{
			if (ui.notesEdit->hasFocus() == false)
			{
				QKeyEvent *ev = dynamic_cast<QKeyEvent *>(e);
				m_atem->keyPressed(ev->text());
				ev->accept();
			}
			break;
		}
	}

	return QMainWindow::eventFilter(target, e);
}
