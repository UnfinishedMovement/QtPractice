#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QPaintEvent>
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(NewGame()));
	connect(ui->actionLevelJunior, SIGNAL(triggered()), this, SLOT(LevelJunior()));
	connect(ui->actionLevelIntermediate, SIGNAL(triggered()), this, SLOT(LevelIntermediate()));
	connect(ui->actionLevelAdvanced, SIGNAL(triggered()), this, SLOT(LevelAdvanced()));

	nMapRow = 8;
	nMapColume = 8;
	nMineCount = 10;
	nMineLeft = 10;

	nTimeCount = 0;
	GameTimer = new QTimer(this);
	connect(GameTimer, SIGNAL(timeout()), this, SLOT(GameTime()));

	NewGame();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::InitGame()
{
	Mine.clear();
	for (int i = 0; i < nMineCount; i++)
	{
		qsrand(QTime::currentTime().msec());
		int x = qrand() % nMapColume;
		int y = qrand() % nMapRow;
		while (Mine.contains(QPoint(x, y)) || QPoint(x, y) == FirstPoint)
		{
			x = qrand() % nMapColume;
			y = qrand() % nMapRow;
		}
		Mine.append(QPoint(x, y));
	}

	MapProperty* pMineProperty = NULL;
	for (int i = 0; i < nMapColume; i++)
	{
		QVector<MapProperty*> ColumeMapItem;
		for (int j = 0; j < nMapRow; j++)
		{
			QPoint position = QPoint(i, j);
			pMineProperty = new MapProperty(position);
			if (Mine.contains(position))
			{
				pMineProperty->bMineDanger = true;
			}
			ColumeMapItem.append(pMineProperty);
		}
		MapItem.append(ColumeMapItem);
	}

	for (int i = 0; i < nMapColume; i++)
	{
		for (int j = 0; j < nMapRow; j++)
		{
			if (MapItem[i][j]->bMineDanger)
			{
				continue;
			}
			int CountMine = 0;
			QPoint PositionTemp = QPoint(-1, -1);
			for (int m = -1; m <= 1; m++)
			{
				for (int n = -1; n <= 1; n++)
				{
					if (m == 0 && n == 0)
					{
						continue;
					}
					PositionTemp = QPoint(i + m, j + n);
					if (!ClickInGameArea(PositionTemp))
					{
						continue;
					}
					if (MapItem[i + m][j + n]->bMineDanger)
					{
						CountMine++;
					}
				}
			}
			MapItem[i][j]->nMineNumber = CountMine;
		}
	}
}

void MainWindow::ReleaseGame()
{
	for (int i = 0; i < MapItem.size(); i++)
	{
		for (int j = 0; j < MapItem[i].size(); j++)
		{
			if (MapItem[i][j] != NULL)
			{
				delete MapItem[i][j];
				MapItem[i][j] = NULL;
			}
		}
	}
	FirstPoint = QPoint(-1, -1);
	MapItem.clear();
}

void MainWindow::NewGame()
{
	bGameIsBegin = false;
	bGameIsEnd = false;
	bGameIsWin = false;
	this->GameTimer->stop();
	resize(MapStartX * 2 + nMapColume*RectWidth, MapStartY + 5 + nMapRow*RectWidth);
	FirstClickProtected();
	ReleaseGame();
	InitGame();
}

void MainWindow::FirstClickProtected()
{/*
	while (FirstPoint == QPoint(-1, -1))
	{
		;
	}*/
}
//什么都没写什么都没写什么都没写什么都没写什么都没写
void MainWindow::GameIsSuccess()
{
	bGameIsEnd = true;
	bGameIsWin = true;
	this->GameTimer->stop();
	QMessageBox::information(NULL, "SUCCESS!", "Congratulations!", QMessageBox::Yes);
}

void MainWindow::GameIsFail()
{
	bGameIsEnd = true;
	bGameIsWin = false;
	this->GameTimer->stop();

}

