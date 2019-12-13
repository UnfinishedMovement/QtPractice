#include "tabTCPClient.h"

tabTCPClient::tabTCPClient(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::tabTCPClient)
{
	ui->setupUi(this);
	initWidget();
	initConfig();
}

tabTCPClient::~tabTCPClient()
{
	delete ui;
}

void tabTCPClient::initWidget()
{
	ConnectOnline = false;
	tcpSocket = new QTcpSocket(this);
	connect(tcpSocket, SIGNAL(connected()), this, SLOT(on_connected()));
	connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(on_disconnected()));
	connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(on_disconnected()));
	connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(on_readyRead()));

	Timer = new QTimer(this);
	connect(Timer, SIGNAL(timeout()), this, SLOT(on_pushButtonSend_clicked()));

	ui->textEdit->setReadOnly(true);
	ui->comboBoxInterval->addItems(CommonSocket::Intervals);
}

void tabTCPClient::initConfig()
{
	ui->checkBoxHexSend->setChecked(CommonSocket::TCPClientHexSend);
	ui->checkBoxHexReceive->setChecked(CommonSocket::TCPClientHexReceive);
	ui->checkBoxASCII->setChecked(CommonSocket::TCPClientASCII);
	ui->checkBoxDebug->setChecked(CommonSocket::TCPClientDebug);
	ui->checkBoxTimingSend->setChecked(CommonSocket::TCPClientTimingSend);
	ui->comboBoxInterval->setCurrentIndex(ui->comboBoxInterval->findText(QString::number(CommonSocket::TCPClientInterval)));
	ui->lineEditServerIP->setText(CommonSocket::TCPServerIP);
	ui->lineEditServerPort->setText(QString::number(CommonSocket::TCPServerPort));

	connect(ui->checkBoxHexSend, SIGNAL(stateChanged(int)), this, SLOT(on_saveConfig()));
	connect(ui->checkBoxHexReceive, SIGNAL(stateChanged(int)), this, SLOT(on_saveConfig()));
	connect(ui->checkBoxASCII, SIGNAL(stateChanged(int)), this, SLOT(on_saveConfig()));
	connect(ui->checkBoxDebug, SIGNAL(stateChanged(int)), this, SLOT(on_saveConfig()));
	connect(ui->checkBoxTimingSend, SIGNAL(stateChanged(int)), this, SLOT(on_saveConfig()));
	connect(ui->comboBoxInterval, SIGNAL(currentIndexChanged(int)), this, SLOT(on_saveConfig()));
	connect(ui->lineEditServerIP, SIGNAL(textChanged(QString)), this, SLOT(on_saveConfig()));
	connect(ui->lineEditServerPort, SIGNAL(textChanged(QString)), this, SLOT(on_saveConfig()));

	Timer->setInterval(CommonSocket::TCPClientInterval);
	ui->checkBoxTimingSend->isChecked() ? Timer->start() : Timer->stop();
}

void tabTCPClient::sendData(const QString& Data)
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

	QString String = QString("[%1:%2] ").arg(ui->lineEditServerIP->text()).arg(ui->lineEditServerPort->text()) + Data;
	updateTextEdit(0, String);
}

void tabTCPClient::updateTextEdit(int type, const QString& data, bool clear)
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
	else
	{
		StringType = "连接";
		ui->textEdit->setTextColor(QColor("blue"));
	}

	StringData = QString("时间[%1] %2: %3").arg(qPrintable(QTime::currentTime().toString("HH:mm:ss zzz"))).arg(StringType).arg(StringData);
	ui->textEdit->append(StringData);
	CurrentCount++;
}

void tabTCPClient::on_saveConfig()
{
	CommonSocket::TCPClientHexSend = ui->checkBoxHexSend->isChecked();
	CommonSocket::TCPClientHexReceive = ui->checkBoxHexReceive->isChecked();
	CommonSocket::TCPClientASCII = ui->checkBoxASCII->isChecked();
	CommonSocket::TCPClientDebug = ui->checkBoxDebug->isChecked();
	CommonSocket::TCPClientTimingSend = ui->checkBoxTimingSend->isChecked();
	CommonSocket::TCPServerInterval = ui->comboBoxInterval->currentText().toInt();
	CommonSocket::TCPServerIP = ui->lineEditServerIP->text().trimmed();
	CommonSocket::TCPServerPort = ui->lineEditServerPort->text().toInt();
	CommonSocket::writeConfig();

	Timer->setInterval(CommonSocket::TCPClientInterval);
	CommonSocket::TCPClientTimingSend ? Timer->start() : Timer->stop();
}

void tabTCPClient::on_connected()
{
	ConnectOnline = true;
	ui->pushButtonConnect->setText("断开");
	updateTextEdit(2, "服务器连接");
}

void tabTCPClient::on_disconnected()
{
	ConnectOnline = false;
	tcpSocket->abort();
	ui->pushButtonConnect->setText("连接");
	updateTextEdit(2, "服务器断开");
}

void tabTCPClient::on_readyRead()
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
	Buffer = QString("[%1:%2] ").arg(ui->lineEditServerIP->text()).arg(ui->lineEditServerPort->text()) + Buffer;
	updateTextEdit(1, Buffer);

	if (ui->checkBoxDebug->isChecked())
	{
		int Count = CommonSocket::Keys.count();
		for (int i = 0;i < Count;i++)
		{
			if (CommonSocket::Keys.at(i) == Buffer)
			{
				sendData(CommonSocket::Values.at(i));
				break;
			}
		}
	}
}

void tabTCPClient::on_pushButtonSend_clicked()
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

	sendData(Data);
}

void tabTCPClient::on_pushButtonConnect_clicked()
{
	if (ui->pushButtonConnect->text() == "连接")
	{
		tcpSocket->abort();
		tcpSocket->connectToHost(ui->lineEditServerIP->text(), ui->lineEditServerPort->text().toInt());
	}
	else
	{
		tcpSocket->disconnectFromHost();
	}
}

void tabTCPClient::on_pushButtonSave_clicked()
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

void tabTCPClient::on_pushButtonClear_clicked()
{
	updateTextEdit(0, "", true);
}
