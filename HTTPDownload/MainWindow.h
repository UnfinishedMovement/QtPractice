#pragma once

#include <QtWidgets/QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>

#include "ui_MainWIndow.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private slots:;
	void on_pushButtonStart_clicked();
	void on_pushButtonDefaultPath_clicked();

private slots:
	void on_finished();
	void on_redayRead();
	void on_downloadProgress(qint64 bytesRead, qint64 totalBytes);

private:
	QNetworkAccessManager networkManager;
	QNetworkReply* reply;
	QFile* downloadedFile;

	Ui::MainWindow* ui;
};