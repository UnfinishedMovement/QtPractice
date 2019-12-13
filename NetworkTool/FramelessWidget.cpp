#include <QHoverEvent>

#include "FramelessWidget.h"

FramelessWidget::FramelessWidget(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::FramelessWidget)
{
	ui->setupUi(this);

	FramelessWidget::setStyle(":/Qss/Resources/Qss/StyleSheet-silvery.qss");

	this->setWindowFlags(Qt::CustomizeWindowHint);

	ui->titleWidget->installEventFilter(this);
	ui->titleWidget->setAttribute(Qt::WA_Hover, true);

	CommonSocket::readConfig();
	CommonSocket::readSendData();
	CommonSocket::readDeviceData();

	pMainWindow = new MainWindow();
	ui->verticalLayout->addWidget(pMainWindow);

	mousePressed = false;

	CreatedSkinMenu();
	InitSignalSlots();
}

FramelessWidget::~FramelessWidget()
{
	delete ui;
}

void FramelessWidget::CreatedSkinMenu()
{
	QMenu* SkinMenu = new QMenu();

	BlackSkin = new QAction();
	SilverSkin = new QAction();

	BlackSkin->setText("Black");
	SilverSkin->setText("Silver");

	SkinMenu->addAction(BlackSkin);
	SkinMenu->addAction(SilverSkin);

	//ui->toolButtonSkin->setPopupMode(QToolButton::InstantPopup);
	//ui->toolButtonSkin->setMenu(SkinMenu);
}

void FramelessWidget::InitSignalSlots()
{
	connect(BlackSkin, SIGNAL(triggered()), this, SLOT(on_actionBlackSkin_clicked()));
	connect(SilverSkin, SIGNAL(triggered()), this, SLOT(on_actionSilverSkin_clicked()));
}

void FramelessWidget::setStyle(const QString& QssPath)
{
	QFile QssFile(QssPath);
	QssFile.open(QFile::ReadOnly);
	qApp->setStyleSheet(QssFile.readAll());
	QssFile.close();
}

bool FramelessWidget::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == ui->titleWidget)
	{
		if (event->type() == QEvent::HoverMove)
		{
			QHoverEvent* hoverEvent = (QHoverEvent*)event;
			QPoint point = hoverEvent->pos();

			int offsetX = point.x() - lastPos.x();
			int offsetY = point.y() - lastPos.y();

			if (mousePressed)
			{
				this->move(this->x() + offsetX, this->y() + offsetY);
			}
		}

		else if (event->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent* mouseEvent = (QMouseEvent*)event;
			lastPos = mouseEvent->pos();

			mousePressed = true;
		}

		else if (event->type() == QEvent::MouseButtonRelease)
		{
			mousePressed = false;
		}

		else if (event->type() == QEvent::MouseButtonDblClick)
		{
			on_pushButtonMaximized_clicked();
		}
	}

	else if (event->type() == QEvent::HoverMove)
	{

	}
	else if (event->type() == QEvent::MouseButtonPress)
	{

	}
	else if (event->type() == QEvent::MouseButtonRelease)
	{

	}
	return QObject::eventFilter(watched, event);
}

void FramelessWidget::on_pushButtonMinimized_clicked()
{
	this->showMinimized();

}

void FramelessWidget::on_pushButtonMaximized_clicked()
{
	this->isMaximized() ? this->showNormal() : this->showMaximized();
}

void FramelessWidget::on_pushButtonExit_clicked()
{
	this->close();
}

void FramelessWidget::on_actionBlackSkin_clicked()
{
	FramelessWidget::setStyle(":/Qss/Resources/Qss/StyleSheet-black.qss");
}

void FramelessWidget::on_actionSilverSkin_clicked()
{
	FramelessWidget::setStyle(":/Qss/Resources/Qss/StyleSheet-silvery.qss");
}