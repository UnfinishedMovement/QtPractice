#include "tabTCPServer.h"

tabTCPServer::tabTCPServer(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::tabTCPServer)
{
	ui->setupUi(this);
	initWidget();
	initConfig();
}

tabTCPServer::~tabTCPServer()
{
	delete ui;
}

void tabTCPServer::initWidget()
{
	ConnectOnline = false;
	tcpServer = new QTcpServer(this);
	connect(tcpServer, SIGNAL(newConnection()), this, SLOT(on_newConnection()));

	Timer = new QTimer(this);
	connect(Timer, SIGNAL(timeout()), this, SLOT(on_pushButtonSend_clicked()));

	ui->textEdit->setReadOnly(true);
	ui->pushButtonClose->setEnabled(false);
	ui->comboBoxInterval->addItems(CommonSocket::Intervals);
}

void tabTCPServer::initConfig()
{
	ui->checkBoxHexSend->setChecked(CommonSocket::TCPServerHexSend);
	ui->checkBoxHexReceive->setChecked(CommonSocket::TCPServerHexReceive);
	ui->checkBoxASCII->setChecked(CommonSocket::TCPServerASCII);
	ui->checkBoxDebug->setChecked(CommonSocket::TCPServerDebug);
	ui->checkBoxTimingSend->setChecked(CommonSocket::TCPServerTimingSend);
	ui->comboBoxInterval->setCurrentIndex(ui->comboBoxInterval->findText(QString::number(CommonSocket::TCPServerInterval)));
	ui->lineEditListenPort->setText(QString::number(CommonSocket::TCPListenPort));

	connect(ui->checkBoxHexSend, SIGNAL(stateChanged(int)), this, SLOT(on_saveConfig()));
	connect(ui->checkBoxHexReceive, SIGNAL(stateChanged(int)), this, SLOT(on_saveConfig()));
	connect(ui->checkBoxASCII, SIGNAL(stateChanged(int)), this, SLOT(on_saveConfig()));
	connect(ui->checkBoxDebug, SIGNAL(stateChanged(int)), this, SLOT(on_saveConfig()));
	connect(ui->checkBoxTimingSend, SIGNAL(stateChanged(int)), this, SLOT(on_saveConfig()));
	connect(ui->comboBoxInterval, SIGNAL(currentIndexChanged(int)), this, SLOT(on_saveConfig()));
	connect(ui->lineEditListenPort, SIGNAL(textChanged(QString)), this, SLOT(on_saveConfig()));

	Timer->setInterval(CommonSocket::TCPServerInterval);
	CommonSocket::TCPServerTimingSend ? Timer->start() : Timer->stop();
}

void tabTCPServer::sendData(const QString& IP, int Port, const QString& Data)
{

	QByteArray Buffer;
	if (ui->checkBoxHexSend->isChecked())
	{
		Buffer = CommonSocket::hexStringToByteArray(Data);
	}
	else if (ui->checkBoxASCII->isChecked())
	{
		Buffer = CommonSocket::asciiStringToByteArray(Data);
	}
	else
	{
		Buffer = Data.toLatin1();
	}

	tcpSocket->write(Buffer);

	QString String = QString("[%1:%2] %3").arg(IP).arg(Port).arg(Data);
	updateTextEdit(0, String);
}

void tabTCPServer::updateTextEdit(int type, const QString& data, bool clear)
{
	static int CurrentCount = 0;
	static int MaxCount = 200;

	if (clear)
	{
		ui->textEdit->clear();
		CurrentCount = 0;
		return;
	}
	else if (CurrentCount >= MaxCount)
	{
		ui->textEdit->clear();
		CurrentCount = 0;
	}
	if (ui->checkBoxPauseShow->isChecked())
	{
		return;
	}

	QString StringData = data;
	StringData = StringData.replace("\r", "");
	StringData = StringData.replace("\n", "");

	QString StringType;
	if (type == 0)
	{
		StringType = "发送";
		ui->textEdit->setTextColor(QColor("green"));
	}
	else if (type == 1)
	{
		StringType = "接收";
		ui->textEdit->setTextColor(QColor("red"));
	}
	else if (type == 2)
	{
		StringType = "连接";
		ui->textEdit->setTextColor(QColor("blue"));
	}

	StringData = QString("时间[%1] %2: %3").arg(qPrintable(QTime::currentTime().toString("HH:mm:ss zzz"))).arg(StringType).arg(StringData);
	ui->textEdit->append(StringData);
	CurrentCount++;
}

