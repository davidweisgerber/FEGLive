#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QDebug>
#include "casparconnection.h"
#include "selectdata.h"
#include "backgroundprogramselectwidget.h"

BackgroundProgramSelectWidget::BackgroundProgramSelectWidget(QWidget *parent)
	: QWidget(parent),
	m_backgroundTransitionTime(0)
{
	m_selected = 0;
	m_default = 0;
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

		if (item == m_default) {
			p.setBrush(Qt::yellow);
			p.setPen(Qt::yellow);
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
			doAction(item);
			update();
			break;
		}

		x += item->getIcon().width() + 5;
	}
}

void BackgroundProgramSelectWidget::doAction(SelectData *data)
{
	performAction(data->getData(), "PLAY");

	if (data != m_default) {
		performAction(m_default->getData(), "LOADBG");
	}
}

void BackgroundProgramSelectWidget::setCasparConnection(CasparConnection *con)
{
	m_con = con;
}

void BackgroundProgramSelectWidget::setBackgroundTransitionTime(int frames)
{
	m_backgroundTransitionTime = frames;
}

void BackgroundProgramSelectWidget::setBackgroundTransitionStyle(const QString &style)
{
	m_brackgroundTransitionStyle = style;
}

void BackgroundProgramSelectWidget::performAction( const QString &action, const QString &command )
{
	QString sourceName;
	QString loop;
	QString autoStart;
	QString transitionStyle = m_brackgroundTransitionStyle;
	int transitionTime = m_backgroundTransitionTime;

	if (transitionStyle == "CUT") {
		transitionTime = 0;
	}

	if (command == "LOADBG") {
		autoStart = "AUTO";
	}

	QStringList commandParameters = action.split("|");

	foreach (QString param, commandParameters) {
		if (param.startsWith("file:")) {
			sourceName = param.mid(5);
		} else if (param == "source:off") {
			sourceName = "EMPTY";
		} else if (param == "LOOP") {
			loop = "LOOP";
		}
	}

	QString outputCommand = QString("%1 1-1 %2 %3 %4 %5 %6").arg(command).arg(sourceName).arg(loop).arg(transitionStyle).arg(transitionTime).arg(autoStart);
	m_con->sendCommand(outputCommand);
}

void BackgroundProgramSelectWidget::setDefault( SelectData *data )
{
	m_default = data;
}
