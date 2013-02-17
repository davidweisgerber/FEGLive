#include "casparconnection.h"
#include "broadcastmanager.h"

BroadcastManager::BroadcastManager( CasparConnection *casparCon, QObject *parent ) : QObject(parent)
{
	m_casparCon = casparCon;
	m_isBroadcasting = false;
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
		m_casparCon->sendCommand("ADD 1-10 FILE rtmp://localhost/flvplayback/high -f flv -vcodec h264 -acodec aac -r 25 -b 3000000 -ac 1");
		m_isBroadcasting = true;
	}
}
