// rdoprocess_shape_process_MJ.h: interface for the RPShapeProcessMJ class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDOPROCESS_SHAPE_PROCESS_MJ_H__A5AEB64C_2CE8_46D3_9E6B_86368CB68269__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_PROCESS_MJ_H__A5AEB64C_2CE8_46D3_9E6B_86368CB68269__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_datablock.h"

class RPShapeProcessMJ : public RPShape_MJ
{
friend class RPMethodProc2RDO_MJ;

private:
	static RPObject* newObject( RPObject* parent );

public:
	RPShapeProcessMJ( RPObject* parent );
	virtual ~RPShapeProcessMJ();
	virtual rp::string getClassName() const { return "RPShapeProcessMJ"; }

	virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos );
	virtual void generate();

	//��������� ��� ����������
	LPRPShapeDataBlockProcess m_pParams;
	//���������� ��� ���������
	tstring gname; // ���
	
	int gtype; // ����� ��������
	int base_gen;
	
	//�������� �������
	double gexp;
	double gdisp;
	double gmax;
	double gmin;

	int action; // ��� �������� �� ��������� � �������
	int prior;
	int queue;
	int parameter;

	std::list< CString> list_resource_procMJ;

	// ��������������� ����������� ������� saveToXML ��� RPShapeProcessMJ:
	void saveToXML(REF(pugi::xml_node) parentNode);
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_PROCESS_MJ_H__A5AEB64C_2CE8_46D3_9E6B_86368CB68269__INCLUDED_)
