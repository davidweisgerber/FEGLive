#ifndef SAVERECORD_H
#define SAVERECORD_H

#include <QDialog>
#include <QProcess>

#include "recordmanager.h"
namespace Ui {class SaveRecord;};

class StartDialog;
class ConfigurationParser;
class QMessageBox;

class SaveRecord : public QDialog
{
	Q_OBJECT

public:
	SaveRecord(const QList<RecordManager::Recording> &records, StartDialog *startDialog, ConfigurationParser *config, QWidget *parent = 0);
	~SaveRecord();

private slots:
	void saveAudio();
	void saveVideo();
	void saveAudioFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
	void copyVideo(QString to);
	void buildList();
	QString getFilenamePart();
	Ui::SaveRecord *ui;

	StartDialog *m_startDialog;
	ConfigurationParser *m_config;
	QList<RecordManager::Recording> m_records;
	QMessageBox *m_waitingBox;
};

#endif // SAVERECORD_H
