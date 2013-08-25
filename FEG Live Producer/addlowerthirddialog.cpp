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
	connect(ui->existingWidget, SIGNAL(currentRowChanged(int)), this, SLOT(existingLowerThirdChanged(int)));
	connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(deleteLowerThird()));
	connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveLowerThird()));
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
	m_widget->update();
}

void AddLowerThirdDialog::addCustom()
{
	m_widget->getGeneralLowerThird()->addText(LowerThirdsText("", ""));
	
	refreshLowerThirds();
	ui->existingWidget->setCurrentRow(ui->existingWidget->count() - 1);
	m_widget->updateGeneral();
}

void AddLowerThirdDialog::refreshLowerThirds()
{
	ui->existingWidget->clear();

	foreach (LowerThirdsText text, m_widget->getGeneralLowerThird()->getTexts())
	{
		ui->existingWidget->addItem(text.getLine1() + "---" + text.getLine2());
	}
}

void AddLowerThirdDialog::existingLowerThirdChanged(int row)
{
	if (row >= 0 && row < m_widget->getGeneralLowerThird()->getTexts().size())
	{
		ui->line1Edit->setText(m_widget->getGeneralLowerThird()->getTexts().at(row).getLine1());
		ui->line2Edit->setText(m_widget->getGeneralLowerThird()->getTexts().at(row).getLine2());
	}
	else
	{
		ui->line1Edit->setText("");
		ui->line2Edit->setText("");
	}
}

void AddLowerThirdDialog::saveLowerThird()
{
	int row = ui->existingWidget->currentRow();

	if (m_widget->getGeneralLowerThird()->getTexts().size() > row && row >= 0) 
	{
		m_widget->getGeneralLowerThird()->getTexts()[row].setLine1(ui->line1Edit->text());
		m_widget->getGeneralLowerThird()->getTexts()[row].setLine2(ui->line2Edit->text());
		m_widget->updateGeneral();
		refreshLowerThirds();

		ui->existingWidget->setCurrentRow(row);
	}
}

void AddLowerThirdDialog::deleteLowerThird()
{
	int row = ui->existingWidget->currentRow();

	if (m_widget->getGeneralLowerThird()->getTexts().size() > row && row >= 0) 
	{
		m_widget->getGeneralLowerThird()->getTexts().removeAt(row);
		m_widget->updateGeneral();
		refreshLowerThirds();

		ui->existingWidget->setCurrentRow(row - 1);
	}
}

void AddLowerThirdDialog::showEvent( QShowEvent * )
{
	refreshLowerThirds();

	if (ui->existingWidget->count() > 0)
	{
		ui->existingWidget->setCurrentRow(0);
	}
}
