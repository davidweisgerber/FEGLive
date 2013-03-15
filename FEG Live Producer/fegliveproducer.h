#ifndef FEGLIVEPRODUCER_H
#define FEGLIVEPRODUCER_H

#include <QtGui/QMainWindow>
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
	FEGLiveProducer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~FEGLiveProducer();

protected:
	virtual bool eventFilter(QObject *watched, QEvent *event);

private slots:
	void updateStuff();
	void recordClicked();
	void logoClicked();
	void saveRecordsClicked();
	void addLowerThird();
	void moveCasparCgOglWindow();
	void toggleSecondMonitor();
	void notesChanged();
	
private:
	void doNastyStuff();
	Ui::FEGLiveProducerClass ui;
	CasparConnection *m_casparCon;
	StartDialog *m_startDialog;
	ConfigurationParser *m_config;
	ATEMControl *m_atem;
	RecordManager *m_records;
	BroadcastManager *m_broadcast;
	QTimer *m_timer;
	QFile m_notesFile;
};

#endif // FEGLIVEPRODUCER_H
