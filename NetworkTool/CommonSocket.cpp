#include "CommonSocket.h"

QString CommonSocket::ConfigFileName = "Config.ini";
QString CommonSocket::SendFileName = "Send.txt";
QString CommonSocket::SimulateDeviceFileName = "SimulateDevice.txt";

//TCPClient Config
bool CommonSocket::TCPClientHexSend = false;
bool CommonSocket::TCPClientHexReceive = false;
bool CommonSocket::TCPClientASCII = false;
bool CommonSocket::TCPClientDebug = false;
bool CommonSocket::TCPClientTimingSend = false;
int CommonSocket::TCPClientInterval = 1000;
QString CommonSocket::TCPServerIP = "127.0.0.1";
int CommonSocket::TCPServerPort = 6000;

//TCPServer Config
bool CommonSocket::TCPServerHexSend = false;
bool CommonSocket::TCPServerHexReceive = false;
bool CommonSocket::TCPServerASCII = false;
bool CommonSocket::TCPServerDebug = false;
bool CommonSocket::TCPServerTimingSend = false;
int CommonSocket::TCPServerInterval = 1000;
int CommonSocket::TCPListenPort = 6000;

//UDPServer Config
bool CommonSocket::UDPServerHexSend = false;
bool CommonSocket::UDPServerHexReceive = false;
bool CommonSocket::UDPServerASCII = false;
bool CommonSocket::UDPServerDebug = false;
bool CommonSocket::UDPServerTimingSend = false;
int CommonSocket::UDPServerInterval = 1000;
QString CommonSocket::UDPServerIP = "127.0.0.1";
int CommonSocket::UDPServerPort = 6000;
int CommonSocket::UDPListenPort = 6000;

QStringList CommonSocket::Intervals = QStringList() << "1" << "10" << "20" << "50" << "100" << "200" << "300" << "500" << "1000" << "1500" << "2000" << "3000" << "5000" << "10000";
QStringList CommonSocket::Datas = QStringList();
QStringList CommonSocket::Keys = QStringList();
QStringList CommonSocket::Values = QStringList();

void CommonSocket::readConfig()
{
	if (!checkConfig())
	{
		return;
	}

	QString FileName = QString("%1/%2").arg(qApp->applicationDirPath()).arg(ConfigFileName);
	QSettings Set(FileName, QSettings::IniFormat);

	Set.beginGroup("TCPClientConfig");
	CommonSocket::TCPClientHexSend = Set.value("TCPClientHexSend").toBool();
	CommonSocket::TCPClientHexReceive = Set.value("TCPClientHexReceive").toBool();
	CommonSocket::TCPClientASCII = Set.value("TCPClientASCII").toBool();
	CommonSocket::TCPClientDebug = Set.value("TCPClientDebug").toBool();
	CommonSocket::TCPClientTimingSend = Set.value("TCPClientTimingSend").toBool();
	CommonSocket::TCPClientInterval = Set.value("TCPClientInterval").toInt();
	CommonSocket::TCPServerIP = Set.value("TCPServerIP").toString();
	CommonSocket::TCPServerPort = Set.value("TCPServerPort").toInt();
	Set.endGroup();

	Set.beginGroup("TCPServerConfig");
	CommonSocket::TCPServerHexSend = Set.value("TCPServerHexSend").toBool();
	CommonSocket::TCPServerHexReceive = Set.value("TCPServerHexReceive").toBool();
	CommonSocket::TCPServerASCII = Set.value("TCPServerASCII").toBool();
	CommonSocket::TCPServerDebug = Set.value("TCPServerDebug").toBool();
	CommonSocket::TCPServerTimingSend = Set.value("TCPServerTimingSend").toBool();
	CommonSocket::TCPServerInterval = Set.value("TCPServerInterval").toInt();
	CommonSocket::TCPListenPort = Set.value("TCPListenPort").toInt();
	Set.endGroup();

	Set.beginGroup("UDPServerConfig");
	CommonSocket::UDPServerHexSend = Set.value("UDPServerHexSend").toBool();
	CommonSocket::UDPServerHexReceive = Set.value("UDPServerHexReceive").toBool();
	CommonSocket::UDPServerASCII = Set.value("UDPServerASCII").toBool();
	CommonSocket::UDPServerDebug = Set.value("UDPServerDebug").toBool();
	CommonSocket::UDPServerTimingSend = Set.value("UDPServerTimingSend").toBool();
	CommonSocket::UDPServerInterval = Set.value("UDPServerInterval").toInt();
	CommonSocket::UDPServerIP = Set.value("UDPServerIP").toString();
	CommonSocket::UDPServerPort = Set.value("UDPServerPort").toInt();
	CommonSocket::UDPListenPort = Set.value("UDPListenPort").toInt();
	Set.endGroup();
}

