#include <QPainter>
#include <QMouseEvent>
#include "lowerthirdsselectwidget.h"

LowerThirdsSelectWidget::LowerThirdsSelectWidget(QWidget *parent)
	: QWidget(parent),
	m_icon(QPixmap(":FEGLiveProducer/lowerthird.png"))
{
	m_current = -1;
}

LowerThirdsSelectWidget::~LowerThirdsSelectWidget()
{

}

void LowerThirdsSelectWidget::addLowerThird(const LowerThird &lowerThird)
{
	m_lowerThirds.push_back(lowerThird);
	update();
}

void LowerThirdsSelectWidget::paintEvent( QPaintEvent *ev )
{
	QPainter p(this);

	int x = 5;

	for (int i=0; i < m_lowerThirds.size(); i++) {
		if (i == m_current) {
			p.setBrush(Qt::cyan);
			p.setPen(Qt::cyan);
			p.drawRoundedRect(x-2, 0, m_icon.width() + 4, size().height(), 5, 5);
			p.setPen(Qt::black);
		}

		p.drawPixmap(x, 0, m_icon);

		p.drawText(x-2, m_icon.height() + 2, m_icon.width() + 4, 100, Qt::AlignTop | Qt::AlignHCenter | Qt::TextWordWrap, m_lowerThirds[i].getName());

		x += m_icon.width() + 5;
	}
}

void LowerThirdsSelectWidget::mouseReleaseEvent(QMouseEvent *e)
{
	int x = 5;

	for (int i=0; i < m_lowerThirds.size(); i++) {
		if (e->x() > x && e->x() < x + m_icon.width()) {
			m_current = i;
			emit lowerThirdChanged(m_lowerThirds[i]);
			update();
			break;
		}

		x += m_icon.width() + 5;
	}
}
