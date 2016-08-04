#include <QTreeWidgetItem>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QProcess>
#include "saverecord.h"
#include "configurationparser.h"
#include "startdialog.h"
#include "ui_saverecord.h"

SaveRecord::SaveRecord(const QList<RecordManager::Recording> &records, StartDialog *startDialog, ConfigurationParser *config, QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::SaveRecord();
	ui->setupUi(this);

	m_startDialog = startDialog;
	m_records = records;
	m_config = config;
	m_waitingBox = 0;

	connect(ui->saveVideoButton, SIGNAL(clicked()), this, SLOT(saveVideo()));
	connect(ui->saveAudioButton, SIGNAL(clicked()), this, SLOT(saveAudio()));

	buildList();
}

SaveRecord::~SaveRecord()
{
	delete ui;
}

void SaveRecord::buildList()
{
	ui->treeWidget->clear();

	foreach (RecordManager::Recording record, m_records) 
	{
		int seconds = record.start.secsTo(record.end);
		QString status;

		if (record.savedAudio == false && record.savedVideo == false) 
		{
			status = tr("Not saved");
		} else
		{
			if (record.savedAudio) 
			{
				status = tr("Audio saved ");
			}

			if (record.savedVideo)
			{
				status += tr("Video saved");
			}
		}

		QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << record.start.toString() << record.end.toString() << tr("%1:%2").arg(static_cast<int>(seconds / 60), 2, 10, QChar('0')).arg(static_cast<int>(seconds % 60), 2, 10, QChar('0')) << status);
		ui->treeWidget->addTopLevelItem(item);
	}

	ui->treeWidget->resizeColumnToContents(0);
	ui->treeWidget->resizeColumnToContents(1);
	ui->treeWidget->resizeColumnToContents(2);
	ui->treeWidget->resizeColumnToContents(3);
}

void SaveRecord::saveAudio()
{
	if (ui->treeWidget->currentItem() == 0) 
	{
		return;
	}

	QString proposedFilename = m_config->getAudioPath() + "/" + getFilenamePart() + ".web.wav";
	QString filename = QFileDialog::getSaveFileName(this, tr("Save as Wave..."), proposedFilename, tr("Wave Audio (*.wav)"));

	if (filename.isEmpty())
	{
		return;
	}

	m_waitingBox = new QMessageBox(QMessageBox::Information, tr("Saving Wave..."), tr("Please wait until the wave file was saved"));
	m_waitingBox->setModal(true);

	int index = ui->treeWidget->indexOfTopLevelItem(ui->treeWidget->currentItem());

	QProcess *ffmpegProcess = new QProcess();
	connect(ffmpegProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(saveAudioFinished(int, QProcess::ExitStatus)));
	m_waitingBox->show();
	ffmpegProcess->start(m_config->getFfmpegPath(), QStringList() <<
		"-i" << m_config->getMediaPath() + "/" + m_records[index].filename <<
		"-vn" << "-ar" << "44100" << "-ac" << "2" << "-f" << "wav" <<
		filename);
}

void SaveRecord::saveVideo()
{
	if (ui->treeWidget->currentItem() == 0) 
	{
		return;
	}

	QString proposedFilename = m_config->getVideoPath() + "/" + getFilenamePart() + ".mov";
	QString filename = QFileDialog::getSaveFileName(this, tr("Save as Movie..."), proposedFilename, tr("Movies (*.mov)"));
	
	if (filename.isEmpty() == false) 
	{
		copyVideo(filename);
	}
}

QString SaveRecord::getFilenamePart()
{
	QString timeTypeString = "";

	switch (m_startDialog->getTimeType()) {
	case MORNING:
		timeTypeString = "m";
		break;
	case EVENING:
		timeTypeString = "a";
		break;
	case MICC:
		timeTypeString = "MICC";
		break;
	}

    QString proposal = m_startDialog->getDate().toString("yyyyMMdd") + timeTypeString + " - " + m_startDialog->getPreacher() + " - " + m_startDialog->getSermonTitle() + " - " + m_startDialog->getBibleText();
    proposal = proposal.replace(":", ",");
    proposal = proposal.replace("?", "");
    proposal = proposal.replace("<", "");
    proposal = proposal.replace(">", "");
    proposal = proposal.replace("|", "");
    proposal = proposal.replace("/", "");
    proposal = proposal.replace("\\", "");
    proposal = proposal.replace("*", "");
    proposal = proposal.replace("\"", "");

    return proposal;
}

void SaveRecord::copyVideo(QString to)
{
	int index = ui->treeWidget->indexOfTopLevelItem(ui->treeWidget->currentItem());
	QFile fromFile(m_config->getMediaPath() + "/" + m_records[index].filename);
	if (!fromFile.open(QIODevice::ReadOnly)) 
	{
		QMessageBox::critical(this, tr("Error opening source file"), tr("The source video file could not be opened. File will not be saved."), QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}

	QFile toFile(to);
	if (!toFile.open(QIODevice::WriteOnly)) 
	{
		QMessageBox::critical(this, tr("Error opening destination file"), tr("The destination file for writing could not be created. File will not be saved."), QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}

	QProgressDialog *progressDialog = new QProgressDialog(tr("Copying video file..."), tr("Cancel"), 0, 100, this);
	progressDialog->setValue(0);
	progressDialog->setWindowModality(Qt::WindowModal);

	const int bufferSize = 1024 * 1024;
	char buffer[bufferSize];

	qint64 size = fromFile.size();
	qint64 written = 0;

	while (!fromFile.atEnd()) 
	{
		int bytesRead = fromFile.read(buffer, bufferSize);
		toFile.write(buffer, bytesRead);

		written += bytesRead;
		progressDialog->setValue((written * 100) / size);

		if (progressDialog->wasCanceled()) 
		{
			break;
		}

		qApp->processEvents();
	}

	m_records[index].savedVideo = true;
	buildList();

	delete progressDialog;
}

void SaveRecord::saveAudioFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	if (m_waitingBox != 0)
	{
		m_waitingBox->hide();
		delete m_waitingBox;
		m_waitingBox = 0;
	}

	if (exitCode != 0 || exitStatus != QProcess::NormalExit)
	{
		QMessageBox::critical(this, tr("Problems while saving wave audio"), tr("The audio wave file could not be saved."));
	}

	int index = ui->treeWidget->indexOfTopLevelItem(ui->treeWidget->currentItem());
	m_records[index].savedAudio = true;
	buildList();
}
