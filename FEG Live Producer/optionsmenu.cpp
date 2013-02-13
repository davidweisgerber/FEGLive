#include "optionsmenu.h"
#include "ui_optionsmenu.h"

OptionsMenu::OptionsMenu(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::OptionsMenu();
	ui->setupUi(this);
}

OptionsMenu::~OptionsMenu()
{
	delete ui;
}

