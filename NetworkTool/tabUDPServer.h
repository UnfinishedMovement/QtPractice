#pragma once

#include <QtWidgets/QMainWindow>
#include <QtNetwork>

#include "CommonSocket.h"
#include "ui_tabUDPServer.h"

namespace Ui {
	class tabUDPServer;
}

class tabUDPServer : public QWidget
{
	Q_OBJECT

public:
	explicit tabUDPServer(QWidget* parent = nullptr);
	~tabUDPServer();

private:
	void initWidget();
	void initConfig();

	void sendData(const QString& IP, int Port, const QString& Data);
	void updateTextEdit(int type, const QString& data, bool clear = false);

private slots:
	void on_writeConfig();
	void on_readyRead();
	
private slots:
	void on_pushButtonSend_clicked();

	void on_pushButtonListen_clicked();
	void on_pushButtonSave_clicked();
	void on_pushButtonClear_clicked();

private:
	QUdpSocket* udpSocket;
	QTimer* Timer;

	Ui::tabUDPServer* ui;
};