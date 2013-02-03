#ifndef SONGFILEPARSER_H
#define SONGFILEPARSER_H

#include <QObject>
#include <QStringList>
#include "lowerthird.h"

class QScriptValue;

class SongFileParser : public QObject
{
	Q_OBJECT

public:
	SongFileParser(QObject *parent=0);
	~SongFileParser();

	bool parseFromFile(const QString &fileName);

	const QString &getName() const;
	const QString &getAuthor() const;
	const QString &getCopyright() const;
	const QStringList &getSlides() const;
	LowerThird toLowerThird(const QString &movie) const;

private:
	QString getPropertyString(const QScriptValue &script, const QString &propertyName);

	QString m_name;
	QString m_author;
	QString m_copyright;
	QStringList m_slides;
};

#endif // SONGFILEPARSER_H
