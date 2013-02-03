#include <QFile>
#include <QDebug>
#include <QScriptValue>
#include <QScriptEngine>
#include "songfileparser.h"

SongFileParser::SongFileParser(QObject *parent)
	: QObject(parent)
{

}

SongFileParser::~SongFileParser()
{

}

bool SongFileParser::parseFromFile(const QString &fileName)
{
	QFile f(fileName);

	if (f.open(QIODevice::ReadOnly) == false) {
		return false;
	}

	QString program = QString::fromUtf8(f.readAll());
	f.close();

	QScriptEngine engine;
	QScriptValue object = engine.evaluate("({" + program + "})");

	m_name = getPropertyString(object, "name");
	m_author = getPropertyString(object, "author");
	m_copyright = getPropertyString(object, "copyright");
	m_slides = object.property("slides").toVariant().toStringList();
	
	if (m_slides.isEmpty()) {
		return false;
	}

	return true;
}

const QString & SongFileParser::getName() const
{
	return m_name;
}

const QString & SongFileParser::getAuthor() const
{
	return m_author;
}

const QString & SongFileParser::getCopyright() const
{
	return m_copyright;
}

const QStringList & SongFileParser::getSlides() const
{
	return m_slides;
}

QString SongFileParser::getPropertyString( const QScriptValue &script, const QString &propertyName )
{
	if (script.property(propertyName).isString()) {
		return script.property(propertyName).toString();
	}

	return "";
}

LowerThird SongFileParser::toLowerThird(const QString &movie) const
{
	QList<LowerThirdsText> texts;
	foreach (QString slide, m_slides) {
		QString line1;
		QString line2;

		QStringList lines = slide.split('\n');
		line1 = lines[0];

		if (lines.size() > 1) {
			line2 = lines[1];
		}

		texts.push_back(LowerThirdsText(line1, line2));
	}
	

	return LowerThird(m_name, movie, texts);
}
