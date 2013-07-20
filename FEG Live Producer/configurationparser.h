#ifndef CONFIGURATIONPARSER_H
#define CONFIGURATIONPARSER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMetaType>
#include "usertypeserializer.h"
#include "selectdata.h"
#include "lowerthirdstext.h"


Q_DECLARE_METATYPE(QList<SelectData *>)

class ConfigurationParser : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString logo READ getLogo)
	Q_PROPERTY(QString songLowerThird READ getSongLowerThird)
	Q_PROPERTY(QString generalLowerThird READ getGeneralLowerThird)
	Q_PROPERTY(QList<SelectData *> program READ getProgram)
	Q_PROPERTY(int defaultClip READ getDefaultClip)
	Q_PROPERTY(QString videoPath READ getVideoPath)
	Q_PROPERTY(QString audioPath READ getAudioPath)
	Q_PROPERTY(QString mediaPath READ getMediaPath)
	Q_PROPERTY(QString ffmpegPath READ getFfmpegPath)
	Q_PROPERTY(QString preacherLowerThirdTitle READ getPreacherLowerThirdTitle)
	Q_PROPERTY(QString topicLowerThirdTitle READ getTopicLowerThirdTitle)
	Q_PROPERTY(QString bibleTextLowerThirdTitle READ getBibleTextLowerThirdTitle)


public:

	class ListSelectDataSerializer : public UserTypeSerializer 
	{
	public:
		ListSelectDataSerializer();
		virtual QString serialize( QVariant variant );
		virtual QVariant deserialize( const QString json );
	};

	ConfigurationParser();
	~ConfigurationParser();

	bool parseFromFile(const QString &fileName);

	const QString &getSongLowerThird() const;
	const QString &getGeneralLowerThird() const;
	const QString &getLogo() const;
	const QList<SelectData *> &getProgram() const;
	int getDefaultClip() const;
	const QString &getVideoPath() const;
	const QString &getAudioPath() const;
	const QString &getMediaPath() const;
	const QString &getFfmpegPath() const;
	const QString &getPreacherLowerThirdTitle() const;
	const QString &getTopicLowerThirdTitle() const;
	const QString &getBibleTextLowerThirdTitle() const;
	QStringList getPreConfiguredSongs() const;
	const QStringList &getPreConfiguredLowerThirds() const;
	QList<LowerThirdsText> getPreConfiguredLowerThirdsList() const;


private:
	QString m_logo;
	QString m_songLowerThird;
	QString m_generalLowerThird;
	QList<SelectData *> m_programs;
	int m_defaultClip;
	QString m_videoPath;
	QString m_audioPath;
	QString m_mediaPath;
	QString m_ffmpegPath;
	QStringList m_preConfiguredSongs;
	QStringList m_preConfiguredLowerThirds;

	QString m_preacherLowerThirdTitle;
	QString m_topicLowerThirdTitle;
	QString m_bibleTextLowerThirdTitle;
};

#endif // CONFIGURATIONPARSER_H
