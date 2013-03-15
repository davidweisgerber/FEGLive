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

private:
	Ui::AddLowerThirdDialog *ui;

	LowerThirdsSelectWidget *m_widget;
	ConfigurationParser *m_config;
	QString m_songDir;
};

#endif // ADDLOWERTHIRDDIALOG_H
