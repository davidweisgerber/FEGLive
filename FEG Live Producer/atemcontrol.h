#ifndef ATEMCONTROL_H
#define ATEMCONTROL_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QToolButton>

class QAtemConnection;
class QTimer;
class QComboBox;
class QSpinBox;
class QToolButton;

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

	void setHardButton(QToolButton *button);
	void setMixButton(QToolButton *button);
	void setDipButton(QToolButton *button);
	void setWipeButton(QToolButton *button);

	void setMixSpinBox(QSpinBox *spinBox);
	void setDipSpinBox(QSpinBox *spinBox);
	void setWipeSpinBox(QSpinBox *spinBox);

	void setDipColourBox(QComboBox *comboBox);

	void start();
	void keyPressed(const QString &key);

public slots:
	void take();
	void autoTakeChanged(bool on);

private slots:
	void buttonPressed();
	void timerTick();
	void transitionChanged();
	void dipColourChanged(int index);
	void transitionFramesChanged(int frames);

private:
	void addAndConnectTransitionButton(QToolButton *button);
	void autoTake(int button);
	QMap<QToolButton *, ButtonInfo> m_buttonInfos;
	QAtemConnection *m_con;
	bool m_autoTake;
	QTimer *m_timer;
	QList<QToolButton *> m_buttons;
	QToolButton *m_hardButton;
	QToolButton *m_mixButton;
	QToolButton *m_dipButton;
	QToolButton *m_wipeButton;
	QSpinBox *m_mixSpinBox;
	QSpinBox *m_dipSpinBox;
	QSpinBox *m_wipeSpinBox;
	QComboBox *m_dipColourBox;
};

#endif // ATEMCONTROL_H
