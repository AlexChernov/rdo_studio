/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/frame/manager.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      28.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_FRAME_MANAGER_H_
#define _RDO_STUDIO_MFC_FRAME_MANAGER_H_

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <map>
#include <memory>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/frame/document.h"
#include "app/rdo_studio_mfc/src/frame/view.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameManager
// --------------------------------------------------------------------------------
OPEN_RDO_ANIMATION_NAMESPACE
struct RDOFrame;
CLOSE_RDO_ANIMATION_NAMESPACE

class RDOStudioFrameManager
{
public:
	struct Frame
	{
		struct Area
		{
			tstring        m_name;
			Gdiplus::Rect  m_rect;
		};
		typedef  std::vector<PTR(Area)>  AreaList;

		 Frame();
		~Frame();

		HTREEITEM               m_hitem;
		tstring                 m_name;
		PTR(RDOStudioFrameDoc)  m_pDoc;
		PTR(RDOStudioFrameView) m_pView;
		AreaList                m_areaList;

	private:
		void clear();
	};

private:
	class FrameDocTemplate: public CMultiDocTemplate
	{
	public:
		FrameDocTemplate(UINT nIDResource, PTR(CRuntimeClass) pDocClass, PTR(CRuntimeClass) pFrameClass, PTR(CRuntimeClass) pViewClass);

		virtual PTR(CFrameWnd) CreateNewFrame(PTR(CDocument) pDoc, PTR(CFrameWnd) pOther);
	};
	PTR(FrameDocTemplate) frameDocTemplate;

	typedef  std::vector<PTR(Frame)>                  FrameList;
	typedef  std::map<tstring, PTR(Gdiplus::Bitmap)>  BitmapList;

	FrameList  m_frameList;
	BitmapList m_bitmapList;

	CDC dcBmp;
	CDC dcMask;

	ruint lastShowedFrame;
	ruint currentShowingFrame;
	rbool changed;

public:
	RDOStudioFrameManager();
	virtual ~RDOStudioFrameManager();

	void insertItem(CREF(tstring) name);

	ruint findFrameIndex(const HTREEITEM          hitem) const;
	ruint findFrameIndex(CPTR(RDOStudioFrameDoc)  pDoc ) const;
	ruint findFrameIndex(CPTR(RDOStudioFrameView) pView) const;

	CREF(tstring)           getFrameName      (ruint index) const;
	PTR(RDOStudioFrameDoc)  getFrameDoc       (ruint index) const;
	PTR(RDOStudioFrameView) getFrameView      (ruint index) const;
	ruint                   count             () const;
	rbool                   isChanged         ();

	void                    areaDown          (ruint frameIndex, CREF(Gdiplus::Point) point) const;

	PTR(RDOStudioFrameDoc)  connectFrameDoc   (ruint index);
	void                    disconnectFrameDoc(CPTR(RDOStudioFrameDoc) pDoc);
	PTR(RDOStudioFrameDoc)  getFirstExistDoc  () const;
	void                    closeAll          ();
	void                    clear             ();

	void bmp_insert( CREF(tstring) name );
	void bmp_clear();

	void expand() const;

	rbool isValidFrameDoc( const RDOStudioFrameDoc* const frame ) const;

	int   getLastShowedFrame() const              { return lastShowedFrame; };
	void  setLastShowedFrame(ruint value);
	void  setCurrentShowingFrame(ruint value );
	void  resetCurrentShowingFrame(ruint value);
	void  showFrame(CPTRC(rdoAnimation::RDOFrame) pFrame, ruint index);
	void  showNextFrame();
	void  showPrevFrame();
	void  showFrame(ruint index);
	rbool canShowNextFrame() const;
	rbool canShowPrevFrame() const;

	void updateStyles() const;
};

#endif // _RDO_STUDIO_MFC_FRAME_MANAGER_H_