void tabTCPServer::on_saveConfig()
{
	CommonSocket::TCPServerHexSend = ui->checkBoxHexSend->isChecked();
	CommonSocket::TCPServerHexReceive = ui->checkBoxHexReceive->isChecked();
	CommonSocket::TCPServerASCII = ui->checkBoxASCII->isChecked();
	CommonSocket::TCPServerDebug = ui->checkBoxDebug->isChecked();
	CommonSocket::TCPServerTimingSend = ui->checkBoxTimingSend->isChecked();
	CommonSocket::TCPServerInterval = ui->comboBoxInterval->currentText().toInt();
	CommonSocket::TCPListenPort = ui->lineEditListenPort->text().toInt();
	CommonSocket::writeConfig();

	Timer->setInterval(CommonSocket::TCPServerInterval);
	CommonSocket::TCPServerTimingSend ? Timer->start() : Timer->stop();
}

void tabTCPServer::on_newConnection()
{
	tcpSocket = tcpServer->nextPendingConnection();
	connect(tcpSocket, SIGNAL(connected()), this, SLOT(on_clientConnected()));
	connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(on_clientDisconnected()));
	connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(on_disconnected()));
	connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(on_socketReadyRead()));

	on_clientConnected();
}

void tabTCPServer::on_clientConnected()
{
	QString String = QString("[%1:%2] 客户端连接").arg(tcpSocket->peerAddress().toString()).arg(tcpSocket->peerPort());
	updateTextEdit(2, String);
	ui->pushButtonClose->setEnabled(true);
}

void tabTCPServer::on_clientDisconnected()
{
	QString String = QString("[%1:%2] 客户端断开").arg(tcpSocket->peerAddress().toString()).arg(tcpSocket->peerPort());
	updateTextEdit(2, String);
	ui->pushButtonClose->setEnabled(false);
}

void tabTCPServer::on_socketReadyRead()
{
	QByteArray Data = tcpSocket->readAll();
	if (Data.length() <= 0)
	{
		return;
	}

	QString Buffer;
	if (ui->checkBoxHexReceive->isChecked())
	{
		Buffer = CommonSocket::byteArrayToHexString(Data);
	}
	else if (ui->checkBoxASCII->isChecked())
	{
		Buffer = CommonSocket::byteArrayToASCIIString(Data);
	}
	else
	{
		Buffer = QString(Data);
	}
	Buffer = QString("[%1:%2] ").arg(tcpSocket->peerAddress().toString()).arg(tcpSocket->peerPort()) + Buffer;
	updateTextEdit(1, Buffer);

	if (ui->checkBoxDebug->isChecked())
	{
		int Count = CommonSocket::Keys.count();
		for (int i = 0;i < Count;i++)
		{
			if (CommonSocket::Keys.at(i) == Buffer)
			{
				sendData(tcpSocket->peerAddress().toString(), tcpSocket->peerPort(), CommonSocket::Values.at(i));
				break;
			}
		}
	}
}

void tabTCPServer::on_pushButtonSend_clicked()
{
	if (!ConnectOnline)
	{
		return;
	}

	QString Data = ui->lineEditData->text();
	if (Data.length() <= 0)
	{
		return;
	}
	sendData(tcpSocket->peerAddress().toString(),tcpSocket->peerPort(),Data);
}

void tabTCPServer::on_pushButtonListen_clicked()
{
	if (ui->pushButtonListen->text() == "监听")
	{
		ConnectOnline = tcpServer->listen(QHostAddress("0.0.0.0"),ui->lineEditListenPort->text().toInt());
		qDebug() << ConnectOnline;
		if (ConnectOnline)
		{
			ui->pushButtonListen->setText("断开");
			updateTextEdit(2, "监听成功");
		}
	}
	else
	{
		ConnectOnline = false;
		tcpSocket->disconnectFromHost();
		tcpServer->close();
		ui->pushButtonListen->setText("监听");
	}
}

void tabTCPServer::on_pushButtonClose_clicked()
{
	if (tcpServer->isListening())
	{
		tcpSocket->disconnectFromHost();
		ui->pushButtonClose->setEnabled(false);
	}
}

void tabTCPServer::on_pushButtonSave_clicked()
{
	QString Data = ui->textEdit->toPlainText();
	if (Data.length() <= 0)
	{
		return;
	}

	QString FileName = QString("%1/%2.txt").arg(qApp->applicationDirPath()).arg(qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss")));
	QFile File(FileName);
	if (File.open(QFile::WriteOnly | QFile::Text))
	{
		File.write(Data.toUtf8());
		File.close();
	}
}

void tabTCPServer::on_pushButtonClear_clicked()
{
	updateTextEdit(0, "", true);
}