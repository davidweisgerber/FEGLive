#include <QDebug>
#include <QTimer>
#include "casparconnection.h"

CasparConnection::CasparConnection(QObject *parent)
	: QObject(parent)
{
	m_socket = new QTcpSocket();
	connect(m_socket, SIGNAL(connected()), this, SLOT(connected()));
	connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
	connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(m_socket, &QAbstractSocket::stateChanged, this, &CasparConnection::stateChanged);
}

CasparConnection::~CasparConnection()
{
    delete m_socket;
}

void CasparConnection::connected()
{
	signalError(true, tr("Connected"));
}

void CasparConnection::disconnected()
{
	signalError(false, tr("Disconnected"));
    QTimer::singleShot(500, this, &CasparConnection::reconnect);
}

void CasparConnection::error(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
	case QAbstractSocket::ConnectionRefusedError:
	case QAbstractSocket::RemoteHostClosedError:
	case QAbstractSocket::HostNotFoundError:
	case QAbstractSocket::SocketAccessError:
	case QAbstractSocket::NetworkError:
	default:
		signalError(false, tr("Network error: %1").arg(socketError));
		break;
	}

    QTimer::singleShot(500, this, &CasparConnection::reconnect);
}

void CasparConnection::readyRead()
{
	QString read = m_socket->readAll();
    qDebug() << read;
}

void CasparConnection::reconnect()
{
    m_socket->connectToHost(m_host, m_port);
}

void CasparConnection::stateChanged(QAbstractSocket::SocketState socketState)
{
    qDebug() << socketState;
}

void CasparConnection::connectToHost(const QString &hostname, int port)
{
	m_host = hostname;
	m_port = port;
	m_socket->connectToHost(hostname, port);
}

void CasparConnection::sendCommand(const QString &command)
{
	m_socket->write((command + "\r\n").toUtf8());
}
