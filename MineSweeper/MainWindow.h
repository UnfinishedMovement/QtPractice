#pragma once

#include <QtWidgets/QMainWindow>

#include "MapProperty.h"

#define RectWidth 35
#define RectHeight 35

#define MapStartX 5
#define MapStartY 70

#define TimeStartX 5
#define TimeStartY 30

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();

	void InitGame();
	void ReleaseGame();

	void FirstClickProtected();
	void GameIsSuccess();
	void GameIsFail();

	bool ClickInGameArea(QPoint PointTemp);
	void OpenEmptyArea(QPoint PointTemp);
	bool JudgeGameIsWin();
	
protected:
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *);

private slots:
	void NewGame();
	void Settings();
		void LevelJunior();
		void LevelIntermediate();
		void LevelAdvanced();
	void GameTime();

private:
	void MouseLeftClick(MapProperty* pMineProperty);
	void MouseRightClick(MapProperty* pMineProperty);
	void OpenAroundArea(MapProperty* pMineProperty);

private:
	void DrawMineField();
	void DrawDisplaySpace(QPainter & painter);
	void DrawMineProperty();
	void DrawMineProperty(QPainter & painter, MapProperty* pMineProperty);

private:
	Ui::MainWindow *ui;

	QPixmap BmpBlock;
	QPixmap BmpDigital;
	QPixmap BmpEmoji;

	int nMapRow;
	int nMapColume;
	int nMineCount;
	int nMineLeft;

	QTimer *GameTimer;
	int nTimeCount;

	QVector<QPoint> Mine;//À×µã
	QPoint FirstPoint;
	QVector<QVector<MapProperty*>> MapItem;//À×ÇøÊôÐÔ

	bool bGameIsBegin;
	bool bGameIsEnd;
	bool bGameIsWin;

};
