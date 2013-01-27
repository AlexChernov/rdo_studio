/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio/src/view_preferences.cpp
  \author    ������� ������� (robot.xet@gmail.com)
  \date      27.11.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/view_preferences.h"
#include "app/rdo_studio/res/build_version.h"
#include "app/rdo_studio/src/application.h"
// --------------------------------------------------------------------------------

ViewPreferences::ViewPreferences(PTR(QWidget) pParent)
	: QDialog(pParent),
	  style_editor()
{
	setupUi(this);

	connect(buttonOk, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));
	connect(buttonApply, SIGNAL(clicked()), this, SLOT(applyButtonClicked()));
	connect(buttonHelp, SIGNAL(clicked()), this, SLOT(helpButtonClicked()));

	//������� "��������"
	connect(checkBoxCodeCompUse, SIGNAL(stateChanged(int)), this, SLOT(codeCompUseChanged(int)));
	connect(radioButtonFullList, SIGNAL(toggled(bool)), this, SLOT(codeCompShowFullListChanged(bool)));
	connect(radioButtonNearestWords, SIGNAL(toggled(bool)), this, SLOT(codeCompShowFullListChanged(bool)));
	connect(checkBoxMarginFold, SIGNAL(stateChanged(int)), this, SLOT(marginFoldChanged(int)));
	connect(checkBoxMarginBookmark, SIGNAL(stateChanged(int)), this, SLOT(marginBookmarkChanged(int)));
	connect(checkBoxMarginLineNum, SIGNAL(stateChanged(int)), this, SLOT(marginLineNumberChanged(int)));

	//������� "����� � ����"
	stackedWidget->setCurrentWidget(pageRoot);
	createTree();

	connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onTreeWidgetItemActivated(QTreeWidgetItem*, int)));
}

void ViewPreferences::onUpdateStyleNotify(const rdoEditor::RDOEditorEditStyle& style)
{
	if(style_editor != style)
		setEditorPreferences(style);
	updateDialog();
}

void ViewPreferences::okButtonClicked()
{

}

void ViewPreferences::cancelButtonClicked()
{

}

void ViewPreferences::applyButtonClicked()
{
	//applyList
}

void ViewPreferences::helpButtonClicked()
{

}

void ViewPreferences::codeCompUseChanged(int state)
{
	style_editor.autoComplete->useAutoComplete = state;
	
	switch(state)
	{
		case 0:
			radioButtonFullList->setEnabled(false);
			radioButtonNearestWords->setEnabled(false);
			break;
		case 2:
			radioButtonFullList->setEnabled(true);
			radioButtonNearestWords->setEnabled(true);
			break;
	}

}

void ViewPreferences::codeCompShowFullListChanged(bool state)
{
	UNUSED(state);

	if(radioButtonFullList->isChecked())
		style_editor.autoComplete->showFullList = true;
	if(radioButtonNearestWords->isChecked())
		style_editor.autoComplete->showFullList = false;
}

void ViewPreferences::marginFoldChanged(int state)
{
	style_editor.margin->fold = state;
}

void ViewPreferences::marginBookmarkChanged(int state)
{
	style_editor.margin->bookmark = state;
}

void ViewPreferences::marginLineNumberChanged(int state)
{
	style_editor.margin->lineNumber = state;
}

void ViewPreferences::setEditorPreferences(const rdoEditor::RDOEditorEditStyle& style)
{
	style_editor = style;
}

void ViewPreferences::updateDialog()
{
	checkBoxCodeCompUse->setCheckState(style_editor.autoComplete->useAutoComplete
		? Qt::Checked
		: Qt::Unchecked
		);
	checkBoxMarginFold->setCheckState(style_editor.margin->fold
		? Qt::Checked
		: Qt::Unchecked
		);
	checkBoxMarginBookmark->setCheckState(style_editor.margin->bookmark
		? Qt::Checked
		: Qt::Unchecked
		);
	checkBoxMarginLineNum->setCheckState(style_editor.margin->lineNumber
		? Qt::Checked
		: Qt::Unchecked
		);

	style_editor.autoComplete->showFullList
		? radioButtonFullList->toggle()
		: radioButtonNearestWords->toggle();
}

