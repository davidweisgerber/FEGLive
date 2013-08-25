#include "lowerthird.h"

LowerThird::LowerThird(const QString &name, const QString &movie, const QList<LowerThirdsText> &texts) :
	m_name(name),
	m_movie(movie),
	m_texts(texts)
{

}

LowerThird::~LowerThird()
{

}

const QString & LowerThird::getName() const
{
	return m_name;
}

const QString & LowerThird::getMovie() const
{
	return m_movie;
}

QList<LowerThirdsText> & LowerThird::getTexts() 
{
	return m_texts;
}

void LowerThird::addText( const LowerThirdsText &text )
{
	m_texts.push_back(text);
}

void LowerThird::removeText(int index)
{
	m_texts.removeAt(index);
}
