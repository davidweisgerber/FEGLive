#ifndef FEGLIVEPRODUCER_H
#define FEGLIVEPRODUCER_H

#include <QtGui/QMainWindow>
#include "ui_fegliveproducer.h"

class CasparConnection;
class StartDialog;
class ConfigurationParser;
class ATEMControl;
class QKeyEvent;

class FEGLiveProducer : public QMainWindow
{
	Q_OBJECT

public:
	FEGLiveProducer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~FEGLiveProducer();

protected:
	virtual bool eventFilter(QObject *watched, QEvent *event);

private:
	Ui::FEGLiveProducerClass ui;
	CasparConnection *m_casparCon;
	StartDialog *m_startDialog;
	ConfigurationParser *m_config;
	ATEMControl *m_atem;
};

#endif // FEGLIVEPRODUCER_H
