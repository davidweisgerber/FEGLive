#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QList>

class CasparConnection;
class ConfigurationParser;

class RecordManager : public QObject
{
	Q_OBJECT

public:
	class Recording {
	public:
		QString filename;
		QDateTime start;
		QDateTime end;
		bool savedAudio;
		bool savedVideo;

		Recording() {
			savedVideo = savedAudio = false;
		}
	};

    RecordManager(CasparConnection *con, ConfigurationParser *config, QObject *parent);
	~RecordManager();

	QString getRecordingTime();
	const QList<Recording> &getRecordings();
	bool isRecording();

public slots:
	void start();
	void end();

private:
	CasparConnection *m_con;
    ConfigurationParser *m_config;
	bool m_running;
	QList<Recording> m_records;
	Recording m_current;
};

#endif // RECORDMANAGER_H
