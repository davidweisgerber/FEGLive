#ifndef SELECTDATA_H
#define SELECTDATA_H

#include <QObject>
#include <QString>
#include <QPixmap>

class SelectData : public QObject
{
	Q_OBJECT

public:
	SelectData(const QPixmap &icon, const QString &name, const QString &data, const QString &iconName);
	~SelectData();

	const QPixmap &getIcon();
	const QString &getName();
	const QString &getData();
	const QString &getIconName();

private:
	QPixmap m_icon;
	QString m_name;
	QString m_data;
	QString m_iconName;
};

#endif // SELECTDATA_H
