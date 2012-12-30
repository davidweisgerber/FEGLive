#ifndef CASPARCONNECTION_H
#define CASPARCONNECTION_H

#include <QObject>
#include <QTcpSocket>

class CasparConnection : public QObject
{
	Q_OBJECT

public:
	CasparConnection(QObject *parent);
	~CasparConnection();

	void connectToHost(const QString &hostname, int port);
	void sendCommand(const QString &command);

private slots:
	void connected();
	void disconnected();
	void error(QAbstractSocket::SocketError socketError);
	void readyRead();

private:
	QTcpSocket *m_socket;
	QString m_host;
	int m_port;

signals:
	void signalError(bool onOff, const QString &message);
};

#endif // CASPARCONNECTION_H
