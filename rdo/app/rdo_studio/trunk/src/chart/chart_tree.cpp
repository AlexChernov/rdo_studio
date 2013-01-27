/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracertreectrl.cpp
  \author    ������� �����
  \date      12.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/foreach.hpp>
#include <QtCore/qprocess.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/chart/chart_tree.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracer.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerrestype.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerresource.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerpattern.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotraceroperation.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerresult.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- ChartTree
// --------------------------------------------------------------------------------
SCODE RDODropSource::GiveFeedback(DROPEFFECT dropEffect)
{
	return COleDropSource::GiveFeedback(dropEffect);
}

//! @todo qt
//BEGIN_MESSAGE_MAP( ChartTree, RDOTreeCtrl )
//	ON_WM_INITMENUPOPUP()
//	ON_COMMAND( ID_CHART_ADDTONEWCHART, OnAddToNewChart )
//	ON_UPDATE_COMMAND_UI( ID_CHART_ADDTONEWCHART, OnUpdateAddToNewChart )
//	ON_NOTIFY_REFLECT( TVN_BEGINDRAG, OnDragDrop )
//	ON_WM_RBUTTONDOWN()
//	ON_UPDATE_COMMAND_UI(ID_CHART_FINDINCHARTS, OnUpdateChartFindincharts)
//	ON_COMMAND(ID_CHART_FINDINCHARTS, OnChartFindincharts)
//	ON_COMMAND( ID_CHART_EXPORT, OnExportChart )
//	ON_UPDATE_COMMAND_UI( ID_CHART_EXPORT, OnUpdateExportChart )
//END_MESSAGE_MAP()

Q_DECLARE_METATYPE(ChartTreeItem*);

ChartTree::ChartTree(PTR(QWidget) pParent)
	: parent_type(pParent)
{
	setColumnCount    (1);
	setHeaderHidden   (true);
	setRootIsDecorated(false);

	m_iconList.reserve(IT_COUNT);
	m_iconList.push_back(QIcon(QString::fromUtf8(":/images/images/tree_chart_root.png")));
	m_iconList.push_back(QIcon(QString::fromUtf8(":/images/images/tree_chart_sub_root_1.png")));
	m_iconList.push_back(QIcon(QString::fromUtf8(":/images/images/tree_chart_sub_root_2.png")));
	m_iconList.push_back(QIcon(QString::fromUtf8(":/images/images/tree_chart_sub_root_3.png")));
	m_iconList.push_back(QIcon(QString::fromUtf8(":/images/images/tree_chart_value.png")));
	m_iconList.push_back(QIcon(QString::fromUtf8(":/images/images/tree_chart_erased.png")));

	m_root.setCtrlItem(new QTreeWidgetItem(this));
	m_root.getCtrlItem().setText(0, "������");
	m_root.getCtrlItem().setIcon(0, m_iconList[IT_ROOT]);

	createItem(m_root, m_rootRTP, "���� ��������", IT_SUB_ROOT_1);
	createItem(m_root, m_rootPAT, "�������",       IT_SUB_ROOT_1);
	createItem(m_root, m_rootPMV, "����������",    IT_SUB_ROOT_1);

	m_root.getCtrlItem().setExpanded(true);

	connect(
		this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
		this, SLOT(onTreeWidgetItemDoubleClicked(QTreeWidgetItem*, int))
	);

	//! @todo qt
	//popupMenu.CreatePopupMenu();

	//if (AfxGetMainWnd())
	//{
	//	CMenu* mainMenu = AfxGetMainWnd()->GetMenu();
	//	if (mainMenu)
	//	{
	//		rbool maximized = studioApp.getIMainWnd()->isMDIMaximazed();
	//		int delta = maximized ? 1 : 0;

	//		appendMenu( mainMenu->GetSubMenu( 6 + delta ), 2, &popupMenu );
	//	}
	//}
}

ChartTree::~ChartTree()
{
	m_source.Empty();
}

PTR(ChartTreeItem) ChartTree::getIfItemIsDrawable(CPTR(QTreeWidgetItem) pCtrlItem) const
{
	PTR(ChartTreeItem) pRes = NULL;
	if (pCtrlItem)
	{
		PTR(ChartTreeItem) pItem = pCtrlItem->data(0, Qt::UserRole).value<PTR(ChartTreeItem)>();
		pRes = pItem && pItem->isDrawable()
			? pItem
			: NULL;
	}
	return pRes;
}

