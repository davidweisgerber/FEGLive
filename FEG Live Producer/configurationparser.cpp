#include <QFile>
#include <QScriptEngine>
#include <QScriptValue>
#include <QSettings>
#include "songfileparser.h"
#include "jsonserializer.h"
#include "configurationparser.h"

ConfigurationParser::ConfigurationParser() : QObject()
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

    m_recordOptions = SongFileParser::getPropertyString(object, "recordOptions");
    m_streamingOptions = object.property("streamingOptions").toVariant().toStringList();

    m_preConfiguredLowerThirds = object.property("lowerThirds").toVariant().toStringList();
	m_preConfiguredSongs = object.property("songs").toVariant().toStringList();

	if (object.property("defaultClip").isNumber()) {
		m_defaultClip = object.property("defaultClip").toInt32();
	}

	int i=0;
	while (object.property("clips").property(i).isObject()) {
		SelectData *data = new SelectData(
			QPixmap(object.property("clips").property(i).property("icon").toString()),
			object.property("clips").property(i).property("name").toString(),
			object.property("clips").property(i).property("data").toString(),
			object.property("clips").property(i).property("icon").toString());
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

QString ConfigurationParser::ListSelectDataSerializer::serialize(QVariant variant)
{
	QList<SelectData *> selectDatas = variant.value<QList<SelectData *> >();

	QString ret = "[";
	foreach (SelectData *selectData, selectDatas)
	{
		ret += "{\"icon\":\"" + selectData->getIconName() + "\",";
		ret += "\"name\":\"" + selectData->getName() + "\",";
		ret += "\"data:\":\"" + selectData->getData() + "\"},";
	}

	JSONSerializer::sanitizeList(&ret, selectDatas.size(), "]");
	return ret;
}

QVariant ConfigurationParser::ListSelectDataSerializer::deserialize(const QScriptValue &script)
{
	QList<SelectData *> selectData;
	for (int i=0; script.property(i).isObject(); i++) 
	{
		SelectData *data = new SelectData(
			QPixmap(script.property(i).property("icon").toString()),
			script.property(i).property("name").toString(),
			script.property(i).property("data").toString(),
			script.property(i).property("icon").toString());
		selectData.append(data);
	}

	return QVariant::fromValue(selectData);
}

QStringList ConfigurationParser::getPreConfiguredSongs() const
{
	QSettings settings;
	QString songDir = settings.value("songdir", "songs/").toString();

	QStringList ret;
	foreach (QString item, m_preConfiguredSongs) {
		ret << songDir + "/" + item + ".js";
	}

	return ret;
}

const QStringList & ConfigurationParser::getPreConfiguredLowerThirds() const
{
	return m_preConfiguredLowerThirds;
}

QList<LowerThirdsText> ConfigurationParser::getPreConfiguredLowerThirdsList() const
{
	QList<LowerThirdsText> returnValue;
	foreach (QString lowerThirdsText, m_preConfiguredLowerThirds) {
		QStringList lines = lowerThirdsText.split("\n");
		while (lines.count() < 2) {
			lines.append("");
		}

		returnValue.append(LowerThirdsText(lines[0], lines[1]));
	}

	return returnValue;
}

QString ConfigurationParser::getRecordOptions() const
{
    return m_recordOptions;
}

void ConfigurationParser::setRecordOptions(const QString &recordOptions)
{
    m_recordOptions = recordOptions;
}

QStringList ConfigurationParser::getStreamingOptions() const
{
    return m_streamingOptions;
}

void ConfigurationParser::setStreamingOptions(const QStringList &streamingOptions)
{
    m_streamingOptions = streamingOptions;
}

void ConfigurationParser::setSongLowerThird( const QString &lowerThird )
{
    m_songLowerThird = lowerThird;
}

void ConfigurationParser::setGeneralLowerThird( const QString &lowerThird )
{
    m_generalLowerThird = lowerThird;
}

void ConfigurationParser::setLogo( const QString &logo )
{
	m_logo = logo;
}

void ConfigurationParser::setProgram(const QList<SelectData *> &program)
{
	m_programs = program;
}

void ConfigurationParser::setDefaultClip(int defaultClip )
{
	m_defaultClip = defaultClip;
}

void ConfigurationParser::setVideoPath( const QString &videoPath )
{
	m_videoPath = videoPath;
}

void ConfigurationParser::setAudioPath( const QString &audioPath )
{
	m_audioPath = audioPath;
}

void ConfigurationParser::setMediaPath( const QString &mediaPath )
{
	m_mediaPath = mediaPath;
}

void ConfigurationParser::setFfmpegPath( const QString &ffmpegPath )
{
	m_ffmpegPath = ffmpegPath;
}

void ConfigurationParser::setPreacherLowerThirdTitle( const QString &preacherLowerThirdTitle )
{
	m_preacherLowerThirdTitle = preacherLowerThirdTitle;
}

void ConfigurationParser::setTopicLowerThirdTitle( const QString &topicLowerThird )
{
	m_topicLowerThirdTitle = topicLowerThird;
}

void ConfigurationParser::setBibleTextLowerThirdTitle( const QString &lowerThird )
{
	m_bibleTextLowerThirdTitle = lowerThird;
}

void ConfigurationParser::setPreConfiguredSongs( const QStringList &songs )
{
	m_preConfiguredSongs = songs;
}

void ConfigurationParser::setPreConfiguredLowerThirds( const QStringList &lowerThird )
{
	m_preConfiguredLowerThirds = lowerThird;
}
