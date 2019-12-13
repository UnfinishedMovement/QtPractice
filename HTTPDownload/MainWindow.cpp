#include <QMessageBox>
#include <QDir>

#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButtonStart_clicked()
{
	QString URLSpec = ui->lineEditURL->text().trimmed();
	if (URLSpec.isEmpty())
	{
		QMessageBox::information(this, "错误", "请输入需要下载的URL");
		return;
	}

	QUrl Url = QUrl::fromUserInput(URLSpec);
	if (!Url.isValid())
	{
		QMessageBox::information(this, "错误", QString("无效URL：%1\n错误信息：%2").arg(URLSpec).arg(Url.errorString()));
		return;
	}

	QString TempDir = ui->lineEditPath->text().trimmed();
	if (TempDir.isEmpty())
	{
		QMessageBox::information(this, "错误", "请输入保存文件的目录");
		return;
	}

	QString FullFileName = TempDir + Url.fileName();
	if (QFile::exists(FullFileName))
	{
		QFile::remove(FullFileName);
	}
	
	downloadedFile = new QFile(FullFileName);
	if (!downloadedFile->open(QIODevice::WriteOnly))
	{
		QMessageBox::information(this, "错误", "临时文件打开错误");
	}

	ui->pushButtonStart->setEnabled(false);
	reply = networkManager.get(QNetworkRequest(Url));
	connect(reply, SIGNAL(finished()), this, SLOT(on_finished()));
	connect(reply, SIGNAL(readyRead()), this, SLOT(on_redayRead()));
	connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(on_downloadProgress(qint64, qint64)));
}

void MainWindow::on_pushButtonDefaultPath_clicked()
{
	QString currentPath = QDir::currentPath();
	QDir dir(currentPath);
	QString sub = "temp";
	dir.mkdir(sub);
	ui->lineEditPath->setText(currentPath + "/" + sub + "/");
}

void MainWindow::on_finished()
{
	QFileInfo FileInfo;
	FileInfo.setFile(downloadedFile->fileName());
	downloadedFile->close();
	delete downloadedFile;
	downloadedFile = Q_NULLPTR;
	reply->deleteLater();
	reply = Q_NULLPTR;
	if (ui->checkBoxClose->isChecked())
	{
		this->close();
	}
}

void MainWindow::on_redayRead()
{
	downloadedFile->write(reply->readAll());
}

void MainWindow::on_downloadProgress(qint64 bytesRead, qint64 totalBytes)
{
	ui->progressBar->setMaximum(totalBytes);
	ui->progressBar->setValue(bytesRead);
}

