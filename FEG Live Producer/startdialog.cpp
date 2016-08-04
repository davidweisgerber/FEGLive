#include "startdialog.h"
#include <QSettings>
#include <QDir>
#include <QTranslator>
#include <QFileDialog>
#include <QMessageBox>
#include "ui_startdialog.h"

StartDialog::StartDialog(QWidget *parent)
	: QDialog(parent)
{
	m_translationDE = new QTranslator();
	m_translationEN = new QTranslator();

	m_translationDE->load(":/FEGLiveProducer/de");
	m_translationEN->load(":/FEGLiveProducer/en");

	qApp->installTranslator(m_translationDE);

	ui = new Ui::StartDialog();
	ui->setupUi(this);

	ui->dateEdit->setDate(QDate::currentDate());
	ui->configurationLabel->setText("config/default.js");

	QSettings settings;
	m_songDir = settings.value("songdir", "songs/").toString();
	QDir dir(m_songDir);
	QStringList list = dir.entryList(QStringList() << "*.js");
	foreach (QString string, list) {
		ui->songSelectWidget->addItem(string.mid(0, string.length() - 3));
	}

	connect(ui->languageComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(languageChanged(int)));
	connect(ui->changeConfigurationButton, SIGNAL(clicked()), this, SLOT(selectConfigurationButtonClicked()));
    connect(ui->okButton, &QAbstractButton::clicked, this, &StartDialog::okClicked);
    m_changingLanguage = false;
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

void StartDialog::languageChanged(int index)
{
	if (m_changingLanguage)
	{
		return;
	}

	qApp->removeTranslator(m_translationDE);
	qApp->removeTranslator(m_translationEN);

	if (index == 0)
	{
		qApp->installTranslator(m_translationDE);
	}
	else if (index == 1)
	{
		qApp->installTranslator(m_translationEN);
	}

	m_changingLanguage = true;
	ui->retranslateUi(this);
	ui->languageComboBox->setCurrentIndex(index);
	m_changingLanguage = false;
}

void StartDialog::selectConfigurationButtonClicked()
{
	QString newConfiguration = QFileDialog::getOpenFileName(this, tr("Select Configuration File"), QDir::currentPath(), tr("JSON Configuration files (*.js)"));

	if (newConfiguration.isEmpty() == false) 
	{
		ui->configurationLabel->setText(newConfiguration);
    }
}

void StartDialog::okClicked()
{
    if (QFile::exists(ui->configurationLabel->text()) == false)
    {
        QMessageBox::critical(this, tr("Error"), tr("Configuration file not found."));
        return;
    }

    accept();
}
