#include "fegliveproducer.h"
#include "configurationparser.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ConfigurationParser::ListSelectDataSerializer serializer;
	
	FEGLiveProducer w;
    if (w.isAbortStart() == true)
    {
        return 0;
    }

	w.show();
	return a.exec();
}
