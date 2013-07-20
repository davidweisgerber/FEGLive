#ifndef usertypeserializer_h__
#define usertypeserializer_h__

#include <QVariant>
#include <QString>

class UserTypeSerializer 
{
public:
	UserTypeSerializer() {}
	~UserTypeSerializer() {}

	virtual QString serialize(QVariant variant) {return "";}
	virtual QVariant deserialize(const QString json) {return QVariant(QVariant::String);}
};

#endif // usertypeserializer_h__