void CommonSocket::writeConfig()
{
	QString FileName = QString("%1/%2").arg(qApp->applicationDirPath()).arg(ConfigFileName);
	QSettings Set(FileName, QSettings::IniFormat);

	Set.beginGroup("TCPClientConfig");
	Set.setValue("TCPClientHexSend", CommonSocket::TCPClientHexSend);
	Set.setValue("TCPClientHexReceive", CommonSocket::TCPClientHexReceive);
	Set.setValue("TCPClientASCII", CommonSocket::TCPClientASCII);
	Set.setValue("TCPClientDebug", CommonSocket::TCPClientDebug);
	Set.setValue("TCPClientTimingSend", CommonSocket::TCPClientTimingSend);
	Set.setValue("TCPClientInterval", CommonSocket::TCPClientInterval);
	Set.setValue("TCPServerIP", CommonSocket::TCPServerIP);
	Set.setValue("TCPServerPort", CommonSocket::TCPServerPort);
	Set.endGroup();

	Set.beginGroup("TCPServerConfig");
	Set.setValue("TCPServerHexSend", CommonSocket::TCPServerHexSend);
	Set.setValue("TCPServerHexReceive", CommonSocket::TCPServerHexReceive);
	Set.setValue("TCPServerASCII", CommonSocket::TCPServerASCII);
	Set.setValue("TCPServerDebug", CommonSocket::TCPServerDebug);
	Set.setValue("TCPServerTimingSend", CommonSocket::TCPServerTimingSend);
	Set.setValue("TCPServerInterval", CommonSocket::TCPServerInterval);
	Set.setValue("TCPListenPort", CommonSocket::TCPListenPort);
	Set.endGroup();

	Set.beginGroup("UDPServerConfig");
	Set.setValue("UDPServerHexSend", CommonSocket::UDPServerHexSend);
	Set.setValue("UDPServerHexReceive", CommonSocket::UDPServerHexReceive);
	Set.setValue("UDPServerASCII", CommonSocket::UDPServerASCII);
	Set.setValue("UDPServerDebug", CommonSocket::UDPServerDebug);
	Set.setValue("UDPServerTimingSend", CommonSocket::UDPServerTimingSend);
	Set.setValue("UDPServerInterval", CommonSocket::UDPServerInterval);
	Set.setValue("UDPServerIP", CommonSocket::UDPServerIP);
	Set.setValue("UDPServerPort", CommonSocket::UDPServerPort);
	Set.setValue("UDPListenPort", CommonSocket::UDPListenPort);
	Set.endGroup();
}

bool CommonSocket::checkConfig()
{
	QString FileName = QString("%1/%2").arg(qApp->applicationDirPath()).arg(ConfigFileName);
	QFile File(FileName);
	if (File.size() == 0)
	{
		writeConfig();
		return false;
	}

	if (File.open(QFile::ReadOnly))
	{
		bool check = true;
		while (!File.atEnd())
		{
			QString Line = File.readLine();
			Line = Line.replace("\r", "");
			Line = Line.replace("\n", "");
			QStringList List = Line.split("=");

			if (List.count() == 2)
			{
				if (List.at(1) == "")
				{
					check = false;
					break;
				}
			}
		}

		if (!check)
		{
			writeConfig();
			return false;
		}
	}
	else
	{
		writeConfig();
		return false;
	}
	return true;
}

void CommonSocket::readSendData()
{
	CommonSocket::Datas.clear();
	QString FileName = QString("%1/%2").arg(qApp->applicationDirPath()).arg(SimulateDeviceFileName);
	QFile File(FileName);
	if (File.size() > 0 && File.open(QFile::ReadOnly | QIODevice::Text))
	{
		while (!File.atEnd())
		{
			QString Line = File.readLine();
			Line = Line.trimmed();
			Line = Line.replace("\r", "");
			Line = Line.replace("\n", "");
			if (!Line.isEmpty())
			{
				QStringList List = Line.split(";");
				QString Key = List.at(0);
				QString Value;
				for (int i = 0;i < List.count();i++)
				{
					Value += QString("%1;").arg(List.at(1));
				}

				Value = Value.mid(0, Value.length() - 1);
				CommonSocket::Keys.append(Key);
				CommonSocket::Values.append(Value);
			}
		}
	}
}

