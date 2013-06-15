#ifndef CONFIGURATIONPARSER_H
#define CONFIGURATIONPARSER_H

#include <QString>
#include <QList>
#include "selectdata.h"
#include "lowerthirdstext.h"

class ConfigurationParser
{
public:
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