//! @todo qt
//void ChartTree::OnInitMenuPopup( CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu )
//{
//	RDOTreeCtrl::OnInitMenuPopup( pPopupMenu, nIndex, bSysMenu );
//	CFrameWnd* pwndFrame = (CFrameWnd*)AfxGetMainWnd();
//	if( pwndFrame ) pwndFrame->SendMessage( WM_INITMENUPOPUP, WPARAM(pPopupMenu->m_hMenu), MAKELPARAM(nIndex, bSysMenu) );
//}
//
//void ChartTree::doDragDrop( ChartTreeItem* item, CPoint point )
//{
//	UNUSED(point);
//
//	UINT format = g_pTracer->getClipboardFormat();
//	if ( format ) {
//		TracerSerie** ptr = (TracerSerie**)::GlobalAlloc( LMEM_FIXED, sizeof( TracerSerie* ) );
//		*ptr = (TracerSerie*)item;
//		source.CacheGlobalData( CLIPFORMAT(format), ptr );
//		source.DoDragDrop( DROPEFFECT_COPY, NULL, &dropsource );
//		source.Empty();
//		// Dont call ::GlobalFree( ptr ), because
//		// COleDataSource::Empty() calls ::ReleaseStgMedium() for
//		// each allocated storage medium. By Microsoft's default
//		// STGMEDIUM::punkForRelease == NULL,
//		// so ::ReleaseStgMedium() calls ::GlobalFree()
//		// for each allocated STGMEDIUM::TYMED_HGLOBAL.
//		// ::GlobalFlags( ptr ) returns GMEM_INVALID_HANDLE
//		// if HGLOBAL is not a valid handle.
//	}
//}
//
//void ChartTree::OnDragDrop ( NMHDR * pNotifyStruct, LRESULT* result )
//{
//	LPNMTREEVIEW lpnmtv = (LPNMTREEVIEW)pNotifyStruct;
//	HTREEITEM hitem = lpnmtv->itemNew.hItem;
//	ChartTreeItem* item = getIfItemIsDrawable( hitem );
//	if ( item  )
//		doDragDrop( item, lpnmtv->ptDrag );
//	*result = 0;
//}

void ChartTree::setModelName(CREF(tstring) modelName)
{
	m_root.getCtrlItem().setText(0, QString::fromStdString(rdo::format(ID_MODEL, modelName.c_str())));
}

void ChartTree::createItem(REF(ChartTreeItem) parent, REF(ChartTreeItem) item, CREF(QString) name, IconType iconType)
{
	PTR(QTreeWidgetItem) pCtrlItem = new QTreeWidgetItem(&parent.getCtrlItem());
	pCtrlItem->setText(0, name);
	pCtrlItem->setIcon(0, m_iconList[iconType]);
	pCtrlItem->setData(0, Qt::UserRole, QVariant::fromValue(&item));
	item.setCtrlItem(pCtrlItem);
}

void ChartTree::addResourceType(PTR(TracerResType) pRTP)
{
	createItem(m_rootRTP, *pRTP, QString::fromStdString(pRTP->Name), IT_SUB_ROOT_2);
}

void ChartTree::addResource(PTR(TracerResource) pRSS)
{
	PTR(TracerResType) pRTP = pRSS->getType();
	createItem(*pRTP, *pRSS, QString::fromStdString(pRSS->Name), IT_SUB_ROOT_3);

	int count = pRTP->getParamsCount();
	for (int i = 0; i < count; i++)
	{
		PTR(ChartTreeItem) pParam = pRSS->getParam(i);
		ASSERT(pParam);
		createItem(*pRSS, *pParam, QString::fromStdString(pRTP->getParamInfo(i)->Name), IT_VALUE);
	}
	updateResource(pRSS);
}

void ChartTree::updateResource(PTR(TracerResource) pRSS)
{
	if (pRSS->isErased())
	{
		pRSS->getCtrlItem().setIcon(0, m_iconList[IT_ERASED]);
	}
	else
	{
		pRSS->getCtrlItem().setIcon(0, m_iconList[IT_SUB_ROOT_3]);
	}
}

void ChartTree::addPattern(PTR(TracerPattern) pPAT)
{
	createItem(m_rootPAT, *pPAT, QString::fromStdString(pPAT->Name), IT_SUB_ROOT_2);
}

void ChartTree::addOperation(PTR(TracerOperationBase) pOPR)
{
	createItem(*pOPR->getPattern(), *pOPR, QString::fromStdString(pOPR->getName()), IT_VALUE);
}

/*void ChartTree::addIrregularEvent(PTR(TracerOperation) pOpr)
{
	addOperation(pOpr);
}*/

void ChartTree::addResult(PTR(TracerResult) pPMV)
{
	createItem(m_rootPMV, *pPMV, QString::fromStdString(pPMV->getName()), IT_VALUE);
}

void ChartTree::deleteChildren(REF(ChartTreeItem) parent)
{
	QList<PTR(QTreeWidgetItem)> children = parent.getCtrlItem().takeChildren();
	BOOST_FOREACH(PTR(QTreeWidgetItem) item, children)
	{
		parent.getCtrlItem().removeChild(item);
	}
}

void ChartTree::clear()
{
	deleteChildren(m_rootRTP);
	deleteChildren(m_rootPAT);
	deleteChildren(m_rootPMV);
	m_root.getCtrlItem().setText(0, "������");
}

