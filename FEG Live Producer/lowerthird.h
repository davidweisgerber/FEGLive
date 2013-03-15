#ifndef LOWERTHIRD_H
#define LOWERTHIRD_H

#include <QString>
#include <QList>
#include "lowerthirdstext.h"

class LowerThird
{
public:
	LowerThird(const QString &name, const QString &movie, const QList<LowerThirdsText> &texts);
	~LowerThird();

	const QString &getName() const;
	const QString &getMovie() const;
	const QList<LowerThirdsText> &getTexts() const;

	void addText(const LowerThirdsText &text);

private:
	QString m_name;
	QString m_movie;
	QList<LowerThirdsText> m_texts;
};

#endif // LOWERTHIRD_H
