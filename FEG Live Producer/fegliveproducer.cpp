#include <Windows.h>
#include <QKeyEvent>
#include <QTimer>
#include "recordmanager.h"
#include "casparconnection.h"
#include "songfileparser.h"
#include "startdialog.h"
#include "configurationparser.h"
#include "optionsmenu.h"
#include "atemcontrol.h"
#include "broadcastmanager.h"
#include "fegliveproducer.h"

FEGLiveProducer::FEGLiveProducer(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	installEventFilter(this);
	ui.setupUi(this);
	

	QCoreApplication::setApplicationName("FEG Live Producer");
	QCoreApplication::setOrganizationName("FEGMM");
	
	m_atem = new ATEMControl(this);
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
	m_atem->setMixButton(ui.mixButton);
	m_atem->setDipButton(ui.dipButton);
	m_atem->setWipeButton(ui.wipeButton);
	m_atem->setHardButton(ui.hardButton);
	m_atem->setMixSpinBox(ui.mixSpinBox);
	m_atem->setDipSpinBox(ui.dipSpinBox);
	m_atem->setWipeSpinBox(ui.wipeSpinBox);
	m_atem->setDipColourBox(ui.dipColourBox);

	connect(ui.takeButton, SIGNAL(clicked()), m_atem, SLOT(take()));
	connect(ui.autoTakeButton, SIGNAL(clicked(bool)), m_atem, SLOT(autoTakeChanged(bool)));
	connect(ui.logoButton, SIGNAL(clicked()), this, SLOT(logoClicked()));

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
	m_config->parseFromFile("config/default.js");

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
	ui.backgroundProgramWidget->start();

	doNastyStuff();

	m_records = new RecordManager(m_casparCon, this);
	connect(ui.recordButton, SIGNAL(clicked()), this, SLOT(recordClicked()));

	m_broadcast = new BroadcastManager(m_casparCon, this);
	connect(ui.broadcastButton, SIGNAL(clicked()), m_broadcast, SLOT(broadcastClicked()));

	m_timer = new QTimer();
	m_timer->setInterval(1000);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(updateStuff()));
	m_timer->start();
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
				
				if (ev->key() == Qt::Key_Left)
				{
					ui.lowerThirdsTextSelect->left();
				}
				else if (ev->key() == Qt::Key_Right)
				{
					ui.lowerThirdsTextSelect->right();
				}
				else
				{
					m_atem->keyPressed(ev->text());
				}
				
				ev->accept();
			}
			break;
		}
	}

	return QMainWindow::eventFilter(target, e);
}

void FEGLiveProducer::moveCasparCgOglWindow()
{
	HWND hCasparCgWindow = FindWindowEx(NULL, NULL, L"SFML_Window", L"ogl[1|720p5000]");

	if (hCasparCgWindow == NULL)
	{
		qDebug("caspar cg window was not found");
		return;
	}

	SetWindowPos(hCasparCgWindow, HWND_TOPMOST, 1052, 408, 860, 484, SWP_NOACTIVATE);
}

void FEGLiveProducer::doNastyStuff()
{
	grabKeyboard();
	moveCasparCgOglWindow();
	logoClicked();
}

void FEGLiveProducer::updateStuff()
{
	ui.recordLabel->setText(m_records->getRecordingTime());
	ui.broadcastLabel->setText(m_broadcast->getBroadcastInformation());
}

void FEGLiveProducer::recordClicked()
{
	if (ui.recordButton->isChecked())
	{
		m_records->start();
	}
	else
	{
		m_records->end();
	}
}

void FEGLiveProducer::logoClicked()
{
	if (ui.logoButton->isChecked())
	{
		m_casparCon->sendCommand("CG 1-10 ADD 10 " + m_config->getLogo() + " 1");
	}
	else
	{
		m_casparCon->sendCommand("CG 1-10 STOP 10");
	}
}
