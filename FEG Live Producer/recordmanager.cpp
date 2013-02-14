#include "casparconnection.h"
#include "recordmanager.h"

RecordManager::RecordManager(CasparConnection *con, QObject *parent)
	: QObject(parent)
{
	m_running = false;
	m_con = con;
}

RecordManager::~RecordManager()
{

}

void RecordManager::start()
{
	if (m_running)
	{
		return;
	}

	QDateTime start = QDateTime::currentDateTime();
	QString filename = start.toString("yyyyMMdd-hhmmss") + ".rec.mov";
	m_con->sendCommand("ADD 1-1 FILE " + filename + " -f mov -vcodec h264 -acodec aac -r 25 -b 3000000 -ac 2");
	m_current = Recording();
	m_current.filename = filename;
	m_current.start = start;

	m_running = true;
}

void RecordManager::end()
{
	if (!m_running)
	{
		return;
	}

	m_current.end = QDateTime::currentDateTime();
	m_con->sendCommand("REMOVE 1-1");
	
	m_records.append(m_current);
	m_running = false;
}

QString RecordManager::getRecordingTime()
{
	if (!m_running) {
		return tr("Not recording at the moment");
	}

	int seconds = m_current.start.secsTo(QDateTime::currentDateTime());
	return tr("%1:%2").arg(static_cast<int>(seconds / 60), 2, 10, QChar('0')).arg(static_cast<int>(seconds % 60), 2, 10, QChar('0'));
}
