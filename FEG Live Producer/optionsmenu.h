#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include <QDialog>
namespace Ui {class OptionsMenu;};

class OptionsMenu : public QDialog
{
	Q_OBJECT

public:
	OptionsMenu(QWidget *parent = 0);
	~OptionsMenu();

private:
	Ui::OptionsMenu *ui;
};

#endif // OPTIONSMENU_H