void MainWindow::MouseLeftClick(MapProperty* pMineProperty)
{
	/*
	if (FirstPoint == QPoint(-1, -1))
	{
	FirstPoint = pMineProperty->MinePosition;
	}
	*/
	if (!pMineProperty->bMineMarked && !pMineProperty->bMineOpen)
	{
		pMineProperty->bMineOpen = true;
		if (pMineProperty->bMineDanger)
		{
			GameIsFail();
			return;
		}
		else
		{
			if (pMineProperty->nMineNumber == 0)
			{
				OpenEmptyArea(pMineProperty->MinePosition);
			}
			if (JudgeGameIsWin())
			{
				GameIsSuccess();
				return;
			}
		}
	}
}

void MainWindow::MouseRightClick(MapProperty* pMineProperty)
{
	if (pMineProperty->bMineMarked)
	{
		pMineProperty->bMineMarked = false;
		nMineLeft++;
	}
	else if (!pMineProperty->bMineOpen)
	{
		pMineProperty->bMineMarked = true;
		if (nMineLeft > 0)
		{
			nMineLeft--;
		}

		if (JudgeGameIsWin())
		{
			GameIsSuccess();
			return;
		}
	}
}

void MainWindow::OpenAroundArea(MapProperty* pMineProperty)
{
	int nCountFlag = 0;
	QPoint PointNewTemp = QPoint(-1, -1);
	MapProperty* pMineProperty2 = NULL;

	for (int i = -1;i <= 1;i++)
	{
		for (int j = -1;j <= 1;j++)
		{
			if (i == 0 && j == 0)
			{
				continue;
			}
			PointNewTemp = pMineProperty->MinePosition + QPoint(i, j);
			if (!ClickInGameArea(PointNewTemp))
			{
				continue;
			}
			pMineProperty2 = MapItem[PointNewTemp.x()][PointNewTemp.y()];
			if (pMineProperty2->bMineMarked)
			{
				nCountFlag++;
			}
		}
	}

	if (pMineProperty->nMineNumber == nCountFlag)
	{
		for (int i = -1;i <= 1;i++)
		{
			for (int j = -1;j <= 1;j++)
			{
				if (i == 0 && j == 0)
				{
					continue;
				}
				PointNewTemp = pMineProperty->MinePosition + QPoint(i, j);
				if (!ClickInGameArea(PointNewTemp))
				{
					continue;
				}
				pMineProperty2 = MapItem[PointNewTemp.x()][PointNewTemp.y()];
				MouseLeftClick(pMineProperty2);
			}
		}
	}
}

void MainWindow::OpenEmptyArea(QPoint PointTemp)
{
	QPoint PointNewTemp = QPoint(-1, -1);
	MapProperty*  pMineProperty = NULL;
	for (int i = -1;i <= 1;i++)
	{
		for (int j = -1;j <= 1;j++)
		{
			if (i == 0 && j == 0)
			{
				continue;
			}

			PointNewTemp = PointTemp + QPoint(i, j);
			if (!ClickInGameArea(PointNewTemp))
			{
				continue;
			}

			pMineProperty = MapItem[PointNewTemp.x()][PointNewTemp.y()];
			if (!pMineProperty->bMineOpen && !pMineProperty->bMineMarked)
			{
				pMineProperty->bMineOpen = true;
			}

			if (pMineProperty->nMineNumber == 0 && !pMineProperty->bMineRecursive)
			{
				pMineProperty->bMineRecursive = true;
				OpenEmptyArea(PointNewTemp);
			}
		}
	}
}

bool MainWindow::JudgeGameIsWin()
{
	bool bTempFlag=true;
	MapProperty* pMineProperty = NULL;
	for (int i = 0; i < MapItem.size(); i++)
	{
		for (int j = 0; j < MapItem[i].size(); j++)
		{
			pMineProperty = MapItem[i][j];
			if (pMineProperty->bMineDanger)
			{
				if (!pMineProperty->bMineMarked)
				{
					bTempFlag = false;
					return bGameIsEnd;

				}
			}
			else
			{
				if (!pMineProperty->bMineOpen)
				{
					bTempFlag = false;
					return bGameIsEnd;
				}
			}
		}
	}
	bGameIsEnd = bTempFlag;
	return bGameIsEnd;
}

