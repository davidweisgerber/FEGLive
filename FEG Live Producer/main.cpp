#include "fegliveproducer.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FEGLiveProducer w;
	w.show();
	return a.exec();
}
