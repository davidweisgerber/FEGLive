#ifndef BACKGROUNDPROGRAMSELECTWIDGET_H
#define BACKGROUNDPROGRAMSELECTWIDGET_H

#include <QWidget>
#include <QList>

class SelectData;
class CasparConnection;

class BackgroundProgramSelectWidget : public QWidget
{
	Q_OBJECT

public:
	BackgroundProgramSelectWidget(QWidget *parent = 0);
	~BackgroundProgramSelectWidget();

	void addSelectData(SelectData *data);
	void setCasparConnection(CasparConnection *con);

protected:
	virtual void paintEvent(QPaintEvent *ev) override;
	virtual void mouseReleaseEvent(QMouseEvent *e) override;
	virtual void doAction(const QString &action);

private:
	QList<SelectData *> m_selectData;

	SelectData *m_selected;
	CasparConnection *m_con;
};

#endif // BACKGROUNDPROGRAMSELECTWIDGET_H