bool MainWindow::ClickInGameArea(QPoint PointTemp)
{
	if (PointTemp.x() >= 0 && PointTemp.x() < nMapColume&&PointTemp.y() >= 0 && PointTemp.y() < nMapRow)
	{
		return true;
	}
	return false;
}

void MainWindow::paintEvent(QPaintEvent *event)  
{
	BmpBlock.load("../MineSweeper/images/block_35_490.bmp");
	BmpDigital.load("../MineSweeper/images/digital_32_300.bmp");
	BmpEmoji.load("../MineSweeper/images/emoji_31_93.bmp");

	QPainter painter(this);
	
	//DrawMineField();
	DrawDisplaySpace(painter);
	DrawMineProperty();
	update();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
	if (!(event->pos().x() >= (width() / 2 - 15) && event->pos().x() <= (width() / 2 + 16) && event->pos().y() >= TimeStartY &&event->pos().y() <= (TimeStartY + 31)))
	{
		QPoint PointTemp;
		PointTemp.setX((event->pos().x() - MapStartX) / RectWidth);
		PointTemp.setY((event->pos().y() - MapStartY) / RectHeight);

		if (!ClickInGameArea(PointTemp))
		{
			return;
		}

		MapProperty* pMineProperty = MapItem[PointTemp.x()][PointTemp.y()];
		if (bGameIsEnd)
		{
			return;
		}
		else
		{
			if (event->buttons() == (Qt::LeftButton | Qt::RightButton))
			{
				OpenAroundArea(pMineProperty);
				return;
			}

			else if (event->button() == Qt::LeftButton)
			{
				if (!bGameIsBegin)
				{
					bGameIsBegin = true;
					this->GameTimer->start(1000);
				}
				MouseLeftClick(pMineProperty);
				return;
			}

			else if (event->button() == Qt::RightButton)
			{
				MouseRightClick(pMineProperty);
				return;
			}
		}
	}
	else
	{
		NewGame();
	}
}
//settings暂时没东西
void MainWindow::Settings()
{
	;
}

void MainWindow::LevelJunior()
{
	nMapRow = 8;
	nMapColume = 8;
	nMineCount = 10;
	nMineLeft = 10;
	nTimeCount = 0;
	this->GameTimer->stop();
	NewGame();
}

void MainWindow::LevelIntermediate()
{
	nMapRow = 16;
	nMapColume = 16;
	nMineCount = 40;
	nMineLeft = 40;
	nTimeCount = 0;
	this->GameTimer->stop();
	NewGame();
}

void MainWindow::LevelAdvanced()
{
	nMapRow = 16;
	nMapColume = 30;
	nMineCount = 99;
	nMineLeft = 99;
	nTimeCount = 0;
	this->GameTimer->stop();
	NewGame();
}

void MainWindow::GameTime()
{
	nTimeCount++;
}

void MainWindow::DrawDisplaySpace(QPainter & painter)
{
	int nOnesPlace, nTensPlace, nHundredsPlace;

	//绘制剩余雷数
	int nMineLeftTemp = nMineLeft;
	nHundredsPlace = nMineLeftTemp / 100;
	nTensPlace = nMineLeftTemp / 10 - nHundredsPlace * 10;
	nOnesPlace = nMineLeftTemp - nTensPlace * 10 - nHundredsPlace * 100;
	painter.drawPixmap(0 + TimeStartX, TimeStartY, BmpDigital, nHundredsPlace * 25, 0, 25, 32);
	painter.drawPixmap(25 + TimeStartX, TimeStartY, BmpDigital, nTensPlace * 25, 0, 25, 32);
	painter.drawPixmap(50 + TimeStartX, TimeStartY, BmpDigital, nOnesPlace * 25, 0, 25, 32);

	//绘制已用时间
	int nTimeCountTemp = this->nTimeCount;
	nHundredsPlace = nTimeCountTemp / 100;
	nTensPlace = nTimeCountTemp / 10 - nHundredsPlace * 10;
	nOnesPlace = nTimeCountTemp - nTensPlace * 10 - nHundredsPlace * 100;
	painter.drawPixmap(width() - TimeStartX - 75, TimeStartY, BmpDigital, nHundredsPlace * 25, 0, 25, 32);
	painter.drawPixmap(width() - TimeStartX - 50, TimeStartY, BmpDigital, nTensPlace * 25, 0, 25, 32);
	painter.drawPixmap(width() - TimeStartX - 25, TimeStartY, BmpDigital, nOnesPlace * 25, 0, 25, 32);

	//绘制Emoji表情
	if (bGameIsEnd)
	{
		if (bGameIsWin)
		{
			painter.drawPixmap(width() / 2 - 15, TimeStartY, BmpEmoji, 1 * 31, 0, 31, 31);
		}
		else
		{
			painter.drawPixmap(width() / 2 - 15, TimeStartY, BmpEmoji, 2 * 31, 0, 31, 31);
		}
	}
	else
	{
		painter.drawPixmap(width() / 2 - 15, TimeStartY, BmpEmoji, 0, 0, 31, 31);
	}
}

