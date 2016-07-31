#include "fegliveproducer.h"
#include "configurationparser.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ConfigurationParser::ListSelectDataSerializer serializer;
	
	FEGLiveProducer w;
	w.show();
	return a.exec();
}
