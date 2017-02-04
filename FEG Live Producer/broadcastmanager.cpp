#include <QSettings>
#include <QProcess>
#include <QTimer>
#include <Windows.h>
#include "casparconnection.h"
#include "configurationparser.h"
#include "broadcastmanager.h"

BroadcastManager::BroadcastManager( CasparConnection *casparCon, ConfigurationParser *config, QObject *parent ) : QObject(parent)
{
	m_casparCon = casparCon;
    m_config = config;
	m_isBroadcasting = false;
	loadAndPlaceTCPRelay();
}

BroadcastManager::~BroadcastManager()
{

}

QString BroadcastManager::getBroadcastInformation()
{
	if (!m_isBroadcasting) {
		return tr("Not broadcasting at the moment");
	}

	int seconds = m_startBroadcast.secsTo(QDateTime::currentDateTime());
	return tr("Broadcasting for %1:%2").arg(static_cast<int>(seconds / 60), 2, 10, QChar('0')).arg(static_cast<int>(seconds % 60), 2, 10, QChar('0'));
}

void BroadcastManager::broadcastClicked()
{
	if (m_isBroadcasting)
	{
        for (int i=0; i < m_config->getStreamingOptions().size(); i++)
        {
            m_casparCon->sendCommand(QString("REMOVE 1-%1").arg(i+10));
        }

		m_isBroadcasting = false;
		m_startBroadcast = QDateTime::currentDateTime();
	}
	else
	{
        for (int i=0; i < m_config->getStreamingOptions().size(); i++)
        {
            m_casparCon->sendCommand(QString("ADD 1-%1 STREAM %2").arg(i+10).arg(m_config->getStreamingOptions()[i]));
        }

        m_startBroadcast = QDateTime::currentDateTime();
		m_isBroadcasting = true;
	}
}

void BroadcastManager::loadAndPlaceTCPRelay()
{
	QSettings tcpRelaySettings(QSettings::NativeFormat, QSettings::UserScope, "StrikerX3", "TCPRelay");
	tcpRelaySettings.setValue("LastTargetURI", "rtmp://streaming.cloud.fegmm.de/");
	tcpRelaySettings.setValue("LastListenPort", 1935);

	QProcess::startDetached("TCPRelay.exe");
	QTimer::singleShot(500, this, SLOT(moveWindow()));
}

void BroadcastManager::moveWindow()
{
    HWND tcpRelayHandle = FindWindowEx(NULL, NULL, L"WindowsForms10.Window.8.app.0.141b42a_r10_ad1", L"TCPRelay");

    if (tcpRelayHandle == NULL)
	{
		qDebug("TCPRelay window was not found");
		QTimer::singleShot(500, this, SLOT(moveWindow()));
		return;
	}

    qDebug() << SetWindowPos(tcpRelayHandle, HWND_TOPMOST, 413, 402, 600, 200, SWP_NOACTIVATE);

    long dwExStyle = GetWindowLong(tcpRelayHandle, GWL_EXSTYLE);
    dwExStyle |= WS_EX_TOPMOST;
    SetWindowLong(tcpRelayHandle, GWL_EXSTYLE, dwExStyle);

}

bool BroadcastManager::isBroadcasting()
{
	return m_isBroadcasting;
}
