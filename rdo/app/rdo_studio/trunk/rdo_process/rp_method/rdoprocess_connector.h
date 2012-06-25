#ifndef RDO_PROCESS_CONNECTOR_H
#define RDO_PROCESS_CONNECTOR_H

#include <list>
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object_matrix.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_shape.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_factory.h"

// --------------------------------------------------------------------------------
// -------------------- RPConnector
// --------------------------------------------------------------------------------
#define CON_DEBUG 1
#undef CON_DEBUG

class RPConnector: public RPObjectChart
{
friend class RPConnectorDock;
friend class RPObjectFlowChart;

private:
	virtual rp::string get_xml_node_name() const { return "connector"; }

	RPConnectorDock* dock_begin;
	RPConnectorDock* dock_end;

	rp::polyline borders;
	int recursive;

	void next_step( CDC& dc, const rp::point& p1, const rp::point& p2, double norm1, double norm2, rp::polyline& pa, rp::polyline& pa_post );

#ifdef CON_DEBUG
	void makeConnector( CDC& dc, const rp::point& p1, const rp::point& p2, double norm1, double norm2, rp::polyline& pa );
	rbool getShortLine( CDC& dc, const rp::polyline& pa, const rp::point& from, const rp::point& to, double& lengthA2, rp::point& B1, rp::point& B2, rp::point& interborder );
	rbool isVectoreIntersect( CDC& dc, RPShape* shape, const rp::point& vect_begin, const rp::point& vect_end ) const;
#else if
	void makeConnector( const rp::point& p1, const rp::point& p2, double norm1, double norm2, rp::polyline& pa );
	rbool getShortLine( const rp::polyline& pa, const rp::point& from, const rp::point& to, double& lengthA2, rp::point& B1, rp::point& B2, rp::point& interborder );
	rbool isVectoreIntersect( RPShape* shape, const rp::point& vect_begin, const rp::point& vect_end ) const;
#endif

	// ������� ���� ��������� ������ � ���������� ����������
	virtual void transformToGlobal() {};

	// ��������� �� ����� ������ ������
	virtual rbool pointInPolygon( const rp::point& global_chart_pos )
	{
		UNUSED(global_chart_pos);
		return false;
	};
	// ��������� �� ����� � ��������� (������������) ����� ������ (������������� ���������, � �������)
	virtual rbool pointInNCArea( const rp::point& global_chart_pos )
	{
		UNUSED(global_chart_pos);
		return false;
	};

	// �������� ������
	virtual rp::rect getBoundingRect( rbool global = true ) const
	{
		UNUSED(global);
		return rp::rect();
	};

	// ��������� ������
	virtual void draw(REF(CDC) dc);

	virtual rbool isConnector() const   { return true; }

private:
	static RPObject* newObject( RPObject* parent );

public:
	RPConnector( RPObject* parent, const rp::string& name = "connector" );
	virtual ~RPConnector();

	static void registerObject();

	virtual rp::string getClassName() const { return "RPConnector"; }
	virtual void load( rp::RPXMLNode* node );
	virtual rp::RPXMLNode* save( rp::RPXMLNode* parent_node );
	// ���������������� ����������� ������� saveToXML � loadFromXML ��� RPConnector:
	void saveToXML  (REF (pugi::xml_node) parentNode) const;
	void loadFromXML(CREF(pugi::xml_node) node);
	RPConnectorDock* getConnectedDock( const RPConnectorDock& dock ) const;
};

// --------------------------------------------------------------------------------
// -------------------- RPConnectorDock
// --------------------------------------------------------------------------------
class RPConnectorDock
{
public:
	enum Type {
		in    = 0x01,
		out   = 0x02,
		inout = 0x03,
		fly   = 0x04,
		all   = 0x07
	};
	static const int delta;

protected:
	RPShape*   obj;
	rp::point  point;
	Type       type;
	double     norm;
	std::vector< rp::string > types;

public:
	RPConnectorDock( RPShape* _obj, Type _type, const rp::point& _point, double _norm, const rp::string& type = "" ): obj( _obj ), type( _type ), point( _point ), norm( _norm ) {
		if ( !type.empty() ) insertType( type );
	};
	virtual ~RPConnectorDock();

	virtual rp::RPXMLNode* save( rp::RPXMLNode* parent_node );

	std::list< RPConnector* > connectors;

