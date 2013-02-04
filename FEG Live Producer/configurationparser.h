#ifndef CONFIGURATIONPARSER_H
#define CONFIGURATIONPARSER_H

#include <QString>
#include <QList>
#include <selectdata.h>

class ConfigurationParser
{
public:
	ConfigurationParser();
	~ConfigurationParser();

	bool parseFromFile(const QString &fileName);

	const QString &getSongLowerThird() const;
	const QString &getGeneralLowerThird() const;
	const QList<SelectData *> &getProgram() const;
	int getDefaultClip() const;

private:
	QString m_songLowerThird;
	QString m_generalLowerThird;
	QList<SelectData *> m_programs;
	int m_defaultClip;
};

#endif // CONFIGURATIONPARSER_H
