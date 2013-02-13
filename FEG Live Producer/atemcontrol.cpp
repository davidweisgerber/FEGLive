#include <QSettings>
#include <QHostAddress>
#include <QTimer>
#include <QComboBox>
#include <QSpinBox>
#include "qatemconnection.h"
#include "atemcontrol.h"

ATEMControl::ATEMControl(QObject *parent)
	: QObject(parent),
	m_autoTake(true)
{
	m_con = new QAtemConnection(this);

	QSettings settings;
	m_con->connectToSwitcher(QHostAddress(settings.value("atem", "192.168.1.21").toString()));

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(timerTick()));

	m_timer->setInterval(200);
	m_timer->setSingleShot(false);
	m_timer->start();
}

ATEMControl::~ATEMControl()
{
	delete m_con;
	delete m_timer;
}

void ATEMControl::buttonPressed()
{
	QToolButton *button = dynamic_cast<QToolButton *>(sender());
	if (!m_buttonInfos.contains(button)) 
	{
		qDebug("Button not found???");
		return;
	}

	if (m_buttonInfos[button].program) 
	{
		m_con->changeProgramInput(m_buttonInfos[button].input);
	}
	else 
	{
		m_con->changePreviewInput(m_buttonInfos[button].input);
	}
}

void ATEMControl::setButtonInfos( const QMap<QToolButton *, ButtonInfo> &buttonInfos )
{
	m_buttonInfos = buttonInfos;
}

void ATEMControl::addButtonInfo( QToolButton * button, const ButtonInfo &buttonInfo )
{
	connect(button, SIGNAL(clicked()), this, SLOT(buttonPressed()));
	button->setAutoFillBackground(true);
	m_buttonInfos.insert(button, buttonInfo);
}

void ATEMControl::take()
{
	if (m_transitionStyleBox->currentIndex() == 3) 
	{
		int program = m_con->programInput();
		int preview = m_con->previewInput();

		m_con->changeProgramInput(preview);
		m_con->changePreviewInput(program);
	}
	else
	{
		m_con->doAuto();
	}
}

void ATEMControl::transitionChanged( int transitionNo )
{
	m_con->setTransitionType(transitionNo);
}

void ATEMControl::transitionFramesChanged( int frames )
{
	m_con->setWipeFrames(frames);
	m_con->setDipFrames(frames);
	m_con->setMixFrames(frames);
}

void ATEMControl::autoTakeChanged( bool on )
{
	m_autoTake = on;
}

void ATEMControl::timerTick()
{
	int program = m_con->programInput();
	int preview = m_con->previewInput();

	QMap<QToolButton *, ButtonInfo>::iterator it = m_buttonInfos.begin();
	while (it != m_buttonInfos.end()) 
	{
		it.key()->setStyleSheet("");

		if (program == it->input && it->program == true) 
		{
			it.key()->setStyleSheet("background-color: rgb(255, 0, 0);");
		}

		if (preview == it->input && it->program == false)
		{
			it.key()->setStyleSheet("background-color: rgb(0, 255, 0);");
		}

		++it;
	}
}

void ATEMControl::setTransitionStyleComboBox( QComboBox *box )
{
	m_transitionStyleBox = box;
}

void ATEMControl::setTransitionFramesSpinBox( QSpinBox *box )
{
	m_transitionFramesBox = box;
}

void ATEMControl::start()
{
	m_transitionStyleBox->setCurrentIndex(m_con->transitionStyle());
	m_transitionFramesBox->setValue(m_con->mixFrames());
	m_con->setColorGeneratorColor(0, Qt::white);
	m_con->setColorGeneratorColor(1, Qt::white);
}

void ATEMControl::keyPressed(const QString &key)
{
	int num = key.toInt();

	if (num == 0) 
	{
		if (key.toLower() == "b") 
		{
			num = 0;
		}
		else if (key == " ")
		{
			take();
			return;
		}
		else
		{
			return;
		}
	}

	

	if (num >= 0 && num < 7)
	{
		m_con->changePreviewInput(num);

		if (m_autoTake)
		{
			take();
		}
	}
}
