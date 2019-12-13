#pragma once

#include <QtWidgets/QMainWindow>
#include <QtNetwork>

#include "CommonSocket.h"
#include "ui_tabTCPServer.h"

namespace Ui {
	class tabTCPServer;
}

class tabTCPServer : public QWidget
{
	Q_OBJECT

public:
	explicit tabTCPServer(QWidget* parent = nullptr);
	~tabTCPServer();

private:
	void initWidget();
	void initConfig();

	void sendData(const QString& IP, int Port, const QString& Data);
	void updateTextEdit(int type, const QString& data, bool clear = false);

private slots:;
	void on_saveConfig();

	void on_newConnection();

	void on_clientConnected();
	void on_clientDisconnected();
	void on_socketReadyRead();

private slots:;
	void on_pushButtonSend_clicked();

	void on_pushButtonListen_clicked();
	void on_pushButtonClose_clicked();
	void on_pushButtonSave_clicked();
	void on_pushButtonClear_clicked();

private:
	bool ConnectOnline;
	QTimer* Timer;
	QTcpServer* tcpServer;
	QTcpSocket* tcpSocket;

	Ui::tabTCPServer* ui;
};