	int getIndex() const                                            { return obj->getDockIndex( this ); }
	virtual rbool can_connect( RPConnectorDock* dock = NULL ) const {
		if ( !dock ) return true;
		if ( alreadyConnected( dock ) ) return false;
		if ( &dock->object() == &object() ) return false;
		return hasType( dock->getTypes() );
	}
	virtual COLORREF color() const                                  { return RGB(0xF0, 0xFF, 0x00);     }
	Type getType() const                                            { return type;                      }
	rbool isType( Type _type ) const {
		switch ( _type ) {
			case fly  : return type & fly ? true : false;
			case in   : return type & in  ? true : false;
			case out  : return type & out ? true : false;
			case inout: return (type & inout) == inout ? true : false;
			case all  : return (type & all  ) == all   ? true : false;
		}
		return false;
	}
	rbool isIn() const  { return type & in  ? true : false; }
	rbool isOut() const { return type & out ? true : false; }
	const RPShape& object() const {
		return *obj;
	}
	rp::point getPosition( rbool global = true ) const {
		return global && obj->isMatrix() ? obj->globalMatrix() * point : point;
	}
	double getNorm( rbool global = true ) const {
		double res = global && obj->isMatrix() ? obj->getRotationGlobal() + norm : norm;
		while ( res >= 360 ) res -= 360;
		while ( res <    0 ) res += 360;
		return res;
	}
	rp::point getDeltaPosition( rbool global = true ) const {
		rp::point pos    = getPosition( global );
		double    rotate = getNorm( global );
		double    cos_a, sin_a;
		rp::math::getCosSin( rotate, cos_a, sin_a );
		return rp::point( pos.x + cos_a * RPConnectorDock::delta, pos.y - sin_a * RPConnectorDock::delta );
	}
	rp::point getOutpoint() const {
		rp::point pos = getPosition();
		rp::point res;
		rp::rect rect = obj->getBoundingRectNoRotateOuter().extendByPerimetr( RPConnectorDock::delta );
		double len = 1e20;
		double l;
		rbool null;
		rp::point p = rp::math::getPerpendicular( rect.p0(), rect.p1(), pos, null );
		if ( !null ) {
			l = rp::math::getLength( p, pos );
			if ( l < len ) {
				len = l;
				res = p;
			}
		}
		p = rp::math::getPerpendicular( rect.p1(), rect.p2(), pos, null );
		if ( !null ) {
			l = rp::math::getLength( p, pos );
			if ( l < len ) {
				len = l;
				res = p;
			}
		}
		p = rp::math::getPerpendicular( rect.p2(), rect.p3(), pos, null );
		if ( !null ) {
			l = rp::math::getLength( p, pos );
			if ( l < len ) {
				len = l;
				res = p;
			}
		}
		p = rp::math::getPerpendicular( rect.p3(), rect.p0(), pos, null );
		if ( !null ) {
			l = rp::math::getLength( p, pos );
			if ( l < len ) {
				len = l;
				res = p;
			}
		}
		return res;
	}
	virtual RPConnector* make_connector( RPObject* _parent ) {
		return static_cast<RPConnector*>(rpMethod::factory->getNewObject( "RPConnector", _parent ));
	}
	rbool alreadyConnected( RPConnectorDock* dock ) const {
		if ( !dock ) return false;
		std::list< RPConnector* >::const_iterator it = connectors.begin();
		while ( it != connectors.end() ) {
			if ( (*it)->dock_begin == dock || (*it)->dock_end == dock ) return true;
			it++;
		}
		return false;
	}

	const std::vector< rp::string >& getTypes() const { return types;            }
	void insertType( const rp::string& type )         { types.push_back( type ); }
	rbool hasType( const rp::string& type ) const     {
		if ( types.empty() ) return true;
		return std::find( types.begin(), types.end(), type ) != types.end();
	}
	rbool hasType( const std::vector< rp::string >& type ) const {
		if ( types.empty() && type.empty() ) return true;
		if ( (!types.empty() && type.empty()) || (types.empty() && !type.empty()) ) return false;
		std::vector< rp::string >::const_iterator it = type.begin();
		while ( it != type.end() ) {
			if ( std::find( types.begin(), types.end(), *it ) == types.end() ) return false;
			it++;
		}
		return true;
	}
};

// --------------------------------------------------------------------------------
// -------------------- RPConnectorDockOne
// --------------------------------------------------------------------------------
class RPConnectorDockOne: public RPConnectorDock
{
public:
	RPConnectorDockOne( RPShape* _parent, Type _type, const rp::point& _point, double _norm, const rp::string& type = "" ): RPConnectorDock( _parent, _type, _point, _norm, type ) {};
	virtual ~RPConnectorDockOne() {};

	virtual rbool can_connect( RPConnectorDock* dock = NULL ) const {
		if ( !RPConnectorDock::can_connect( dock ) ) return false;
		return connectors.empty();
	}
};

#endif // RDO_PROCESS_CONNECTOR_H