void CommonSocket::readDeviceData()
{
	CommonSocket::Keys.clear();
	CommonSocket::Values.clear();
	QString FileName = QString("%1/%2").arg(qApp->applicationDirPath()).arg(CommonSocket::SimulateDeviceFileName);
	QFile File(FileName);
	if (File.size() > 0 && File.open(QFile::ReadOnly | QIODevice::Text))
	{
		while (!File.atEnd())
		{
			QString Line = File.readLine();
			Line = Line.trimmed();
			Line = Line.replace("\r", "");
			Line = Line.replace("\n", "");
			if (!Line.isEmpty())
			{
				QStringList List = Line.split(";");
				QString Key = List.at(0);
				QString Value;
				for (int i = 0;i < List.count();i++)
				{
					Value += QString("%1").arg(List.at(i));
				}

				Value = Value.mid(0, Value.length() - 1);
				CommonSocket::Keys.append(Key);
				CommonSocket::Values.append(Value);
			}
		}
		File.close();
	}
}

char CommonSocket::charToHexChar(char ch)
{
	if ((ch >= '0') && (ch <= '9'))
	{
		return ch - 0x30;
	}
	else if ((ch >= 'A') && (ch <= 'F'))
	{
		return ch - 'A' + 10;
	}
	else if ((ch >= 'a') && (ch <= 'f'))
	{
		return ch - 'a' + 10;
	}
	else
	{
		return (-1);
	}
}

int CommonSocket::stringHexToDecimal(const QString& StringHex)
{
	bool Temp;
	return StringHex.toInt(&Temp, 16);
}

QString CommonSocket::decimalToStringHex(int Decimal)
{
	QString temp = QString::number(Decimal, 16);
	if (temp.length() == 1) {
		temp = "0" + temp;
	}

	return temp;
}

QByteArray CommonSocket::hexStringToByteArray(const QString& String)
{
	QByteArray SendData;
	int HexData, LowHexData;
	int HexDataLength = 0;
	int Length = String.length();
	SendData.resize(Length / 2);
	char lstr, hstr;

	for (int i = 0; i < Length;) {
		hstr = String.at(i).toLatin1();
		if (hstr == ' ') {
			i++;
			continue;
		}

		i++;
		if (i >= Length) {
			break;
		}

		lstr = String.at(i).toLatin1();
		HexData = charToHexChar(hstr);
		LowHexData = charToHexChar(lstr);

		if ((HexData == 16) || (LowHexData == 16)) {
			break;
		}
		else {
			HexData = HexData * 16 + LowHexData;
		}

		i++;
		SendData[HexDataLength] = (char)HexData;
		HexDataLength++;
	}

	SendData.resize(HexDataLength);
	return SendData;
}

