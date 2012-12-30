#ifndef SELECTDATA_H
#define SELECTDATA_H

#include <QObject>
#include <QString>
#include <QPixmap>

class SelectData : public QObject
{
	Q_OBJECT

public:
	SelectData(const QPixmap &icon, const QString &name, const QString &data);
	~SelectData();

	const QPixmap &getIcon();
	const QString &getName();
	const QString &getData();

private:
	QPixmap m_icon;
	QString m_name;
	QString m_data;
};

#endif // SELECTDATA_H
