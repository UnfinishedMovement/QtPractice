#include "MapProperty.h"

MapProperty::MapProperty()
{
	MinePosition = QPoint(-1, -1);

	nMineNumber = 0;
	bMineDanger = false;
	bMineMarked = false;
	bMineOpen = false;
	bMineRecursive = false;
}

MapProperty::MapProperty(QPoint position)
{
	MinePosition = position;
	nMineNumber = 0;
	bMineDanger = false;
	bMineMarked = false;
	bMineOpen = false;
	bMineRecursive = false;
}