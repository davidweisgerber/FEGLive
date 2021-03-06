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
#include "saverecord.h"
#include "addlowerthirddialog.h"
#include "jsonserializer.h"
#include "fegliveproducer.h"

FEGLiveProducer::FEGLiveProducer(QWidget *parent)
    : QMainWindow(parent),
	m_notesFile("notes.txt")
{
	ui.setupUi(this);
	
    m_lastKeyEvent = QDateTime::currentDateTime();

	connect(ui.actionAddLowerThird, SIGNAL(triggered()), this, SLOT(addLowerThird()));
	connect(ui.actionReposition_Windows, SIGNAL(triggered()), this, SLOT(moveCasparCgOglWindow()));
	connect(ui.actionSecond_Monitor_Output, SIGNAL(triggered()), this, SLOT(toggleSecondMonitor()));

	QCoreApplication::setApplicationName("FEG Live Producer");
	QCoreApplication::setOrganizationName("FEGMM");
	
	// Input fields that block atem control
	m_inputFields.append(ui.notesEdit);
	m_inputFields.append(ui.mixSpinBox);
	m_inputFields.append(ui.dipSpinBox);
	m_inputFields.append(ui.wipeSpinBox);
	m_inputFields.append(ui.backgroundTransitionTime);

	m_lastStartBroadcastClicked = QDateTime::currentDateTime();
	m_lastStartRecordClicked = QDateTime::currentDateTime();
	m_lastTakeClicked = QDateTime::currentDateTime();
	m_lastTakeHappened = QDateTime::currentDateTime();

	m_dialogOpen = false;

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

	connect(ui.takeButton, SIGNAL(clicked()), this, SLOT(takeClicked()));
	connect(ui.autoTakeButton, SIGNAL(clicked(bool)), m_atem, SLOT(autoTakeChanged(bool)));
	connect(ui.logoButton, SIGNAL(clicked()), this, SLOT(logoClicked()));
	connect(m_atem, SIGNAL(takeHappened()), this, SLOT(takeHappened()));

	m_casparCon = new CasparConnection(this);
	m_casparCon->connectToHost("localhost", 5250);

	ui.backgroundProgramWidget->setCasparConnection(m_casparCon);
	ui.lowerThirdsTextSelect->setCasparConnection(m_casparCon);

	connect(ui.backgroundTransitionComboBox, SIGNAL(activated(const QString &)), ui.backgroundProgramWidget, SLOT(setBackgroundTransitionStyle(const QString &)));
	connect(ui.backgroundTransitionTime, SIGNAL(valueChanged(int)), ui.backgroundProgramWidget, SLOT(setBackgroundTransitionTime(int)));
	connect(ui.lowerThirdsSelect, SIGNAL(lowerThirdChanged(LowerThird &)), ui.lowerThirdsTextSelect, SLOT(setLowerThird(LowerThird &)));

	ui.backgroundProgramWidget->setBackgroundTransitionStyle(ui.backgroundTransitionComboBox->currentText());
	ui.backgroundProgramWidget->setBackgroundTransitionTime(ui.backgroundTransitionTime->value());

	m_startDialog = new StartDialog(this);
	m_dialogOpen = true;
    if (m_startDialog->exec() != QDialog::Accepted)
    {
        m_dontStart = true;
        return;
    }
    else
    {
        m_dontStart = false;
    }

	m_dialogOpen = false;

	setWindowState(Qt::WindowMaximized);

	m_config = new ConfigurationParser();
	QFile configFileFile(m_startDialog->getConfigurationFileName());
	configFileFile.open(QIODevice::ReadOnly);
	QByteArray configFile = "{" + configFileFile.readAll() + "}";

	JSONSerializer serializer;
	int listSelectDataId = qRegisterMetaType<QList<SelectData *> >("QList<SelectData*>");
	serializer.registerUserType(listSelectDataId, new ConfigurationParser::ListSelectDataSerializer());
	serializer.deserialize(configFile, m_config);

	qDebug("%s", serializer.serialize(*m_config).data());
	
	LowerThirdsText preacher(m_startDialog->getPreacher(), m_config->getPreacherLowerThirdTitle());
	LowerThirdsText bibleText(m_startDialog->getBibleText(), m_config->getBibleTextLowerThirdTitle());
	LowerThirdsText topic(m_startDialog->getSermonTitle(), m_config->getTopicLowerThirdTitle());

	LowerThird generalLowerThird(tr("General"), m_config->getGeneralLowerThird(), QList<LowerThirdsText>() << m_config->getPreConfiguredLowerThirdsList() << preacher << bibleText << topic); 
	ui.lowerThirdsSelect->addLowerThird(generalLowerThird);
	ui.lowerThirdsTextSelect->setLowerThird(generalLowerThird);

	foreach (QString song, m_config->getPreConfiguredSongs()) {
		SongFileParser songFile;
		songFile.parseFromFile(song);

		ui.lowerThirdsSelect->addLowerThird(songFile.toLowerThird(m_config->getSongLowerThird()));
	}

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

    m_records = new RecordManager(m_casparCon, m_config, this);
	connect(ui.recordButton, SIGNAL(clicked()), this, SLOT(recordClicked()));
	connect(ui.saveRecordButton, SIGNAL(clicked()), this, SLOT(saveRecordsClicked()));

    m_broadcast = new BroadcastManager(m_casparCon, m_config, this);
	connect(ui.broadcastButton, SIGNAL(clicked()), this, SLOT(broadcastClicked()));

	m_timer = new QTimer();
	m_timer->setInterval(1000);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(updateStuff()));
	m_timer->start();

	qApp->installEventFilter(this);
	if (m_notesFile.open(QIODevice::ReadWrite)) {
		ui.notesEdit->setText(QString::fromUtf8(m_notesFile.readAll()));
	} else {
		ui.notesEdit->setText(tr("notes.txt could not be opened. That means that notes are not available in this session."));
	}
	connect(ui.notesEdit, SIGNAL(textChanged()), this, SLOT(notesChanged()));

    ui.retranslateUi(this);
}