void ChartTree::addToNewChart(PTR(QTreeWidgetItem) pCtrlItem) const
{
	PTR(ChartTreeItem) pTreeItem = getIfItemIsDrawable(pCtrlItem);
	if (pTreeItem)
	{
		g_pTracer->addSerieToChart(static_cast<PTR(TracerSerie)>(pTreeItem));
	}
}

rbool ChartTree::findInCharts(PTR(QTreeWidgetItem) pCtrlItem) const
{
	PTR(ChartTreeItem) pTreeItem = getIfItemIsDrawable(pCtrlItem);
	if (pTreeItem)
	{
		PTR(TracerSerie) pSerie = static_cast<PTR(TracerSerie)>(pTreeItem);
		return pSerie->activateFirstDoc();
	}
	return false;
}

PTR(QTreeWidgetItem) ChartTree::getSelected() const
{
	QList<PTR(QTreeWidgetItem)> selected = selectedItems();
	return selected.size() == 1
		? selected.front()
		: NULL;
}

void ChartTree::OnAddToNewChart()
{
	addToNewChart(getSelected());
}

void ChartTree::OnUpdateAddToNewChart( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(g_pTracer->getDrawTrace() && getIfItemIsDrawable(getSelected()));
}

void ChartTree::onTreeWidgetItemDoubleClicked(QTreeWidgetItem* pCtrlItem, int)
{
	if (!g_pTracer->getDrawTrace())
		return;

	if (!findInCharts(pCtrlItem))
	{
		addToNewChart(pCtrlItem);
	}
}

//! @todo qt
//void ChartTree::OnRButtonDown(UINT _nFlags, CPoint point)
//{
//	UNUSED(_nFlags);
//
//	UINT uFlags;
//	HTREEITEM hitem = HitTest( point, &uFlags );
//	if ( hitem && ( TVHT_ONITEM & uFlags ) ) {
//		SelectItem( hitem );
//	}
//	if ( GetFocus() != this )
//		SetFocus();
//	CPoint pos = point;
//	ClientToScreen( &pos );
//	if ( popupMenu.m_hMenu ) popupMenu.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this );
//}
//
//void TracerTreeCtrl::OnExportChart()
//{
//	if (!g_pTracer->getDrawTrace())
//		return;
//
//	PTR(TracerTreeItem) pItem = getIfItemIsDrawable(GetSelectedItem());
//	if (!pItem)
//		return;
//
//	PTR(TracerSerie) pSerie = static_cast<PTR(TracerSerie)>(pItem);
//	ASSERT(pSerie);
//	TracerSerie::ExportData exportData = pSerie->exportData();
//	if (exportData.empty())
//		return;
//
//	CString filter("csv-���� (*.csv)|*.csv|��� ����� (*.*)|*.*||");
//	CFileDialog dlg(false, _T("csv"), pSerie->getTitle().c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, AfxGetMainWnd());
//	if (dlg.DoModal() != IDOK)
//		return;
//
//	CString fileName = dlg.GetPathName();
//	std::ofstream stream(fileName);
//	if (!stream.is_open())
//		return;
//
//	BOOST_FOREACH(CREF(TracerSerie::ExportData::value_type) exportItem, exportData)
//	{
//		stream << exportItem << std::endl;
//	}
//
//	stream.close();
//}
//
//void TracerTreeCtrl::OnUpdateExportChart( CCmdUI* pCmdUI )
//{
//	pCmdUI->Enable( g_pTracer->getDrawTrace() && getIfItemIsDrawable( GetSelectedItem() ) != NULL );
//}

void ChartTree::OnUpdateChartFindincharts(CCmdUI* pCmdUI)
{
	rbool enable = false;
	if (g_pTracer->getDrawTrace())
	{
		PTR(ChartTreeItem) pTreeItem = getIfItemIsDrawable(getSelected());
		if (pTreeItem)
		{
			PTR(TracerSerie) pSerie = static_cast<PTR(TracerSerie)>(pTreeItem);
			enable = pSerie->isInOneOrMoreDocs();
		}
	}
	pCmdUI->Enable(enable);
}

void ChartTree::OnChartFindincharts()
{
	findInCharts(getSelected());
}

void ChartTree::focusInEvent(QFocusEvent* pEvent)
{
	parent_type::focusInEvent(pEvent);
	activate(pEvent);
}

void ChartTree::focusOutEvent(QFocusEvent* pEvent)
{
	parent_type::focusOutEvent(pEvent);
	deactivate(pEvent);
}

void ChartTree::onUpdateActions(rbool activated)
{
	RDOStudioMainFrame* pMainWindow = studioApp.getMainWndUI();
	ASSERT(pMainWindow);

	updateAction(
		pMainWindow->actHelpContext,
		activated,
		this, "onHelpContext()"
	);
}

void ChartTree::onHelpContext()
{
	QByteArray ba;
	ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_model/work_model_chart.htm\n");
	studioApp.callQtAssistant(ba);
}
