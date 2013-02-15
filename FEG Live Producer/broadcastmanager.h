#ifndef BROADCASTMANAGER_H
#define BROADCASTMANAGER_H

#include <QObject>
#include <QDateTime>

class CasparConnection;

class BroadcastManager : public QObject
{
	Q_OBJECT

public:
	BroadcastManager(CasparConnection *casparCon, QObject *parent);
	~BroadcastManager();

	QString getBroadcastInformation();

public slots:
	void startBroadcast();
	void stopBroadcast();

private:
	CasparConnection *m_casparCon;

	QDateTime m_startBroadcast;

	bool m_isBroadcasting;
};

#endif // BROADCASTMANAGER_H
