#pragma once
#pragma execution_character_set("utf-8")

#include <QtWidgets/QMainWindow>

#include "ui_MainWindow.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	void InitSignalSlots(); //≥ı ºªØ–≈∫≈≤€

private slots:
	void on_actionAbout_clicked();

private:
	Ui::MainWindow* ui;
};