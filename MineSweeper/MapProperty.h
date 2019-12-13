#pragma once

#include <QPoint.h>

class MapProperty
{
public:
	MapProperty();
	MapProperty(QPoint position);

	QPoint MinePosition;

	int nMineNumber;

	bool bMineDanger;
	bool bMineMarked;
	bool bMineOpen;
	bool bMineRecursive;

};