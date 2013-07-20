#include "jsonserializer.h"
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValue>
#include <QMetaProperty>
#include <QDateTime>

JSONSerializer::JSONSerializer()
{

}

JSONSerializer::~JSONSerializer()
{

}

QByteArray JSONSerializer::serialize(const QObject &object)
{
	QString jsonObject = "{";
	const QMetaObject *metaObject = object.metaObject();
	
	int propertyCount = metaObject->propertyCount();
	for (int i=1; i < propertyCount; i++)
	{
		serializeProperty(metaObject->property(i), object, &jsonObject);
	}

	sanitizeList(&jsonObject, propertyCount - 1, "}");

	return jsonObject.toUtf8();
}

void JSONSerializer::serializeProperty(const QMetaProperty &property, const QObject &object, QString* jsonObject)
{
	*jsonObject += "\"" + QString(property.name()) + "\":";
	
	serializeVariant(property.read(&object), jsonObject);

	*jsonObject += ",";
}

QString JSONSerializer::sanitizeString(QString str)
{
	str.replace( QLatin1String( "\\" ), QLatin1String( "\\\\" ) );
	str.replace( QLatin1String( "\"" ), QLatin1String( "\\\"" ) );
	str.replace( QLatin1String( "\b" ), QLatin1String( "\\b" ) );
	str.replace( QLatin1String( "\f" ), QLatin1String( "\\f" ) );
	str.replace( QLatin1String( "\n" ), QLatin1String( "\\n" ) );
	str.replace( QLatin1String( "\r" ), QLatin1String( "\\r" ) );
	str.replace( QLatin1String( "\t" ), QLatin1String( "\\t" ) );
	return QString( QLatin1String( "\"%1\"" ) ).arg( str );
}

void JSONSerializer::registerUserType( int typeID, UserTypeSerializer *serializer )
{
	m_serializers.insert(typeID, serializer);
}

void JSONSerializer::deserialize(QByteArray json, QObject *out)
{
	if (out == 0)
	{
		return;
	}

	deserialize(QString::fromUtf8(json), out);
}

void JSONSerializer::deserialize(QString json, QObject *out)
{
	if (out == 0)
	{
		return;
	}

	QScriptEngine engine;
	QScriptValue object = engine.evaluate("(" + json + ")");

	const QMetaObject *metaObject = out->metaObject();

	int propertyCount = metaObject->propertyCount();
	for (int i=1; i < propertyCount; i++)
	{
		deserializeProperty(object, metaObject->property(i), out);
	}
}

QByteArray JSONSerializer::serialize(const std::vector<const QObject *> &objects)
{
	QByteArray json = "[";

	foreach (const QObject *object, objects)
	{
		json += serialize(*object) + ",";
	}

	if (objects.size() > 0)
	{
		(json.data())[json.length() - 1] = ']';
	}
	else
	{
		json += "]";
	}

	return json;
}

QByteArray JSONSerializer::serialize(const QVariant &variant)
{
	QString json = "";
	serializeVariant(variant, &json);
	return json.toUtf8();
}

std::vector<QObject *> JSONSerializer::deserialize(QByteArray json, const QMetaObject &type)
{
	return deserialize(QString::fromUtf8(json), type);
}

std::vector<QObject *> JSONSerializer::deserialize(QString json, const QMetaObject &type)
{
	std::vector<QObject *> objects;

	QScriptEngine engine;
	QScriptValue object = engine.evaluate("(" + json + ")");
	if (object.isArray() == false) 
	{
		return objects;
	}

	int propertyCount = type.propertyCount();
	
	int j = 0;
	while (object.property(j).isValid())
	{
		QObject *out = type.newInstance();

		for (int i=1; i < propertyCount; i++)
		{
			deserializeProperty(object.property(j), type.property(i), out);
		}

		objects.push_back(out);
		j++;
	}

	return objects;
}

void JSONSerializer::deserialize(QByteArray json, QVariant *out)
{
	if (out == 0)
	{
		return;
	}

	deserialize(QString::fromUtf8(json), out);
}

