#include <Windows.h>
#include <QSettings>
#include <QHostAddress>
#include <QTimer>
#include <QComboBox>
#include <QSpinBox>
#include <QToolButton>
#include "qatemconnection.h"
#include "atemcontrol.h"

ATEMControl::ATEMControl(QObject *parent)
	: QObject(parent),
	m_autoTake(false)
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
	if(m_hardButton->isChecked()) 
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

void ATEMControl::transitionChanged()
{
	QToolButton *senderButton = dynamic_cast<QToolButton *>(sender());

	for (int i=0; i < m_buttons.size(); i++)
	{
		if (m_buttons[i] != senderButton)
		{
			m_buttons[i]->setChecked(false);
		}
	}

	if (senderButton == m_mixButton)
	{
		m_con->setTransitionType(0);
	}
	else if (senderButton == m_dipButton)
	{
		m_con->setTransitionType(1);
	} 
	else if (senderButton == m_wipeButton)
	{
		m_con->setTransitionType(2);
	}
	else
	{
		m_con->setTransitionType(3);
	}
}

void ATEMControl::transitionFramesChanged(int frames)
{
	QSpinBox *senderBox = dynamic_cast<QSpinBox *>(sender());

	if (senderBox == m_mixSpinBox)
	{
		m_con->setMixFrames(frames);
	}
	else if (senderBox == m_dipSpinBox)
	{
		m_con->setDipFrames(frames);
	}
	else if (senderBox == m_wipeSpinBox)
	{
		m_con->setWipeFrames(frames);
	}	
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

void ATEMControl::start()
{
	int transitionStyle = m_con->transitionStyle();

	switch (transitionStyle)
	{
	case 0:
		m_mixButton->setChecked(true);
		break;
	case 1:
		m_dipButton->setChecked(true);
		break;
	case 2:
		m_wipeButton->setChecked(true);
		break;
	case 3:
		m_hardButton->setChecked(true);
		break;
	}

	m_mixSpinBox->setValue(m_con->mixFrames());
	m_dipSpinBox->setValue(m_con->dipFrames());
	m_wipeSpinBox->setValue(m_con->wipeFrames());

	QColor dipColor = m_con->colorGeneratorColor(0);
	if (dipColor == Qt::black)
	{
		m_dipColourBox->setCurrentIndex(1);
	}
	else if (dipColor == Qt::white)
	{
		m_dipColourBox->setCurrentIndex(0);
	}
	else
	{
		m_dipColourBox->setCurrentIndex(0);
		dipColourChanged(0);
	}
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
		if (m_autoTake)
		{
			autoTake(num);
		}
		else
		{
			m_con->changePreviewInput(num);
		}
	}
}

void ATEMControl::setHardButton( QToolButton *button )
{
	m_hardButton = button;
	addAndConnectTransitionButton(button);
}

void ATEMControl::setMixButton( QToolButton *button )
{
	m_mixButton = button;
	addAndConnectTransitionButton(button);
}

void ATEMControl::setDipButton( QToolButton *button )
{
	m_dipButton = button;
	addAndConnectTransitionButton(button);
}

void ATEMControl::setWipeButton( QToolButton *button )
{
	m_wipeButton = button;
	addAndConnectTransitionButton(button);
}

void ATEMControl::setMixSpinBox( QSpinBox *spinBox )
{
	m_mixSpinBox = spinBox;
	connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(transitionFramesChanged(int)));
}

void ATEMControl::setDipSpinBox( QSpinBox *spinBox )
{
	m_dipSpinBox = spinBox;
	connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(transitionFramesChanged(int)));
}

void ATEMControl::setWipeSpinBox( QSpinBox *spinBox )
{
	m_wipeSpinBox = spinBox;
	connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(transitionFramesChanged(int)));
}

void ATEMControl::setDipColourBox( QComboBox *comboBox )
{
	m_dipColourBox = comboBox;
	connect(m_dipColourBox, SIGNAL(currentIndexChanged(int)), this, SLOT(dipColourChanged(int)));
}

void ATEMControl::addAndConnectTransitionButton( QToolButton *button )
{
	m_buttons.append(button);
	connect(button, SIGNAL(clicked()), this, SLOT(transitionChanged()));
}

void ATEMControl::dipColourChanged( int index )
{
	QColor color = Qt::white;
	switch (index)
	{
	case 0:
		color = Qt::white;
		break;
	case 1:
		color = Qt::black;
		break;
	default:
		color = Qt::white;
		break;
	}

	m_con->setColorGeneratorColor(0, color);
	m_con->setColorGeneratorColor(1, color);
}

void ATEMControl::autoTake(int button)
{
	if(m_hardButton->isChecked()) 
	{
		int program = m_con->programInput();
		
		m_con->changeProgramInput(button);
		m_con->changePreviewInput(program);
	}
	else
	{
		m_con->changePreviewInput(button);
		m_con->doAuto();
	}
}
