/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio/src/view_preferences.h
  \author    ������� ������� (robot.xet@gmail.com)
  \date      27.11.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_VIEW_PREFERENCES_H_
#define _RDO_STUDIO_VIEW_PREFERENCES_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qdialog.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/projects/common/bin/rdo_studio/generated/ui_view_preferenses.h"
#include "app/rdo_studio/src/options.h"
// --------------------------------------------------------------------------------

class ViewPreferences
	: public QDialog
	, private Ui::ViewPreferencesDialog
{
Q_OBJECT
public:
	explicit ViewPreferences(PTR(QWidget) pParent = NULL);

	void onUpdateStyleNotify(const rdoEditor::RDOEditorEditStyle& style);
	void setEditorPreferences(const rdoEditor::RDOEditorEditStyle& style);
	void updateDialog();
	rbool checkNewOptions(const rdoEditor::RDOEditorEditStyle& style);

private slots:
	void okButtonClicked();
	void cancelButtonClicked();
	void applyButtonClicked();
	void helpButtonClicked();

	void codeCompUseChanged(int state);
	void codeCompShowFullListChanged(bool state);
	void marginFoldChanged(int state);
	void marginBookmarkChanged(int state);
	void marginLineNumberChanged(int state);

	void onTreeWidgetItemActivated(QTreeWidgetItem* item, int column);

private:
	rdoEditor::RDOEditorEditStyle style_editor;

	typedef PTR(QTreeWidgetItem) treeItem;

	//��� ����
	treeItem m_pRoot;
	treeItem m_pText;
	treeItem m_pCompile;
	treeItem m_pDebug;
	treeItem m_pTrace;
	treeItem m_pResult;
	treeItem m_pSearch;
	treeItem m_pChart;
	treeItem m_pAnimation;

	//�������� �����
	treeItem m_pPlainText;
	treeItem m_pVariable;
	treeItem m_pKeyword;
	treeItem m_pFunction;
	treeItem m_pTraceText;
	treeItem m_pColor;
	treeItem m_pComment;
	treeItem m_pNumber;
	treeItem m_pString;
	treeItem m_pOperator;
	treeItem m_pCaret;
	treeItem m_pSelection;
	treeItem m_pBookmark;
	treeItem m_pGroup;
	treeItem m_pError;

	//���� ����������
	treeItem m_pTextCompile;
	treeItem m_pSelectedString;
	treeItem m_pCaretCompile;
	treeItem m_pSelectionCompile;
	treeItem m_pBookmarkCompile;

	//���� �������
	treeItem m_pTextDebug;
	treeItem m_pCaretDebug;
	treeItem m_pSelectionDebug;
	treeItem m_pBookmarkDebug;

	//���� �����������
	treeItem m_pES;
	treeItem m_pEB;
	treeItem m_pEF;
	treeItem m_pEI;
	treeItem m_pER;
	treeItem m_pRC;
	treeItem m_pRE;
	treeItem m_pRK;
	treeItem m_pV;
	treeItem m_pStatus;
	treeItem m_pDPS;
	treeItem m_pSB;
	treeItem m_pSO;
	treeItem m_pSTN;
	treeItem m_pSTD;
	treeItem m_pSTR;
	treeItem m_pSRC;
	treeItem m_pSRE;
	treeItem m_pSRK;
	treeItem m_pSD;
	treeItem m_pSES;
	treeItem m_pSEN;
	treeItem m_pSEM;
	treeItem m_pSEF;
	treeItem m_pSEU;

	//���� �����������
	treeItem m_pPlainTextResult;
	treeItem m_pVariableResult;
	treeItem m_pKeywordResult;
	treeItem m_pNumberResult;
	treeItem m_pStringResult;
	treeItem m_pOperatorResult;
	treeItem m_pCaretResult;
	treeItem m_pSelectionResult;
	treeItem m_pBookmarkResult;

	//���� ������
	treeItem m_pTextSearch;
	treeItem m_pStringSearch;
	treeItem m_pSelectedStringSearch;
	treeItem m_pCaretSearch;
	treeItem m_pSelectionSearch;
	treeItem m_pBookmarkSearch;

	//���� ��������
	treeItem m_pAxis;
	treeItem m_pTitle;
	treeItem m_pLegend;
	treeItem m_pGraph;
	treeItem m_pTime;

	//���� ��������
	treeItem m_pEdgingColor;
	treeItem m_pBackgroundColor;	

	enum ItemType
	{
		IT_ROOT = 0,
		IT_TEXT,
		IT_COMPILE,
		IT_DEBUG,
		IT_TRACE,
		IT_RESULT,
		IT_SEARCH,
		IT_CHART,
		IT_ANIMATION
	};

	void createTree();
	PTR(QTreeWidgetItem) createTreeItem (PTR(QTreeWidgetItem) parent, CREF(QString) name, ItemType itemType);
};

#endif // _RDO_STUDIO_VIEW_PREFERENCES_H_