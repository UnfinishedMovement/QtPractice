#pragma once

#include <QtWidgets/QMainWindow>
#include <QtNetwork>

#include "CommonSocket.h"
#include "ui_tabTCPClient.h"

namespace Ui {
	class tabTCPClient;
}

class tabTCPClient : public QWidget
{
	Q_OBJECT

public:
	explicit tabTCPClient(QWidget* parent = nullptr);
	~tabTCPClient();

private:
	void initWidget();
	void initConfig();

	void sendData(const QString& Data);
	void updateTextEdit(int type, const QString& data, bool clear = false);

private slots:
	void on_saveConfig();

	void on_connected();
	void on_disconnected();
	void on_readyRead();

private slots:
	void on_pushButtonSend_clicked();

	void on_pushButtonConnect_clicked();
	void on_pushButtonSave_clicked();
	void on_pushButtonClear_clicked();

private:
	bool ConnectOnline;
	QTcpSocket* tcpSocket;
	QTimer* Timer;

	Ui::tabTCPClient* ui;
};