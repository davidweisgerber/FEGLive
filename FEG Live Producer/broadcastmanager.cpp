#include <QProcess>
#include <QTimer>
#include <Windows.h>
#include "casparconnection.h"
#include "broadcastmanager.h"

BroadcastManager::BroadcastManager( CasparConnection *casparCon, QObject *parent ) : QObject(parent)
{
	m_casparCon = casparCon;
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
		m_casparCon->sendCommand("REMOVE 1-10");
		m_isBroadcasting = false;
		m_startBroadcast = QDateTime::currentDateTime();
	}
	else
	{
		m_casparCon->sendCommand("ADD 1-10 FILE rtmp://localhost/flvplayback/high -f flv -vcodec h264 -r 25 -b 3000000 -minrate 3000000 -maxrate 3000000 -bufsize 9000000 -acodec aac -ac 1");
		m_startBroadcast = QDateTime::currentDateTime();
		m_isBroadcasting = true;
	}
}

void BroadcastManager::loadAndPlaceTCPRelay()
{
	QProcess::startDetached("TCPRelay.exe");
	QTimer::singleShot(500, this, SLOT(moveWindow()));
}

void BroadcastManager::moveWindow()
{
	HWND hCasparCgWindow = FindWindowEx(NULL, NULL, L"WindowsForms10.Window.8.app.0.2bf8098_r11_ad1", L"TCPRelay");

	if (hCasparCgWindow == NULL)
	{
		qDebug("TCPRelay window was not found");
		QTimer::singleShot(500, this, SLOT(moveWindow()));
		return;
	}

	SetWindowPos(hCasparCgWindow, HWND_TOPMOST, 413, 402, 600, 200, SWP_NOACTIVATE);
}