QByteArray CommonSocket::asciiStringToByteArray(const QString& String)
{
	QByteArray Buffer;
	int len = String.length();
	QString Letter;
	QString Hex;

	for (int i = 0; i < len; i++) {
		Letter = String.at(i);

		if (Letter == "\\") {
			i++;
			Letter = String.mid(i, 1);

			if (Letter == "x") {
				i++;
				Hex = String.mid(i, 2);
				Buffer.append(stringHexToDecimal(Hex));
				i++;
				continue;
			}
			else if (Letter == "N") {
				i++;
				Hex = String.mid(i, 1);

				if (Hex == "U") {
					i++;
					Hex = String.mid(i, 1);

					if (Hex == "L") {           //NUL=0x00
						Buffer.append((char)0x00);
						continue;
					}
				}
				else if (Hex == "A") {
					i++;
					Hex = String.mid(i, 1);

					if (Hex == "K") {           //NAK=0x15
						Buffer.append(0x15);
						continue;
					}
				}
			}
			else if (Letter == "S") {
				i++;
				Hex = String.mid(i, 1);

				if (Hex == "O") {
					i++;
					Hex = String.mid(i, 1);

					if (Hex == "H") {           //SOH=0x01
						Buffer.append(0x01);
						continue;
					}
					else {                    //SO=0x0E
						Buffer.append(0x0E);
						i--;
						continue;
					}
				}
				else if (Hex == "T") {
					i++;
					Hex = String.mid(i, 1);

					if (Hex == "X") {           //STX=0x02
						Buffer.append(0x02);
						continue;
					}
				}
				else if (Hex == "I") {        //SI=0x0F
					Buffer.append(0x0F);
					continue;
				}
				else if (Hex == "Y") {
					i++;
					Hex = String.mid(i, 1);

					if (Hex == "N") {           //SYN=0x16
						Buffer.append(0x16);
						continue;
					}
				}
				else if (Hex == "U") {
					i++;
					Hex = String.mid(i, 1);

					if (Hex == "B") {           //SUB=0x1A
						Buffer.append(0x1A);
						continue;
					}
				}
			}
			else if (Letter == "E") {
				i++;
				Hex = String.mid(i, 1);

				if (Hex == "T") {
					i++;
					Hex = String.mid(i, 1);

					if (Hex == "X") {           //ETX=0x03
						Buffer.append(0x03);
						continue;
					}
					else if (Hex == "B") {    //ETB=0x17
						Buffer.append(0x17);
						continue;
					}
				}
				else if (Hex == "O") {
					i++;
					Hex = String.mid(i, 1);

					if (Hex == "T") {           //EOT=0x04
						Buffer.append(0x04);
						continue;
					}
				}
				else if (Hex == "N") {
					i++;
					Hex = String.mid(i, 1);

					if (Hex == "Q") {           //ENQ=0x05
						Buffer.append(0x05);
						continue;
					}
				}
				else if (Hex == "M") {        //EM=0x19
					Buffer.append(0x19);
					continue;
				}
				else if (Hex == "S") {
					i++;
					Hex = String.mid(i, 1);

					if (Hex == "C") {           //ESC=0x1B
						Buffer.append(0x1B);
						continue;
					}
				}
			}
			else if (Letter == "A") {
				i++;
				Hex = String.mid(i, 1);

				if (Hex == "C") {
					i++;
					Hex = String.mid(i, 1);

					if (Hex == "K") {           //ACK=0x06
						Buffer.append(0x06);
						continue;
					}
				}
			}
			else if (Letter == "B") {
				i++;
				Hex = String.mid(i, 1);

				if (Hex == "E") {
					i++;
					Hex = String.mid(i, 1);

					if (Hex == "L") {           //BEL=0x07
						Buffer.append(0x07);
						continue;
					}
				}
				else if (Hex == "S") {        //BS=0x08
					Buffer.append(0x08);
					continue;
				}
			}
			else if (Letter == "C") {
				i++;
				Hex = String.mid(i, 1);

				if (Hex == "R") {               //CR=0x0D
					Buffer.append(0x0D);
					continue;
				}
				else if (Hex == "A") {
					i++;
					Hex = String.mid(i, 1);

					if (Hex == "N") {           //CAN=0x18
						Buffer.append(0x18);
						continue;
					}
				}
			}
			else if (Letter == "D") {
				i++;
				Hex = String.mid(i, 1);

				if (Hex == "L") {
					i++;
					Hex = String.mid(i, 1);

					if (Hex == "E") {           //DLE=0x10
						Buffer.append(0x10);
						continue;
					}
				}
				else if (Hex == "C") {
					i++;
					Hex = String.mid(i, 1);

					if (Hex == "1") {           //DC1=0x11
						Buffer.append(0x11);
						continue;
					}
					else if (Hex == "2") {    //DC2=0x12
						Buffer.append(0x12);
						continue;
					}
					else if (Hex == "3") {    //DC3=0x13
						Buffer.append(0x13);
						continue;
					}
					else if (Hex == "4") {    //DC2=0x14
						Buffer.append(0x14);
						continue;
					}
				}
			}
			else if (Letter == "F") {
				i++;
				Hex = String.mid(i, 1);

				if (Hex == "F") {               //FF=0x0C
					Buffer.append(0x0C);
					continue;
				}
				else if (Hex == "S") {        //FS=0x1C
					Buffer.append(0x1C);
					continue;
				}
			}
			else if (Letter == "H") {
				i++;
				Hex = String.mid(i, 1);

				if (Hex == "T") {               //HT=0x09
					Buffer.append(0x09);
					continue;
				}
			}
			else if (Letter == "L") {
				i++;
				Hex = String.mid(i, 1);

				if (Hex == "F") {               //LF=0x0A
					Buffer.append(0x0A);
					continue;
				}
			}
			else if (Letter == "G") {
				i++;
				Hex = String.mid(i, 1);

				if (Hex == "S") {               //GS=0x1D
					Buffer.append(0x1D);
					continue;
				}
			}
			else if (Letter == "R") {
				i++;
				Hex = String.mid(i, 1);

				if (Hex == "S") {               //RS=0x1E
					Buffer.append(0x1E);
					continue;
				}
			}
			else if (Letter == "U") {
				i++;
				Hex = String.mid(i, 1);

				if (Hex == "S") {               //US=0x1F
					Buffer.append(0x1F);
					continue;
				}
			}
			else if (Letter == "V") {
				i++;
				Hex = String.mid(i, 1);

				if (Hex == "T") {               //VT=0x0B
					Buffer.append(0x0B);
					continue;
				}
			}
			else if (Letter == "\\") {
				//如果连着的是多个\\则对应添加\对应的16进制0x5C
				Buffer.append(0x5C);
				continue;
			}
			else {
				//将对应的\[前面的\\也要加入
				Buffer.append(0x5C);
				Buffer.append(Letter.toLatin1());
				continue;
			}
		}

		Buffer.append(String.mid(i, 1).toLatin1());

	}

	return Buffer;
}

