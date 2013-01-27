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

	createTreeItem(m_pRoot, m_pText,      "�������� �����",   IT_TEXT);
	createTreeItem(m_pRoot, m_pCompile,   "���� ����������",  IT_COMPILE);
	createTreeItem(m_pRoot, m_pDebug,     "���� �������",     IT_DEBUG);
	createTreeItem(m_pRoot, m_pTrace,     "���� �����������", IT_TRACE);
	createTreeItem(m_pRoot, m_pResult,    "���� �����������", IT_RESULT);
	createTreeItem(m_pRoot, m_pSearch,    "���� ������",      IT_SEARCH);
	createTreeItem(m_pRoot, m_pChart,     "���� ��������",    IT_CHART);
	createTreeItem(m_pRoot, m_pAnimation, "���� ��������",    IT_ANIMATION);

	m_pRoot->setExpanded(true);

	//createTreeItem(m_pText, m_pPlainText, "������������ �����", IT_TEXT);
	//createTreeItem(m_pText, m_pVariable,  "����������",         IT_TEXT);
	//createTreeItem(m_pText, m_pKeyword,   "�������� �����",     IT_TEXT);
	//createTreeItem(m_pText, m_pTraceText, "�����������",        IT_TEXT);
	//createTreeItem(m_pText, m_pColor,     "����",               IT_TEXT);
	//createTreeItem(m_pText, m_pComment,   "�����������",        IT_TEXT);
	//createTreeItem(m_pText, m_pNumber,    "�����",              IT_TEXT);
	//createTreeItem(m_pText, m_pString,    "������",             IT_TEXT);
	//createTreeItem(m_pText, m_pOperator,  "��������",           IT_TEXT);
	//createTreeItem(m_pText, m_pCaret,     "�������",            IT_TEXT);
	//createTreeItem(m_pText, m_pSelection, "���������",          IT_TEXT);
	//createTreeItem(m_pText, m_pBookmark,  "��������",           IT_TEXT);
	//createTreeItem(m_pText, m_pGroup,     "������",             IT_TEXT);
	//createTreeItem(m_pText, m_pError,     "������",             IT_TEXT);
}

void ViewPreferences::createTreeItem(PTR(QTreeWidgetItem) parent, PTR(QTreeWidgetItem) item, CREF(QString) name, ItemType itemType)
{
	item = new QTreeWidgetItem(parent);
	item->setText(0, name);
	item->setData(0, Qt::UserRole, QVariant(itemType));
	item->setDisabled(false);
}

void ViewPreferences::onTreeWidgetItemActivated(QTreeWidgetItem* item, int column)
{
	stackedWidget->setCurrentIndex(item->data(column, Qt::UserRole).toInt());
}