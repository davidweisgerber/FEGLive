#include <QDebug>
#include "casparconnection.h"

CasparConnection::CasparConnection(QObject *parent)
	: QObject(parent)
{
	m_socket = new QTcpSocket();
	connect(m_socket, SIGNAL(connected()), this, SLOT(connected()));
	connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
	connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

CasparConnection::~CasparConnection()
{

}

void CasparConnection::connected()
{
	signalError(true, tr("Connected"));
}

void CasparConnection::disconnected()
{
	signalError(false, tr("Disconnected"));
}

void CasparConnection::error(QAbstractSocket::SocketError socketError)
{
	switch (socketError) {
	case QAbstractSocket::ConnectionRefusedError:
	case QAbstractSocket::RemoteHostClosedError:
	case QAbstractSocket::HostNotFoundError:
	case QAbstractSocket::SocketAccessError:
	case QAbstractSocket::NetworkError:
	default:
		signalError(false, tr("Network error: %1").arg(socketError));
		break;
	}
}

void CasparConnection::readyRead()
{
	QString read = m_socket->readAll();
	qDebug() << read;
}

void CasparConnection::connectToHost(const QString &hostname, int port)
{
	m_host = hostname;
	m_port = port;
	m_socket->connectToHost(hostname, port);
}

void CasparConnection::sendCommand(const QString &command)
{
	m_socket->write((command + "\r\n").toLatin1());
}
