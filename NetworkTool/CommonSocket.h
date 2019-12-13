#pragma once
#pragma execution_character_set("utf-8")

#include <QtWidgets/QMainWindow>
#include <QtCore>

class CommonSocket
{
public:
	static void readConfig();
	static void writeConfig();
	static bool checkConfig();

	static void readSendData();
	static void readDeviceData();

	static char charToHexChar(char ch);
	static int stringHexToDecimal(const QString& StringHex);
	static QString decimalToStringHex(int Decimal);

	static QByteArray hexStringToByteArray(const QString& String);
	static QByteArray asciiStringToByteArray(const QString& String);
	static QString byteArrayToHexString(const QByteArray& data);
	static QString byteArrayToASCIIString(const QByteArray& data);

	static QStringList Intervals;
	static QStringList Datas;
	static QStringList Keys;
	static QStringList Values;

	static QString ConfigFileName;
	static QString SendFileName;
	static QString SimulateDeviceFileName;

	//TCPClient Config
	static bool TCPClientHexSend;
	static bool TCPClientHexReceive;
	static bool TCPClientASCII;
	static bool TCPClientDebug;
	static bool TCPClientTimingSend;
	static int TCPClientInterval;
	static QString TCPServerIP;
	static int TCPServerPort;


	//TCPServer Config
	static bool TCPServerHexSend;
	static bool TCPServerHexReceive;
	static bool TCPServerASCII;
	static bool TCPServerDebug;
	static bool TCPServerTimingSend;
	static int TCPServerInterval;
	static int TCPListenPort;

	//UDPServer Config
	static bool UDPServerHexSend;
	static bool UDPServerHexReceive;
	static bool UDPServerASCII;
	static bool UDPServerDebug;
	static bool UDPServerTimingSend;
	static int UDPServerInterval;
	static QString UDPServerIP;
	static int UDPServerPort;
	static int UDPListenPort;
};