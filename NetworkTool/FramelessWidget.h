#pragma once

#include <QtWidgets/QWidget>

#include "MainWindow.h"
#include "ui_FramelessWidget.h"

namespace Ui {
	class FramelessWidget;
}

class FramelessWidget : public QWidget
{
	Q_OBJECT

public:
	explicit FramelessWidget(QWidget* parent = nullptr);
	~FramelessWidget();

	void CreatedSkinMenu(); //�������������˵�
	void InitSignalSlots(); //��ʼ���źŲ�

	static void setStyle(const QString& QssPath);//����Qss

protected:
	bool eventFilter(QObject* watched, QEvent* event); //��װ�¼�������

private slots:
	void on_pushButtonMinimized_clicked();
	void on_pushButtonMaximized_clicked();
	void on_pushButtonExit_clicked();

	void on_actionBlackSkin_clicked();
	void on_actionSilverSkin_clicked();

private:
	QPoint lastPos;
	bool mousePressed;

private:
	QAction* BlackSkin;
	QAction* SilverSkin;

private:
	MainWindow* pMainWindow;
	Ui::FramelessWidget* ui;

};