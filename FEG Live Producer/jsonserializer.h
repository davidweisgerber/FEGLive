#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include <QString>
#include <QByteArray>
#include <QObject>
#include <QMap>
#include <usertypeserializer.h>
#include <vector>

class QScriptValue;
class QMetaProperty;

class JSONSerializer
{
public:
	JSONSerializer();
	~JSONSerializer();

	QByteArray serialize(const QObject &object);
	QByteArray serialize(const std::vector<const QObject *> &objects);
	QByteArray serialize(const QVariant &variant);

	void deserialize(QByteArray json, QObject *out);
	void deserialize(QString json, QObject *out);
	std::vector<QObject *> deserialize(QByteArray json, const QMetaObject &type);
	std::vector<QObject *> deserialize(QString json, const QMetaObject &type);
	void deserialize(QByteArray json, QVariant *out);
	void deserialize(QString json, QVariant *out);

	static QString sanitizeString(QString str);
	void registerUserType(int typeID, UserTypeSerializer *serializer);

private:
	void serializeProperty(const QMetaProperty &property, const QObject &object, QString* jsonObject);
	void serializeVariant(const QVariant &variant, QString* jsonObject);
	void deserializeProperty(const QScriptValue &json, QMetaProperty property, QObject *out );
	void sanitizeList(QString *stringToSanitize, int numberOfItems, const QString &endCharacter);

	QMap<int, UserTypeSerializer *> m_serializers;
};

#endif // JSONSERIALIZER_H
