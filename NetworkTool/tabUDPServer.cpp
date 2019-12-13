#include "tabUDPServer.h"

tabUDPServer::tabUDPServer(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::tabUDPServer)
{
	ui->setupUi(this);
	initWidget();
	initConfig();
}

tabUDPServer::~tabUDPServer()
{
	delete ui;
}

void tabUDPServer::initWidget()
{
	udpSocket = new QUdpSocket(this);
	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(on_readyRead()));

	Timer = new QTimer(this);
	connect(Timer, SIGNAL(timeout()), this, SLOT(on_pushButtonSend_clicked()));

	ui->textEdit->setReadOnly(true);
	ui->comboBoxInterval->addItems(CommonSocket::Intervals);
}

void tabUDPServer::initConfig()
{
	ui->checkBoxHexSend->setChecked(CommonSocket::UDPServerHexSend);
	ui->checkBoxHexReceive->setChecked(CommonSocket::UDPServerHexReceive);
	ui->checkBoxASCII->setChecked(CommonSocket::UDPServerASCII);
	ui->checkBoxDebug->setChecked(CommonSocket::UDPServerDebug);
	ui->checkBoxTimingSend->setChecked(CommonSocket::UDPServerTimingSend);
	ui->comboBoxInterval->setCurrentIndex(ui->comboBoxInterval->findText(QString::number(CommonSocket::UDPServerInterval)));
	ui->lineEditServerIP->setText(CommonSocket::UDPServerIP);
	ui->lineEditServerPort->setText(QString::number(CommonSocket::UDPServerPort));
	ui->lineEditListenPort->setText(QString::number(CommonSocket::UDPListenPort));

	connect(ui->checkBoxHexSend, SIGNAL(stateChanged(int)), this, SLOT(on_writeConfig()));
	connect(ui->checkBoxHexReceive, SIGNAL(stateChanged(int)), this, SLOT(on_writeConfig()));
	connect(ui->checkBoxASCII, SIGNAL(stateChanged(int)), this, SLOT(on_writeConfig()));
	connect(ui->checkBoxDebug, SIGNAL(stateChanged(int)), this, SLOT(on_writeConfig()));
	connect(ui->checkBoxTimingSend, SIGNAL(stateChanged(int)), this, SLOT(on_writeConfig()));
	connect(ui->comboBoxInterval, SIGNAL(currentIndexChanged(int)), this, SLOT(on_writeConfig()));
	connect(ui->lineEditServerIP, SIGNAL(textChanged(QString)), this, SLOT(on_writeConfig()));
	connect(ui->lineEditServerPort, SIGNAL(textChanged(QString)), this, SLOT(on_writeConfig()));
	connect(ui->lineEditListenPort, SIGNAL(textChanged(QString)), this, SLOT(on_writeConfig()));

	Timer->setInterval(CommonSocket::UDPServerInterval);
	CommonSocket::UDPServerTimingSend ? Timer->start() : Timer->stop();
}

void tabUDPServer::sendData(const QString& IP, int Port, const QString& Data)
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

	udpSocket->writeDatagram(Buffer, QHostAddress(IP), Port);

	QString String = QString("[%1:%2] %3").arg(IP).arg(Port).arg(Data);
	updateTextEdit(0, String);
}

void tabUDPServer::updateTextEdit(int type, const QString& data, bool clear)
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

void tabUDPServer::on_writeConfig()
{
	CommonSocket::UDPServerHexSend = ui->checkBoxHexSend->isChecked();
	CommonSocket::UDPServerHexReceive = ui->checkBoxHexReceive->isChecked();
	CommonSocket::UDPServerASCII = ui->checkBoxASCII->isChecked();
	CommonSocket::UDPServerDebug = ui->checkBoxDebug->isChecked();
	CommonSocket::UDPServerTimingSend = ui->checkBoxTimingSend->isChecked();
	CommonSocket::UDPServerInterval = ui->comboBoxInterval->currentText().toInt();
	CommonSocket::UDPServerIP = ui->lineEditServerIP->text().trimmed();
	CommonSocket::UDPServerPort = ui->lineEditServerPort->text().toInt();
	CommonSocket::UDPListenPort = ui->lineEditListenPort->text().toInt();
	CommonSocket::writeConfig();

	Timer->setInterval(CommonSocket::UDPServerInterval);
	CommonSocket::UDPServerTimingSend ? Timer->start() : Timer->stop();
}

void tabUDPServer::on_readyRead()
{
	QHostAddress Host;
	quint16 Port;
	QByteArray Data;
	QString Buffer;

	while (udpSocket->hasPendingDatagrams())
	{
		Data.resize(udpSocket->pendingDatagramSize());
		udpSocket->readDatagram(Data.data(), Data.size(), &Host, &Port);

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

		QString IP = Host.toString();
		if (IP.isEmpty())
		{
			continue;
		}

		QString String = QString("[%1:%2] %3").arg(IP).arg(Port).arg(Buffer);
		updateTextEdit(1, String);

		if (ui->checkBoxDebug->isChecked())
		{
			int Count = CommonSocket::Keys.count();
			for (int i = 0;i < Count;i++)
			{
				if (CommonSocket::Keys.at(i) == Buffer)
				{
					sendData(IP, Port, CommonSocket::Values.at(i));
					break;
				}
			}
		}
	}
}

void tabUDPServer::on_pushButtonSend_clicked()
{
	QString Data = ui->lineEditData->text();
	if (Data.length() <= 0)
	{
		return;
	}

	sendData(ui->lineEditServerIP->text(), ui->lineEditServerPort->text().toInt(), Data);
}

void tabUDPServer::on_pushButtonListen_clicked()
{
	if (ui->pushButtonListen->text() == "监听")
	{
		bool Checked = udpSocket->bind(QHostAddress::AnyIPv4, ui->lineEditListenPort->text().toInt());
		if (Checked)
		{
			ui->pushButtonListen->setText("断开");
			updateTextEdit(2, "监听成功");
		}
	}
	else
	{
		udpSocket->abort();
		ui->pushButtonListen->setText("监听");
		updateTextEdit(2, "断开监听");
	}
}

void tabUDPServer::on_pushButtonSave_clicked()
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

void tabUDPServer::on_pushButtonClear_clicked()
{
	updateTextEdit(0, "", true);
}