void JSONSerializer::deserialize(QString json, QVariant *out)
{
	if (out == 0)
	{
		return;
	}

	QScriptEngine engine;
	QScriptValue object = engine.evaluate("(" + json + ")");
	*out = object.toVariant();
}

void JSONSerializer::deserializeProperty(const QScriptValue &json, QMetaProperty property, QObject *out)
{
	if (json.property(property.name()).isValid() == false) {
		return;
	}

	QVariant value = json.property(property.name()).toVariant();

	switch (property.type()) 
	{
	case QVariant::String:
		property.write(out, value);
		break;
	case QVariant::Date:
		property.write(out, QDateTime::fromMSecsSinceEpoch(value.toLongLong()).date());
		break;
	case QVariant::DateTime:
		property.write(out, QDateTime::fromMSecsSinceEpoch(value.toLongLong()));
		break;
	case QVariant::Bool:
		property.write(out, QVariant(value.toString() == "true"));
		break;
	case QVariant::UserType:
		property.write(out, m_serializers[property.userType()]->deserialize(json.property(property.name())));
		break;
	default:
		property.write(out, value);
		break;
	}
}

void JSONSerializer::serializeVariant(const QVariant &variant, QString* jsonObject)
{
	switch (variant.type()) 
	{
	case QVariant::Invalid:
		break;
	case QVariant::String:
		*jsonObject += sanitizeString(variant.toString());
		break;
	case QVariant::Date:
		*jsonObject += QString("%1").arg(QDateTime(variant.toDate()).toMSecsSinceEpoch());
		break;
	case QVariant::DateTime:
		*jsonObject += QString("%1").arg(variant.toDateTime().toMSecsSinceEpoch());
		break;
	case QVariant::Bool:
		*jsonObject += variant.toBool() ? "true" : "false";
		break;
	case QVariant::UserType:
		*jsonObject += m_serializers[variant.userType()]->serialize(variant);
		break;
	case QVariant::ByteArray:
		{
			*jsonObject += "[";
			QByteArray byteArray = variant.toByteArray();
			for (int i=0; i < byteArray.size(); i++)
			{
				*jsonObject += QString("%1,").arg(static_cast<unsigned char>(byteArray[i]), 1, 10, QChar('0'));
			}
			sanitizeList(jsonObject, byteArray.size(), "]");
			break;
		}
	case QVariant::StringList:
	case QVariant::List:
		{
			*jsonObject += "[";
			QList<QVariant> variantList = variant.toList();
			foreach(QVariant innerVariant, variantList)
			{
				serializeVariant(innerVariant, jsonObject);
				*jsonObject += ",";
			}
			sanitizeList(jsonObject, variantList.size(), "]");
			break;
		}
	case QVariant::Map:
		{
			*jsonObject += "{";
			QMap<QString, QVariant> variantMap = variant.toMap();
			foreach(QString key, variantMap.keys())
			{
				*jsonObject += "\"" + key + "\":";
				serializeVariant(variantMap[key], jsonObject);
				*jsonObject += ",";
			}
			sanitizeList(jsonObject, variantMap.keys().size(), "}");
			break;
		}
	case QVariant::Hash:
		{
			*jsonObject += "{";
			QHash<QString, QVariant> variantMap = variant.toHash();
			foreach(QString key, variantMap.keys())
			{
				*jsonObject += "\"" + key + "\":";
				serializeVariant(variantMap[key], jsonObject);
				*jsonObject += ",";
			}
			sanitizeList(jsonObject, variantMap.keys().size(), "}");
			break;
		}
	default:
		*jsonObject += variant.toString();
		break;
	}
}

void JSONSerializer::sanitizeList(QString *stringToSanitize, int numberOfItems, const QString &endCharacter)
{
	if (numberOfItems > 0) 
	{
		stringToSanitize->replace(stringToSanitize->length() - 1, 1 , endCharacter);
	} 
	else 
	{
		*stringToSanitize += endCharacter;
	}
}