bool FEGLiveProducer::isAbortStart()
{
    return m_dontStart;
}

FEGLiveProducer::~FEGLiveProducer()
{

}


bool FEGLiveProducer::eventFilter( QObject *target, QEvent *e)
{
	switch (e->type())
	{
	case QEvent::LanguageChange:
		ui.retranslateUi(this);
		break;
	case QEvent::KeyPress:
		{
			QKeyEvent *ev = dynamic_cast<QKeyEvent *>(e);

			int ret = inputFieldHasFocus(ev);
			if (ret == 1)
			{
				return true;
			} 
			else if (ret == 0)
			{
				return false;
			}
			
            if (qAbs(m_lastKeyEvent.msecsTo(QDateTime::currentDateTime())) < 100)
            {
                return false;
            }

            m_lastKeyEvent = QDateTime::currentDateTime();
				
			if (ev->key() == Qt::Key_Left)
			{
				ui.lowerThirdsTextSelect->left();
                return true;
			}
			else if (ev->key() == Qt::Key_Right)
			{
				ui.lowerThirdsTextSelect->right();
                return true;
			}
			else if (ev->key() == Qt::Key_Up)
			{
				ui.lowerThirdsSelect->next();
                return true;
			}
			else if (ev->key() == Qt::Key_Down)
			{
				ui.lowerThirdsSelect->previous();
                return true;
			}
			else if (ev->key() == Qt::Key_Tab)
			{
				return true;
			}
			else if (ev->key() == Qt::Key_Enter || ev->key() == Qt::Key_Return)
			{
				m_atem->takeHard();
			}
			else
			{
				m_atem->keyPressed(ev->text());
			}

			return false;
		}
	}

	return false;
}

