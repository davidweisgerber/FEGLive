#ifndef FEGLIVEPRODUCER_H
#define FEGLIVEPRODUCER_H

#include <QtGui/QMainWindow>
#include "ui_fegliveproducer.h"

class CasparConnection;
class StartDialog;
class ConfigurationParser;
class ATEMControl;
class QKeyEvent;
class RecordManager;
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

private:
	void doNastyStuff();
	void moveCasparCgOglWindow();
	Ui::FEGLiveProducerClass ui;
	CasparConnection *m_casparCon;
	StartDialog *m_startDialog;
	ConfigurationParser *m_config;
	ATEMControl *m_atem;
	RecordManager *m_records;
	QTimer *m_timer;
};

#endif // FEGLIVEPRODUCER_H
