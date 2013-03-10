#include "startdialog.h"
#include <QSettings>
#include <QDir>
#include "ui_startdialog.h"

StartDialog::StartDialog(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::StartDialog();
	ui->setupUi(this);

	ui->dateEdit->setDate(QDate::currentDate());
	ui->configurationLabel->setText("default.js");

	QSettings settings;
	m_songDir = settings.value("songdir", "songs/").toString();
	QDir dir(m_songDir);
	QStringList list = dir.entryList(QStringList() << "*.js");
	foreach (QString string, list) {
		ui->songSelectWidget->addItem(string.mid(0, string.length() - 3));
	}
}

StartDialog::~StartDialog()
{
	delete ui;
	ui = 0;
}

QDate StartDialog::getDate() const
{
	return ui->dateEdit->date();
}

TimeType StartDialog::getTimeType() const
{
	return static_cast<TimeType>(ui->timeComboBox->currentIndex() + 1);
}

QString StartDialog::getPreacher() const
{
	return ui->preacherEdit->text();
}

QString StartDialog::getSermonTitle() const
{
	return ui->sermonEdit->text();
}

QString StartDialog::getBibleText() const
{
	return ui->bibleEdit->text();
}

QString StartDialog::getConfigurationFileName() const
{
	return ui->configurationLabel->text();
}

QStringList StartDialog::getSongs() const
{
	QStringList ret;
	foreach (QListWidgetItem *item, ui->songSelectWidget->selectedItems()) {
		ret << m_songDir + "/" + item->text() + ".js";
	}

	return ret;
}

