#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include "casparconnection.h"
#include "lowerthirdstextselectwidget.h"

LowerThirdsTextSelectWidget::LowerThirdsTextSelectWidget(QWidget *parent)
	: QWidget(parent),
	m_icon(QPixmap(":FEGLiveProducer/lowerthirdtext.png")),
	m_offIcon(QPixmap(":FEGLiveProducer/off.png"))
{
	m_current = 0;
	m_con = 0;
	m_hasClipRunning = false;
}

LowerThirdsTextSelectWidget::~LowerThirdsTextSelectWidget()
{

}

void LowerThirdsTextSelectWidget::setTexts( const QList<LowerThirdsText> &texts )
{
	m_texts = texts;
	m_current = 0;
	goOff();
	update();
}

void LowerThirdsTextSelectWidget::paintEvent( QPaintEvent *ev )
{
	QPainter p(this);

	int x = 5;

	p.drawPixmap(x, 0, m_offIcon);
	x += m_offIcon.width() + 5;

	for (int i=0; i < m_texts.size(); i++) {
		if (i+1 == m_current) {
			p.setBrush(Qt::cyan);
			p.setPen(Qt::cyan);
			p.drawRoundedRect(x-2, 0, m_icon.width() + 4, size().height(), 5, 5);
			p.setPen(Qt::black);
		}

		p.drawPixmap(x, 0, m_icon);

		p.drawText(x-2, m_icon.height() + 2, m_icon.width() + 4, 100, Qt::AlignTop | Qt::AlignHCenter | Qt::TextWordWrap, m_texts[i].getLine1());

		x += m_icon.width() + 5;
	}

	p.drawPixmap(x, 0, m_offIcon);
}

void LowerThirdsTextSelectWidget::goOff()
{
	m_con->sendCommand("CG 1-2 STOP 1");
	m_hasClipRunning = false;
}

void LowerThirdsTextSelectWidget::mouseReleaseEvent( QMouseEvent *e )
{
	int x = 5;

	if (e->x() > x && e->x() < x + m_offIcon.width()) {
		m_current = 0;
		goOff();
		update();
		return;
	}

	x += m_offIcon.width() + 5;

	for (int i=0; i < m_texts.size(); i++) {
		if (e->x() > x && e->x() < x + m_icon.width()) {
			m_current = i+1;
			doAction(m_texts[i]);
			update();
			return;
		}

		x += m_icon.width() + 5;
	}

	if (e->x() > x && e->x() < x + m_offIcon.width()) {
		m_current = m_texts.size() + 2;
		goOff();
		update();
		return;
	}
}

void LowerThirdsTextSelectWidget::setLowerThird(LowerThird &lowerThird )
{
	setTexts(lowerThird.getTexts());
	m_movie = lowerThird.getMovie();
}

void LowerThirdsTextSelectWidget::doAction( const LowerThirdsText &text )
{
	if (m_hasClipRunning == false) {
		m_con->sendCommand("CG 1-2 ADD 1 " + m_movie + " 1");
		m_hasClipRunning = true;
	}

	m_con->sendCommand("CG 1-2 UPDATE 1 \"<templateData><componentData id=\\\"f0\\\"><data id=\\\"text\\\" value=\\\"" + text.getLine1() + "\\\"></data></componentData>" + 
		"<componentData id=\\\"f1\\\"><data id=\\\"text\\\" value=\\\"" + text.getLine2() + "\\\"></data></componentData>" +
		+ "</templateData>");
}

void LowerThirdsTextSelectWidget::setCasparConnection( CasparConnection *con )
{
	m_con = con;
}

void LowerThirdsTextSelectWidget::left()
{
	m_current--;

	if (m_current <= 0) 
	{
		m_current = 0;
		goOff();
	}
	else
	{
		doAction(m_texts[m_current-1]);
	}

	update();
}

void LowerThirdsTextSelectWidget::right()
{
	m_current++;

	if (m_current > m_texts.size())
	{
		m_current = m_texts.size() + 1;
		goOff();
	}
	else
	{
		doAction(m_texts[m_current-1]);
	}

	update();
}
