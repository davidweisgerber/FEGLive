#ifndef LOWERTHIRDSTEXT_H
#define LOWERTHIRDSTEXT_H

#include <QString>

class LowerThirdsText
{
public:
	LowerThirdsText(const QString &line1, const QString &line2);
	~LowerThirdsText();

	const QString &getLine1() const;
	const QString &getLine2() const;

	void setLine1(const QString &line1);
	void setLine2(const QString &line2);

private:
	QString m_line1;
	QString m_line2;
};

#endif // LOWERTHIRDSTEXT_H
