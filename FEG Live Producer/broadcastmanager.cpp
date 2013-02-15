#include "broadcastmanager.h"

BroadcastManager::BroadcastManager( CasparConnection *casparCon, QObject *parent ) : QObject(parent)
{
	m_casparCon = casparCon;
}

BroadcastManager::~BroadcastManager()
{

}

void BroadcastManager::startBroadcast()
{

}

void BroadcastManager::stopBroadcast()
{

}

QString BroadcastManager::getBroadcastInformation()
{
	return QString();
}
