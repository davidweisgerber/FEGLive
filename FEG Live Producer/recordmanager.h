#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QList>

class CasparConnection;

class RecordManager : public QObject
{
	Q_OBJECT

public:
	class Recording {
	public:
		QString filename;
		QDateTime start;
		QDateTime end;
	};

	RecordManager(CasparConnection *con, QObject *parent);
	~RecordManager();

	QString getRecordingTime();

public slots:
	void start();
	void end();

private:
	CasparConnection *m_con;
	bool m_running;
	QList<Recording> m_records;
	Recording m_current;
};

#endif // RECORDMANAGER_H
