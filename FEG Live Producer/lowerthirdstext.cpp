#include "lowerthirdstext.h"

LowerThirdsText::LowerThirdsText(const QString &line1, const QString &line2) :
	m_line1(line1),
	m_line2(line2)
{

}

LowerThirdsText::~LowerThirdsText()
{

}

const QString & LowerThirdsText::getLine1() const
{
	return m_line1;
}

const QString & LowerThirdsText::getLine2() const
{
	return m_line2;
}
