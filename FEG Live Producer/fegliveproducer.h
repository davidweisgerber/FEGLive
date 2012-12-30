#ifndef FEGLIVEPRODUCER_H
#define FEGLIVEPRODUCER_H

#include <QtGui/QMainWindow>
#include "ui_fegliveproducer.h"

class CasparConnection;

class FEGLiveProducer : public QMainWindow
{
	Q_OBJECT

public:
	FEGLiveProducer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~FEGLiveProducer();

private:
	Ui::FEGLiveProducerClass ui;
	CasparConnection *m_casparCon;
};

#endif // FEGLIVEPRODUCER_H
