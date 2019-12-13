#include <QMessageBox>
#include <QDebug>

#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	this->setWindowFlags(Qt::FramelessWindowHint);

	//ui->centralWidget->setMouseTracking(true);

	ui->tabWidget->setCurrentIndex(0);
	InitSignalSlots();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::InitSignalSlots()
{
	connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(on_actionAbout_clicked()));
}

void MainWindow::on_actionAbout_clicked()
{
	QMessageBox::about(NULL, "¹ØÓÚ", "https://github.com/UnfinishedMovement");
}
