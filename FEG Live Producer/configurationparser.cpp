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

	m_preacherLowerThirdTitle = SongFileParser::getPropertyString(object, "preacherLowerThirdTitle");
	m_topicLowerThirdTitle = SongFileParser::getPropertyString(object, "topicLowerThirdTitle");
	m_bibleTextLowerThirdTitle = SongFileParser::getPropertyString(object, "bibleTextLowerThirdTitle");

	m_ffmpegPath = SongFileParser::getPropertyString(object, "ffmpegPath");
	m_mediaPath = SongFileParser::getPropertyString(object, "casparcgMediaPath");
	m_audioPath = SongFileParser::getPropertyString(object, "audioPath");
	m_videoPath = SongFileParser::getPropertyString(object, "videoPath");
	m_logo = SongFileParser::getPropertyString(object, "logoTemplate");
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

const QString & ConfigurationParser::getLogo() const
{
	return m_logo;
}

const QString &ConfigurationParser::getVideoPath() const
{
	return m_videoPath;
}

const QString &ConfigurationParser::getAudioPath() const
{
	return m_audioPath;
}

const QString & ConfigurationParser::getMediaPath() const
{
	return m_mediaPath;
}

const QString & ConfigurationParser::getFfmpegPath() const
{
	return m_ffmpegPath;
}

const QString & ConfigurationParser::getPreacherLowerThirdTitle() const
{
	return m_preacherLowerThirdTitle;
}

const QString & ConfigurationParser::getTopicLowerThirdTitle() const
{
	return m_topicLowerThirdTitle;
}

const QString & ConfigurationParser::getBibleTextLowerThirdTitle() const
{
	return m_bibleTextLowerThirdTitle;
}
