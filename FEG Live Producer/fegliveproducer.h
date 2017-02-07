#ifndef FEGLIVEPRODUCER_H
#define FEGLIVEPRODUCER_H

#include <QMainWindow>
#include <QDateTime>
#include <QFile>
#include "ui_fegliveproducer.h"

class CasparConnection;
class StartDialog;
class ConfigurationParser;
class ATEMControl;
class QKeyEvent;
class RecordManager;
class BroadcastManager;
class QTimer;

class FEGLiveProducer : public QMainWindow
{
	Q_OBJECT

public:
    FEGLiveProducer(QWidget *parent = 0);
    bool isAbortStart();
	~FEGLiveProducer();

protected:
	virtual bool eventFilter(QObject *watched, QEvent *event);

private slots:
	void updateStuff();
	void recordClicked();
	void broadcastClicked();
	void takeClicked();
	void logoClicked();
	void saveRecordsClicked();
	void addLowerThird();
	void moveCasparCgOglWindow();
	void toggleSecondMonitor();
	void notesChanged();
	void takeHappened();
	
private:
	void doNastyStuff();
	int inputFieldHasFocus( QKeyEvent * ev );

	Ui::FEGLiveProducerClass ui;
	CasparConnection *m_casparCon;
	StartDialog *m_startDialog;
	ConfigurationParser *m_config;
	ATEMControl *m_atem;
	RecordManager *m_records;
	BroadcastManager *m_broadcast;
	QTimer *m_timer;
	QFile m_notesFile;
	QList<QWidget *> m_inputFields;
	bool m_dialogOpen;
    bool m_dontStart;

	QDateTime m_lastStartRecordClicked;
	QDateTime m_lastStartBroadcastClicked;
	QDateTime m_lastTakeClicked;
    QDateTime m_lastKeyEvent;

	QDateTime m_lastTakeHappened;
};

#endif // FEGLIVEPRODUCER_H
