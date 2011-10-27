#ifndef RDO_PROCESS_OBJECT_PIXMAP_H
#define RDO_PROCESS_OBJECT_PIXMAP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_shape.h"

// --------------------------------------------------------------------------------
// -------------------- RPObjectPixmap
// --------------------------------------------------------------------------------
class RPPixmap;

class RPObjectPixmap: public RPShape
{
protected:
	RPPixmap* pixmap;

	virtual void setPosition( double posx, double posy );
	virtual void draw( CDC& dc );

public:
	RPObjectPixmap( RPObject* parent, RPPixmap* pixmap );
	virtual ~RPObjectPixmap();

	virtual rp::string getClassName() const { return "RPObjectPixmap"; }
	virtual void load( rp::RPXMLNode* node );
	virtual rp::RPXMLNode* save( rp::RPXMLNode* parent_node );
	// ��������������� ����������� ������� save_To_XML ��� RPObjectPixmap:
	void save_To_XML(pugi::xml_document &doc, std::list< RPObject* >& all_child);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDO_PROCESS_OBJECT_PIXMAP_H
