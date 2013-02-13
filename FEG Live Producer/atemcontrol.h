#ifndef ATEMCONTROL_H
#define ATEMCONTROL_H

#include <QObject>
#include <QMap>
#include <QToolButton>

class QAtemConnection;
class QTimer;
class QComboBox;
class QSpinBox;

class ATEMControl : public QObject
{
	Q_OBJECT

public:
	class ButtonInfo {
	public:
		ButtonInfo(int input, bool program, const QString &name) {
			this->input = input;
			this->program = program;
			this->name = name;
		}

		ButtonInfo() {}

		int input;
		bool program;
		QString name;
	};


	ATEMControl(QObject *parent);
	~ATEMControl();

	void setButtonInfos(const QMap<QToolButton *, ButtonInfo> &buttonInfos);
	void addButtonInfo(QToolButton *, const ButtonInfo &buttonInfo);

	void setTransitionStyleComboBox(QComboBox *box);
	void setTransitionFramesSpinBox(QSpinBox *box);

	void start();
	void keyPressed(const QString &key);

public slots:
	void take();
	void transitionChanged(int transitionNo);
	void transitionFramesChanged(int frames);
	void autoTakeChanged(bool on);

private slots:
	void buttonPressed();
	void timerTick();

private:
	QMap<QToolButton *, ButtonInfo> m_buttonInfos;
	QAtemConnection *m_con;
	bool m_autoTake;
	QTimer *m_timer;
	QComboBox *m_transitionStyleBox;
	QSpinBox *m_transitionFramesBox;
};

#endif // ATEMCONTROL_H