void FEGLiveProducer::moveCasparCgOglWindow()
{
    HWND hCasparCgWindow = FindWindowEx(NULL, NULL, L"SFML_Window", L"Screen consumer [1|720p5000]");

	if (hCasparCgWindow == NULL)
	{
		qDebug("caspar cg window was not found");
		return;
	}

	SetWindowPos(hCasparCgWindow, HWND_TOPMOST, 1052, 408, 860, 484, SWP_NOACTIVATE);

    hCasparCgWindow = FindWindowEx(NULL, hCasparCgWindow, L"SFML_Window", L"Screen consumer [1|720p5000]");
	if (hCasparCgWindow == NULL)
	{
		return;
	}

	DWORD dwStyle = GetWindowLong(hCasparCgWindow, GWL_STYLE);
	SetWindowLong(hCasparCgWindow, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
	
	MONITORINFO mi = { sizeof(mi) };
	GetMonitorInfo(MonitorFromWindow(hCasparCgWindow,
		MONITOR_DEFAULTTOPRIMARY), &mi);
	SetWindowPos(hCasparCgWindow, HWND_TOP,
		mi.rcMonitor.left, mi.rcMonitor.top,
		mi.rcMonitor.right - mi.rcMonitor.left,
		mi.rcMonitor.bottom - mi.rcMonitor.top,
		SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
}

void FEGLiveProducer::doNastyStuff()
{
	moveCasparCgOglWindow();
	logoClicked();
}

void FEGLiveProducer::updateStuff()
{
	ui.recordLabel->setText(m_records->getRecordingTime());
	ui.broadcastLabel->setText(m_broadcast->getBroadcastInformation());
	ui.lastTakeLabel->setText(QString::number(m_lastTakeHappened.secsTo(QDateTime::currentDateTime())));
}

void FEGLiveProducer::recordClicked()
{
	if (m_lastStartRecordClicked.msecsTo(QDateTime::currentDateTime()) > 1000)
	{
		if (ui.recordButton->isChecked())
		{
			m_records->start();
		}
		else
		{
			m_records->end();
		}
		m_lastStartRecordClicked = QDateTime::currentDateTime();
	}
	else
	{
		if (m_records->isRecording())
		{
			ui.recordButton->setDown(true);
		}
		else
		{
			ui.recordButton->setDown(false);
		}
	}
	
}

void FEGLiveProducer::logoClicked()
{
	if (ui.logoButton->isChecked())
	{
        m_casparCon->sendCommand("PLAY 1-10 [HTML] \"" + m_config->getLogo() + "\"");
	}
	else
	{
        m_casparCon->sendCommand("STOP 1-10");
	}
}

void FEGLiveProducer::saveRecordsClicked()
{
	m_dialogOpen = true;
	SaveRecord saver(m_records->getRecordings(), m_startDialog, m_config, this);
	saver.exec();
	m_dialogOpen = false;
}

void FEGLiveProducer::addLowerThird()
{
	m_dialogOpen = true;
	AddLowerThirdDialog dlg(ui.lowerThirdsSelect, m_config, 0);
	connect(&dlg, SIGNAL(changed()), ui.lowerThirdsTextSelect, SLOT(update()));
	dlg.exec();
	m_dialogOpen = false;
}

void FEGLiveProducer::toggleSecondMonitor()
{
	if (ui.actionSecond_Monitor_Output->isChecked()) 
	{
		m_casparCon->sendCommand("ADD 1-100 SCREEN 2 DEVICE 1");
		QTimer::singleShot(1000, this, SLOT(moveCasparCgOglWindow()));
	}
	else
	{
		m_casparCon->sendCommand("REMOVE 1-100");
	}
}

void FEGLiveProducer::notesChanged()
{
	if (m_notesFile.isOpen()) 
	{
		m_notesFile.seek(0);
		QByteArray noteText = ui.notesEdit->toPlainText().toUtf8();
		m_notesFile.write(noteText);
		m_notesFile.resize(noteText.length());
	}
}

int FEGLiveProducer::inputFieldHasFocus( QKeyEvent * ev )
{
	foreach (QWidget *widget, m_inputFields)
	{
		if (widget->hasFocus()) 
		{
			if (ev->key() == Qt::Key_Tab)
			{
				ui.lowerThirdsSelect->setFocus();
				ev->accept();
				return 1;
			}

			return 0;
		}
	}

	if (m_dialogOpen)
	{
		return 0;
	}

	return -1;
}

void FEGLiveProducer::broadcastClicked()
{
	if (m_lastStartBroadcastClicked.msecsTo(QDateTime::currentDateTime()) > 1000)
	{
		m_broadcast->broadcastClicked();
		m_lastStartBroadcastClicked = QDateTime::currentDateTime();
	}
	else
	{
		if (m_broadcast->isBroadcasting())
		{
			ui.broadcastButton->setDown(true);
		}
		else
		{
			ui.broadcastButton->setDown(false);
		}
	}
}

void FEGLiveProducer::takeClicked()
{
	if (m_lastTakeClicked.msecsTo(QDateTime::currentDateTime()) > 1000)
	{
		m_atem->take();
		m_lastTakeClicked = QDateTime::currentDateTime();
	}
}

void FEGLiveProducer::takeHappened()
{
	m_lastTakeHappened = QDateTime::currentDateTime();
}
