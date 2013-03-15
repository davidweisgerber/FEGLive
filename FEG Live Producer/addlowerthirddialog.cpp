#include <QSettings>
#include <QDir>
#include <QStringList>
#include "addlowerthirddialog.h"
#include "lowerthirdsselectwidget.h"
#include "configurationparser.h"
#include "songfileparser.h"
#include "ui_addlowerthirddialog.h"

AddLowerThirdDialog::AddLowerThirdDialog(LowerThirdsSelectWidget *widget, ConfigurationParser *config, QWidget *parent)
	: QDialog(parent),
	m_widget(widget),
	m_config(config)
{
	ui = new Ui::AddLowerThirdDialog();
	ui->setupUi(this);

	QSettings settings;
	m_songDir = settings.value("songdir", "songs/").toString();
	QDir dir(m_songDir);
	QStringList list = dir.entryList(QStringList() << "*.js");
	foreach (QString string, list) {
		ui->listWidget->addItem(string.mid(0, string.length() - 3));
	}

	connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addSong()));
	connect(ui->addCustomButton, SIGNAL(clicked()), this, SLOT(addCustom()));
}

AddLowerThirdDialog::~AddLowerThirdDialog()
{
	delete ui;
}

void AddLowerThirdDialog::addSong()
{
	if (ui->listWidget->selectedItems().size() == 0)
	{
		return;
	}

	SongFileParser songFile;
	songFile.parseFromFile(m_songDir + "/" + ui->listWidget->selectedItems()[0]->text() + ".js");

	m_widget->addLowerThird(songFile.toLowerThird(m_config->getSongLowerThird()));
}

void AddLowerThirdDialog::addCustom()
{
	m_widget->getGeneralLowerThird()->addText(LowerThirdsText(ui->line1Edit->text(), ui->line2Edit->text()));
}