void MainWindow::DrawMineField()
{
	QPainter painter(this);
	painter.setBrush(QColor(123, 123, 123));
	painter.drawRect(0, 0, width(), height());
}

void MainWindow::DrawMineProperty()
{
	QPainter painter(this);

	for (int i = 0; i < nMapColume; i++)
	{
		for (int j = 0; j < nMapRow; j++)
		{
			DrawMineProperty(painter, MapItem[i][j]);
		}
	}
}

void MainWindow::DrawMineProperty(QPainter & painter, MapProperty* pMineProperty)
{
	if (pMineProperty->bMineOpen)
	{
		//被打开雷显示为红色，被打开的安全格子显示其周围雷数
		if (pMineProperty->bMineDanger)
		{
			painter.drawPixmap(MapStartX + pMineProperty->MinePosition.x()*RectWidth, MapStartY + pMineProperty->MinePosition.y()*RectHeight, BmpBlock, 11 * 35, 0, 35, 35);
		}
		else
		{
			painter.drawPixmap(MapStartX + pMineProperty->MinePosition.x()*RectWidth, MapStartY + pMineProperty->MinePosition.y()*RectHeight, BmpBlock, pMineProperty->nMineNumber * 35, 0, 35, 35);
		}
	}

	else
	{
		if (!bGameIsWin)
		{
			//当游戏未结束时，标记格子显示为红旗，未标记格子显示为普通
			if (pMineProperty->bMineMarked)
			{
				painter.drawPixmap(MapStartX + pMineProperty-> MinePosition.x()*RectWidth, MapStartY + pMineProperty->MinePosition.y()*RectHeight, BmpBlock, 12 * 35, 0, 35, 35);
			}
			else
			{
				painter.drawPixmap(MapStartX + pMineProperty->MinePosition.x()*RectWidth, MapStartY + pMineProperty->MinePosition.y()*RectHeight, BmpBlock, 9 * 35, 0, 35, 35);
			}
		}
		
		else
		{
			//当游戏结束时，标记正确格子显示为红旗，标记错误格子显示为错误雷
			if (pMineProperty->bMineMarked)
			{
				if (pMineProperty->bMineDanger)
				{
					painter.drawPixmap(MapStartX + pMineProperty->MinePosition.x()*RectWidth, MapStartY + pMineProperty->MinePosition.y()*RectHeight, BmpBlock, 12 * 35, 0, 35, 35);
				}
				else
				{
					painter.drawPixmap(MapStartX + pMineProperty->MinePosition.x()*RectWidth, MapStartY + pMineProperty->MinePosition.y()*RectHeight, BmpBlock, 13 * 35, 0, 35, 35);
				}
			}
			//当游戏结束时，未被标记的雷显示，其余格子不显示
			else
			{
				if (pMineProperty->bMineDanger)
				{
					painter.drawPixmap(MapStartX + pMineProperty->MinePosition.x()*RectWidth, MapStartY + pMineProperty->MinePosition.y()*RectHeight, BmpBlock, 10 * 35, 0, 35, 35);
				}
				else
				{
					painter.drawPixmap(MapStartX + pMineProperty->MinePosition.x()*RectWidth, MapStartY + pMineProperty->MinePosition.y()*RectHeight, BmpBlock, 9 * 35, 0, 35, 35);
				}
			}
		}
	}
}