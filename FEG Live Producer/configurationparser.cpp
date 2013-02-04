#include <QFile>
#include <QScriptEngine>
#include <QScriptValue>
#include "songfileparser.h"
#include "configurationparser.h"

ConfigurationParser::ConfigurationParser()
{
	m_defaultClip = 0;
}

ConfigurationParser::~ConfigurationParser()
{

}

bool ConfigurationParser::parseFromFile( const QString &fileName )
{
	QFile f(fileName);

	if (f.open(QIODevice::ReadOnly) == false) {
		return false;
	}

	QString program = QString::fromUtf8(f.readAll());
	f.close();

	QScriptEngine engine;
	QScriptValue object = engine.evaluate("({" + program + "})");

	m_songLowerThird = SongFileParser::getPropertyString(object, "songTemplate");
	m_generalLowerThird = SongFileParser::getPropertyString(object, "generalTemplate");
	if (object.property("defaultClip").isNumber()) {
		m_defaultClip = object.property("defaultClip").toInt32();
	}

	int i=0;
	while (object.property("clips").property(i).isObject()) {
		SelectData *data = new SelectData(
			QPixmap(object.property("clips").property(i).property("icon").toString()),
			object.property("clips").property(i).property("name").toString(),
			object.property("clips").property(i).property("data").toString());
		m_programs.push_back(data);
		i++;
	}

	return true;
}

const QString & ConfigurationParser::getSongLowerThird() const
{
	return m_songLowerThird;
}

const QString & ConfigurationParser::getGeneralLowerThird() const
{
	return m_generalLowerThird;
}

const QList<SelectData *> & ConfigurationParser::getProgram() const
{
	return m_programs;
}

int ConfigurationParser::getDefaultClip() const
{
	return m_defaultClip;
}