QString CommonSocket::byteArrayToHexString(const QByteArray& data)
{
	QString Temp;
	QString Hex = data.toHex();

	for (int i = 0;i < Hex.length();i = i + 2)
	{
		Temp += Hex.mid(i, 2) + " ";
	}

	return Temp.trimmed().toUpper();
}

QString CommonSocket::byteArrayToASCIIString(const QByteArray& data)
{
	QString Temp;
	int len = data.size();

	for (int i = 0; i < len; i++) {
		//0x20为空格,空格以下都是不可见字符
		char b = data.at(i);

		if (0x00 == b) {
			Temp += QString("\\NUL");
		}
		else if (0x01 == b) {
			Temp += QString("\\SOH");
		}
		else if (0x02 == b) {
			Temp += QString("\\STX");
		}
		else if (0x03 == b) {
			Temp += QString("\\ETX");
		}
		else if (0x04 == b) {
			Temp += QString("\\EOT");
		}
		else if (0x05 == b) {
			Temp += QString("\\ENQ");
		}
		else if (0x06 == b) {
			Temp += QString("\\ACK");
		}
		else if (0x07 == b) {
			Temp += QString("\\BEL");
		}
		else if (0x08 == b) {
			Temp += QString("\\BS");
		}
		else if (0x09 == b) {
			Temp += QString("\\HT");
		}
		else if (0x0A == b) {
			Temp += QString("\\LF");
		}
		else if (0x0B == b) {
			Temp += QString("\\VT");
		}
		else if (0x0C == b) {
			Temp += QString("\\FF");
		}
		else if (0x0D == b) {
			Temp += QString("\\CR");
		}
		else if (0x0E == b) {
			Temp += QString("\\SO");
		}
		else if (0x0F == b) {
			Temp += QString("\\SI");
		}
		else if (0x10 == b) {
			Temp += QString("\\DLE");
		}
		else if (0x11 == b) {
			Temp += QString("\\DC1");
		}
		else if (0x12 == b) {
			Temp += QString("\\DC2");
		}
		else if (0x13 == b) {
			Temp += QString("\\DC3");
		}
		else if (0x14 == b) {
			Temp += QString("\\DC4");
		}
		else if (0x15 == b) {
			Temp += QString("\\NAK");
		}
		else if (0x16 == b) {
			Temp += QString("\\SYN");
		}
		else if (0x17 == b) {
			Temp += QString("\\ETB");
		}
		else if (0x18 == b) {
			Temp += QString("\\CAN");
		}
		else if (0x19 == b) {
			Temp += QString("\\EM");
		}
		else if (0x1A == b) {
			Temp += QString("\\SUB");
		}
		else if (0x1B == b) {
			Temp += QString("\\ESC");
		}
		else if (0x1C == b) {
			Temp += QString("\\FS");
		}
		else if (0x1D == b) {
			Temp += QString("\\GS");
		}
		else if (0x1E == b) {
			Temp += QString("\\RS");
		}
		else if (0x1F == b) {
			Temp += QString("\\US");
		}
		else if (0x7F == b) {
			Temp += QString("\\x7F");
		}
		else if (0x5C == b) {
			Temp += QString("\\x5C");
		}
		else if (0x20 >= b) {
			Temp += QString("\\x%1").arg(decimalToStringHex((quint8)b));
		}
		else {
			Temp += QString("%1").arg(b);
		}
	}

	return Temp.trimmed();
}