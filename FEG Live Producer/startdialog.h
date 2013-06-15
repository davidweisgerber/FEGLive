#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QDialog>
#include <QDate>
#include <QString>
#include "TimeType.h"

namespace Ui {class StartDialog;};

class QTranslator;

class StartDialog : public QDialog
{
	Q_OBJECT

public:
	StartDialog(QWidget *parent = 0);
	~StartDialog();

	QDate getDate() const;
	TimeType getTimeType() const;
	QString getPreacher() const;
	QString getSermonTitle() const;
	QString getBibleText() const;
	QString getConfigurationFileName() const;
	QStringList getSongs() const;

private slots:
	void languageChanged(int index);
	void selectConfigurationButtonClicked();

private:
	Ui::StartDialog *ui;
	QString m_songDir;

	QTranslator *m_translationDE;
	QTranslator *m_translationEN;

	bool m_changingLanguage;
};

#endif // STARTDIALOG_H
