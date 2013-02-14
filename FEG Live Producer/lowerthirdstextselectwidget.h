#ifndef LOWERTHIRDSTEXTSELECTWIDGET_H
#define LOWERTHIRDSTEXTSELECTWIDGET_H

#include <QWidget>
#include <QList>
#include <QPixmap>
#include "lowerthird.h"
#include "lowerthirdstext.h"

class CasparConnection;

class LowerThirdsTextSelectWidget : public QWidget
{
	Q_OBJECT

public:
	LowerThirdsTextSelectWidget(QWidget *parent = 0);
	~LowerThirdsTextSelectWidget();

	void setCasparConnection(CasparConnection *con);

public slots:
	void setLowerThird(const LowerThird &lowerThird);
	void left();
	void right();

protected:
	virtual void paintEvent(QPaintEvent *ev) override;
	virtual void mouseReleaseEvent(QMouseEvent *e) override;
	void doAction(const LowerThirdsText &text);
	void goOff();

private:
	void setTexts(const QList<LowerThirdsText> &texts);

	QList<LowerThirdsText> m_texts;
	int m_current;
	QPixmap m_icon;
	QPixmap m_offIcon;
	QString m_movie;

	bool m_hasClipRunning;
	CasparConnection *m_con;
};

#endif // LOWERTHIRDSTEXTSELECTWIDGET_H
