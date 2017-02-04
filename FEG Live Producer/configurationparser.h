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
	Q_PROPERTY(QString logoTemplate READ getLogo WRITE setLogo)
	Q_PROPERTY(QString songTemplate READ getSongLowerThird WRITE setSongLowerThird)
	Q_PROPERTY(QString generalTemplate READ getGeneralLowerThird WRITE setGeneralLowerThird)
	Q_PROPERTY(QList<SelectData *> clips READ getProgram WRITE setProgram)
	Q_PROPERTY(int defaultClip READ getDefaultClip WRITE setDefaultClip)
	Q_PROPERTY(QString videoPath READ getVideoPath WRITE setVideoPath)
	Q_PROPERTY(QString audioPath READ getAudioPath WRITE setAudioPath)
	Q_PROPERTY(QString casparcgMediaPath READ getMediaPath WRITE setMediaPath)
	Q_PROPERTY(QString ffmpegPath READ getFfmpegPath WRITE setFfmpegPath)
	Q_PROPERTY(QString preacherLowerThirdTitle READ getPreacherLowerThirdTitle WRITE setPreacherLowerThirdTitle)
	Q_PROPERTY(QString topicLowerThirdTitle READ getTopicLowerThirdTitle WRITE setTopicLowerThirdTitle)
	Q_PROPERTY(QString bibleTextLowerThirdTitle READ getBibleTextLowerThirdTitle WRITE setBibleTextLowerThirdTitle)
	Q_PROPERTY(QStringList preConfiguredSongs READ getPreConfiguredSongs WRITE setPreConfiguredSongs)
	Q_PROPERTY(QStringList preConfiguredLowerThirds READ getPreConfiguredLowerThirds WRITE setPreConfiguredLowerThirds)
    Q_PROPERTY(QString recordOptions READ getRecordOptions WRITE setRecordOptions)
    Q_PROPERTY(QStringList streamingOptions READ getStreamingOptions WRITE setStreamingOptions)

public:

	class ListSelectDataSerializer : public UserTypeSerializer 
	{
	public:
		ListSelectDataSerializer() {}
		virtual QString serialize( QVariant variant );
		virtual QVariant deserialize(const QScriptValue &script);
	};

	ConfigurationParser();
	~ConfigurationParser();

	bool parseFromFile(const QString &fileName);

	const QString &getSongLowerThird() const;
	void setSongLowerThird(const QString &lowerThird);
	
	const QString &getGeneralLowerThird() const;
	void setGeneralLowerThird(const QString &lowerThird);

	const QString &getLogo() const;
	void setLogo(const QString &lowerThird);

	const QList<SelectData *> &getProgram() const;
	void setProgram(const QList<SelectData *> &program);

	int getDefaultClip() const;
	void setDefaultClip(int lowerThird);

	const QString &getVideoPath() const;
	void setVideoPath(const QString &lowerThird);

	const QString &getAudioPath() const;
	void setAudioPath(const QString &lowerThird);

	const QString &getMediaPath() const;
	void setMediaPath(const QString &lowerThird);

	const QString &getFfmpegPath() const;
	void setFfmpegPath(const QString &lowerThird);

	const QString &getPreacherLowerThirdTitle() const;
	void setPreacherLowerThirdTitle(const QString &lowerThird);

	const QString &getTopicLowerThirdTitle() const;
	void setTopicLowerThirdTitle(const QString &lowerThird);

	const QString &getBibleTextLowerThirdTitle() const;
	void setBibleTextLowerThirdTitle(const QString &lowerThird);

	QStringList getPreConfiguredSongs() const;
	void setPreConfiguredSongs(const QStringList &lowerThird);

	const QStringList &getPreConfiguredLowerThirds() const;
	void setPreConfiguredLowerThirds(const QStringList &lowerThird);

	QList<LowerThirdsText> getPreConfiguredLowerThirdsList() const;

    QString getRecordOptions() const;
    void setRecordOptions(const QString &recordOptions);

    QStringList getStreamingOptions() const;
    void setStreamingOptions(const QStringList &streamingOptions);

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
    QString m_recordOptions;
    QStringList m_streamingOptions;
	QStringList m_preConfiguredSongs;
	QStringList m_preConfiguredLowerThirds;

	QString m_preacherLowerThirdTitle;
	QString m_topicLowerThirdTitle;
	QString m_bibleTextLowerThirdTitle;
};

#endif // CONFIGURATIONPARSER_H
