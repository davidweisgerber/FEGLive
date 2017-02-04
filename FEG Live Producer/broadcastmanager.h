#ifndef BROADCASTMANAGER_H
#define BROADCASTMANAGER_H

#include <QObject>
#include <QDateTime>

class CasparConnection;
class ConfigurationParser;

class BroadcastManager : public QObject
{
	Q_OBJECT

public:
    BroadcastManager(CasparConnection *casparCon, ConfigurationParser *config, QObject *parent);
	~BroadcastManager();

	QString getBroadcastInformation();
	bool isBroadcasting();

public slots:
	void broadcastClicked();

private slots:
	void moveWindow();

private:
	void loadAndPlaceTCPRelay();
	CasparConnection *m_casparCon;
    ConfigurationParser *m_config;

	QDateTime m_startBroadcast;

	bool m_isBroadcasting;
};

#endif // BROADCASTMANAGER_H