void ViewPreferences::createTree()
{
	treeWidget->setColumnCount(1);
	treeWidget->setHeaderHidden(true);
	treeWidget->setRootIsDecorated(false);

	m_pRoot = new QTreeWidgetItem(treeWidget);
	m_pRoot->setText(0, "��� ����");
	m_pRoot->setData(0, Qt::UserRole, IT_ROOT);

	m_pText      = createTreeItem(m_pRoot, "�������� �����",   IT_TEXT);
	m_pCompile   = createTreeItem(m_pRoot, "���� ����������",  IT_COMPILE);
	m_pDebug     = createTreeItem(m_pRoot, "���� �������",     IT_DEBUG);
	m_pTrace     = createTreeItem(m_pRoot, "���� �����������", IT_TRACE);
	m_pResult    = createTreeItem(m_pRoot, "���� �����������", IT_RESULT);
	m_pSearch    = createTreeItem(m_pRoot, "���� ������",      IT_SEARCH);
	m_pChart     = createTreeItem(m_pRoot, "���� ��������",    IT_CHART);
	m_pAnimation = createTreeItem(m_pRoot, "���� ��������",    IT_ANIMATION);

	m_pRoot->setExpanded(true);

	m_pPlainText = createTreeItem(m_pText, "������������ �����", IT_TEXT);
	m_pVariable  = createTreeItem(m_pText, "����������",         IT_TEXT);
	m_pKeyword   = createTreeItem(m_pText, "�������� �����",     IT_TEXT);
	m_pTraceText = createTreeItem(m_pText, "�����������",        IT_TEXT);
	m_pColor     = createTreeItem(m_pText, "����",               IT_TEXT);
	m_pComment   = createTreeItem(m_pText, "�����������",        IT_TEXT);
	m_pNumber    = createTreeItem(m_pText, "�����",              IT_TEXT);
	m_pString    = createTreeItem(m_pText, "������",             IT_TEXT);
	m_pOperator  = createTreeItem(m_pText, "��������",           IT_TEXT);
	m_pCaret     = createTreeItem(m_pText, "�������",            IT_TEXT);
	m_pSelection = createTreeItem(m_pText, "���������",          IT_TEXT);
	m_pBookmark  = createTreeItem(m_pText, "��������",           IT_TEXT);
	m_pGroup     = createTreeItem(m_pText, "������",             IT_TEXT);
	m_pError     = createTreeItem(m_pText, "������",             IT_TEXT);

	m_pTextCompile      = createTreeItem(m_pCompile, "�����",             IT_COMPILE);
	m_pSelectedString   = createTreeItem(m_pCompile, "���������� ������", IT_COMPILE);
	m_pCaretCompile     = createTreeItem(m_pCompile, "�������",           IT_COMPILE);
	m_pSelectionCompile = createTreeItem(m_pCompile, "���������",         IT_COMPILE);
	m_pBookmarkCompile  = createTreeItem(m_pCompile, "��������",          IT_COMPILE);

	m_pTextDebug      = createTreeItem(m_pDebug, "�����",     IT_DEBUG);
	m_pCaretDebug     = createTreeItem(m_pDebug, "�������",   IT_DEBUG);
	m_pSelectionDebug = createTreeItem(m_pDebug, "���������", IT_DEBUG);
	m_pBookmarkDebug  = createTreeItem(m_pDebug, "��������",  IT_DEBUG);

	m_pES     = createTreeItem(m_pTrace, "��������� ������� (ES)",                         IT_TRACE);
	m_pEB     = createTreeItem(m_pTrace, "������ �������� (EB)",                           IT_TRACE);
	m_pEF     = createTreeItem(m_pTrace, "��������� �������� (EF)",                        IT_TRACE);
	m_pEI     = createTreeItem(m_pTrace, "������������ ������� (EI)",                      IT_TRACE);
	m_pER     = createTreeItem(m_pTrace, "������������� ������� (ER)",                     IT_TRACE);
	m_pRC     = createTreeItem(m_pTrace, "�������� ������� (RC)",                          IT_TRACE);
	m_pRE     = createTreeItem(m_pTrace, "�������� ������� (RE)",                          IT_TRACE);
	m_pRK     = createTreeItem(m_pTrace, "��������� ��������� ������� (RK)",               IT_TRACE);
	m_pV      = createTreeItem(m_pTrace, "����������� ������� (V)",                        IT_TRACE);
	m_pStatus = createTreeItem(m_pTrace, "������ ��������� ������������� ($Status)",       IT_TRACE);
	m_pDPS    = createTreeItem(m_pTrace, "���������� �� ������ �� ����� (DPS)",            IT_TRACE);
	m_pSB     = createTreeItem(m_pTrace, "������ ������ (SB)",                             IT_TRACE);
	m_pSO     = createTreeItem(m_pTrace, "����������� ������������ ������� (SO)",          IT_TRACE);
	m_pSTN    = createTreeItem(m_pTrace, "������� ������� (STN)",                          IT_TRACE);
	m_pSTD    = createTreeItem(m_pTrace, "������� ������� (STD)",                          IT_TRACE);
	m_pSTR    = createTreeItem(m_pTrace, "������� ������� (STR)",                          IT_TRACE);
	m_pSRC    = createTreeItem(m_pTrace, "�������� ������� (��� ������) (SRC)",            IT_TRACE);
	m_pSRE    = createTreeItem(m_pTrace, "�������� ������� (��� ������) (SRE)",            IT_TRACE);
	m_pSRK    = createTreeItem(m_pTrace, "��������� ��������� ������� (��� ������) (SRK)", IT_TRACE);
	m_pSD     = createTreeItem(m_pTrace, "����������� ������� (SD)",                       IT_TRACE);
	m_pSES    = createTreeItem(m_pTrace, "���������� ������ (SES)",                        IT_TRACE);
	m_pSEN    = createTreeItem(m_pTrace, "���������� ������ (SEN)",                        IT_TRACE);
	m_pSEM    = createTreeItem(m_pTrace, "���������� ������ (SEM)",                        IT_TRACE);
	m_pSEF    = createTreeItem(m_pTrace, "���������� ������ (SEF)",                        IT_TRACE);
	m_pSEU    = createTreeItem(m_pTrace, "���������� ������ (SEU)",                        IT_TRACE);

	m_pPlainTextResult = createTreeItem(m_pResult, "�������� �����", IT_RESULT);
	m_pVariableResult  = createTreeItem(m_pResult, "����������",     IT_RESULT);
	m_pKeywordResult   = createTreeItem(m_pResult, "�������� �����", IT_RESULT);
	m_pNumberResult    = createTreeItem(m_pResult, "�����",          IT_RESULT);
	m_pStringResult    = createTreeItem(m_pResult, "������",         IT_RESULT);
	m_pOperatorResult  = createTreeItem(m_pResult, "��������",       IT_RESULT);
	m_pCaretResult     = createTreeItem(m_pResult, "�������",        IT_RESULT);
	m_pSelectionResult = createTreeItem(m_pResult, "���������",      IT_RESULT);
	m_pBookmarkResult  = createTreeItem(m_pResult, "��������",       IT_RESULT);

	m_pTextSearch           = createTreeItem(m_pSearch, "�����",             IT_SEARCH);
	m_pStringSearch         = createTreeItem(m_pSearch, "������ ��� ������", IT_SEARCH);
	m_pSelectedStringSearch = createTreeItem(m_pSearch, "���������� ������", IT_SEARCH);
	m_pCaretSearch          = createTreeItem(m_pSearch, "�������",           IT_SEARCH);
	m_pSelectionSearch      = createTreeItem(m_pSearch, "���������",         IT_SEARCH);
	m_pBookmarkSearch       = createTreeItem(m_pSearch, "��������",          IT_SEARCH);

	m_pAxis   = createTreeItem(m_pChart, "���",       IT_CHART);
	m_pTitle  = createTreeItem(m_pChart, "���������", IT_CHART);
	m_pLegend = createTreeItem(m_pChart, "�������",   IT_CHART);
	m_pGraph  = createTreeItem(m_pChart, "������",    IT_CHART);
	m_pTime   = createTreeItem(m_pChart, "�����",     IT_CHART);

	m_pEdgingColor     = createTreeItem(m_pAnimation, "���� ���������",               IT_ANIMATION);
	m_pBackgroundColor = createTreeItem(m_pAnimation, "���� ���� �� ��������� �����", IT_ANIMATION);
}

PTR(QTreeWidgetItem) ViewPreferences::createTreeItem(PTR(QTreeWidgetItem) parent, CREF(QString) name, ItemType itemType)
{
	PTR(QTreeWidgetItem) item = new QTreeWidgetItem(parent);
	item->setText(0, name);
	item->setData(0, Qt::UserRole, QVariant(itemType));
	return item;
}

void ViewPreferences::onTreeWidgetItemActivated(QTreeWidgetItem* item, int column)
{
	stackedWidget->setCurrentIndex(item->data(column, Qt::UserRole).toInt());
}