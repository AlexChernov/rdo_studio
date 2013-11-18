/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/plugin_game5_tiles_order_dialog.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_PLUGIN_TEST_GAME_5_TILE_ORDER_DIALOG_H_
#define _RDO_PLUGIN_TEST_GAME_5_TILE_ORDER_DIALOG_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "ui_plugin_game5_tiles_order_dialog.h"
// --------------------------------------------------------------------------------

class TilesOrderDialog
	: public QDialog
	, public Ui_RAOgame5TilesOrderDialog
{
Q_OBJECT
public:
	TilesOrderDialog(QWidget* parent, const std::vector<unsigned int>& vector);
	virtual ~TilesOrderDialog();

signals:
	void tilesOrderCommited(const QString& string);

private:
	QString validatorRegExpPattern(int value);
	std::vector<unsigned int> stringToVector(const QString& string);

private slots:
	void onOkClick();
};

#endif // _RDO_PLUGIN_TEST_GAME_5_TILE_ORDER_DIALOG_H_
