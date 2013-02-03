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

public slots:
	void setBackgroundTransitionTime(int frames);
	void setBackgroundTransitionStyle(const QString &style);

protected:
	virtual void paintEvent(QPaintEvent *ev) override;
	virtual void mouseReleaseEvent(QMouseEvent *e) override;
	virtual void doAction(SelectData *data);

private:
	void performAction(const QString &action, const QString &command);

	QList<SelectData *> m_selectData;

	SelectData *m_selected;
	SelectData *m_default;
	CasparConnection *m_con;

	int m_backgroundTransitionTime;
	QString m_brackgroundTransitionStyle;
};

#endif // BACKGROUNDPROGRAMSELECTWIDGET_H
