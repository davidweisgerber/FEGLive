#ifndef ADDLOWERTHIRDDIALOG_H
#define ADDLOWERTHIRDDIALOG_H

#include <QDialog>
namespace Ui {class AddLowerThirdDialog;};

class LowerThirdsSelectWidget;
class ConfigurationParser;

class AddLowerThirdDialog : public QDialog
{
	Q_OBJECT

public:
	AddLowerThirdDialog(LowerThirdsSelectWidget *widget, ConfigurationParser *config, QWidget *parent = 0);
	~AddLowerThirdDialog();

private slots:
	void addSong();
	void addCustom();
	void refreshLowerThirds();
	void existingLowerThirdChanged(int row);
	void saveLowerThird();
	void deleteLowerThird();

protected:
	void showEvent(QShowEvent *);

signals:
	void changed();

private:
	Ui::AddLowerThirdDialog *ui;

	LowerThirdsSelectWidget *m_widget;
	ConfigurationParser *m_config;
	QString m_songDir;
};

#endif // ADDLOWERTHIRDDIALOG_H
