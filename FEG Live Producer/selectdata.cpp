#include "selectdata.h"

SelectData::SelectData( const QPixmap &icon, const QString &name, const QString &data ) :
	m_icon (icon),
	m_name (name),
	m_data (data)
{
}

SelectData::~SelectData()
{

}

const QPixmap & SelectData::getIcon()
{
	return m_icon;
}

const QString & SelectData::getName()
{
	return m_name;
}

const QString & SelectData::getData()
{
	return m_data;
}
