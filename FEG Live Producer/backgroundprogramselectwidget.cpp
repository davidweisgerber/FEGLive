#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QDebug>
#include "casparconnection.h"
#include "selectdata.h"
#include "backgroundprogramselectwidget.h"

BackgroundProgramSelectWidget::BackgroundProgramSelectWidget(QWidget *parent)
	: QWidget(parent)
{
	m_selectData.push_back(new SelectData(QPixmap(":FEGLiveProducer/camera.png"), "Cameras", "camera"));
	m_selectData.push_back(new SelectData(QPixmap(":FEGLiveProducer/video.png"), "AMB", "file:AMB"));
	m_selectData.push_back(new SelectData(QPixmap(":FEGLiveProducer/video.png"), "CG 1080i50", "file:CG1080i50"));
	m_selectData.push_back(new SelectData(QPixmap(":FEGLiveProducer/video.png"), "GO 1080p", "file:GO1080p25"));
	m_selectData.push_back(new SelectData(QPixmap(":FEGLiveProducer/off.png"), "Off", "off"));

	m_selected = m_selectData.at(0);
}

BackgroundProgramSelectWidget::~BackgroundProgramSelectWidget()
{

}

void BackgroundProgramSelectWidget::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	int x = 5;

	foreach (SelectData *item, m_selectData) {
		if (item == m_selected) {
			p.setBrush(Qt::cyan);
			p.setPen(Qt::cyan);
			p.drawRoundedRect(x-2, 0, item->getIcon().width() + 4, size().height(), 5, 5);
			p.setPen(Qt::black);
		}

		p.drawPixmap(x, 0, item->getIcon());
		
		p.drawText(x-2, item->getIcon().height() + 2, item->getIcon().width() + 4, 100, Qt::AlignTop | Qt::AlignHCenter | Qt::TextWordWrap, item->getName());

		x += item->getIcon().width() + 5;
	}
}

void BackgroundProgramSelectWidget::addSelectData(SelectData *data)
{
	m_selectData.push_back(data);
	update();
}

void BackgroundProgramSelectWidget::mouseReleaseEvent(QMouseEvent *e)
{
	int x = 5;

	foreach (SelectData *item, m_selectData) {
		if (e->x() > x && e->x() < x + item->getIcon().width()) {
			m_selected = item;
			doAction(item->getData());
			update();
			break;
		}

		x += item->getIcon().width() + 5;
	}
}

void BackgroundProgramSelectWidget::doAction(const QString &action)
{
	qDebug() << action;

	if (action.startsWith("file:")) {
		QString filename = action.mid(5);
		QString command = "PLAY 1-1 " + filename;
		m_con->sendCommand(command);
	} else if (action == "off") {
		m_con->sendCommand("CLEAR 1-1");
	}
}

void BackgroundProgramSelectWidget::setCasparConnection(CasparConnection *con)
{
	m_con = con;
}
