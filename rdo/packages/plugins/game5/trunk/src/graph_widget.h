/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/graph_widget.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_PLUGIN_GAME_5_GRAPH_WIDGET_H_
#define _RDO_PLUGIN_GAME_5_GRAPH_WIDGET_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QGraphicsView>
#include <QWheelEvent>
// ---------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class GraphWidget : public QGraphicsView
{
Q_OBJECT

public:
	GraphWidget(QWidget* parent = 0);
	~GraphWidget();

	QGraphicsScene* scene;

protected:
	virtual void wheelEvent     (QWheelEvent* wEvent);
	virtual void keyPressEvent  (QKeyEvent* kEvent);
	virtual void keyReleaseEvent(QKeyEvent* kEvent);

	void scaleView(double scaleFactor);
};

#endif // _RDO_PLUGIN_GAME_5_GRAPH_WIDGET_H_