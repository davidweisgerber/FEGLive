#ifndef LOWERTHIRDSSELECTWIDGET_H
#define LOWERTHIRDSSELECTWIDGET_H

#include <QWidget>
#include <QList>
#include <QPixmap>
#include "lowerthird.h"

class LowerThirdsSelectWidget : public QWidget
{
	Q_OBJECT

public:
	LowerThirdsSelectWidget(QWidget *parent = 0);
	~LowerThirdsSelectWidget();

	void addLowerThird(const LowerThird &lowerThird);
	LowerThird *getGeneralLowerThird();

public slots:
	void next();
	void previous();
	void updateGeneral();

protected:
	virtual void paintEvent(QPaintEvent *ev) override;
	virtual void mouseReleaseEvent(QMouseEvent *e) override;
	virtual void keyPressEvent(QKeyEvent *ev) override;
	virtual void keyReleaseEvent(QKeyEvent *ev) override;

signals:
	void lowerThirdChanged(LowerThird &lowerThird);

private:
	QList<LowerThird> m_lowerThirds;
	int m_current;

	QPixmap m_icon;
};

#endif // LOWERTHIRDSSELECTWIDGET_H
