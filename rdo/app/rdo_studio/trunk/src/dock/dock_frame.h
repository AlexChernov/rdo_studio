/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_frame.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      07.10.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_DOCK_FRAME_H_
#define _RDO_STUDIO_DOCK_FRAME_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_focusable.h"
#include "app/rdo_studio/src/frame/tree_ctrl.h"
// --------------------------------------------------------------------------------

class DockFrame: public DockFocusable
{
public:
	typedef  RDOStudioFrameTreeCtrl  context_type;

	DockFrame(PTR(QWidget) pParent);
	virtual ~DockFrame();

	REF(context_type) getContext();
};

#endif // _RDO_STUDIO_DOCK_FRAME_H_
