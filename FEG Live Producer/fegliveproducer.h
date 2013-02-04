#ifndef FEGLIVEPRODUCER_H
#define FEGLIVEPRODUCER_H

#include <QtGui/QMainWindow>
#include "ui_fegliveproducer.h"

class CasparConnection;
class StartDialog;
class ConfigurationParser;

class FEGLiveProducer : public QMainWindow
{
	Q_OBJECT

public:
	FEGLiveProducer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~FEGLiveProducer();

private:
	Ui::FEGLiveProducerClass ui;
	CasparConnection *m_casparCon;
	StartDialog *m_startDialog;
	ConfigurationParser *m_config;
};

#endif // FEGLIVEPRODUCER_H
