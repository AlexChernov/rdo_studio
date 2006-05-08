#include "rdocommon.h"

#pragma warning(disable : 4786)  

#include <afxwin.h>
#include <stdio.h>
#include <stdarg.h>

namespace rdo {

std::string format( const char* str, ... )
{
	va_list params;
	va_start( params, str );
	std::string res = format( str, params );
	va_end( params );
	return res;
}

std::string format( const char* str, va_list& params )
{
	std::vector< char > s;
	s.resize( 256 );
	int size = -1;
	while ( size == -1 ) {
		size = _vsnprintf( s.begin(), s.size(), str, params );
		if ( size == -1 ) {
			s.resize( s.size() + 256 );
		}
	}
	s.resize( size );
	return std::string( s.begin(), s.end() );
}

std::string format( unsigned int resource, ... )
{
	va_list params;
	va_start( params, resource );
	std::string res = format( resource, params );
	va_end( params );
	return res;
}

std::string format( unsigned int resource, va_list& params )
{
	CString str;
	if ( str.LoadString( resource ) ) {
		return format( static_cast<LPCTSTR>(str), params );
	}
	return "";
}

void trim( std::string& str )
{
	static char szDelims[] = " \t\n\r";
	str.erase( 0, str.find_first_not_of( szDelims ) );
	str.erase( str.find_last_not_of( szDelims ) + 1, std::string::npos );
}

void trimLeft( std::string& str )
{
	static char szDelims[] = " \t\n\r";
	str.erase( 0, str.find_first_not_of( szDelims ) );
}

void trimRight( std::string& str )
{
	static char szDelims[] = " \t\n\r";
	str.erase( str.find_last_not_of( szDelims ) + 1, std::string::npos );
}

std::string extractFilePath( const std::string& fileName )
{
	std::string s;
	std::string::size_type pos = fileName.find_last_of( '\\' );
	if ( pos == std::string::npos ) {
		pos = fileName.find_last_of( '/' );
	}
	if ( pos == std::string::npos ) {
		return "";
	}
	if ( pos != std::string::npos && pos < fileName.length() - 1 ) {
		s.assign( fileName.begin(), pos + 1 );
		static char szDelims[] = " \t\n\r";
		s.erase( 0, s.find_first_not_of( szDelims ) );
		s.erase( s.find_last_not_of( szDelims ) + 1, std::string::npos );
	} else {
		s = fileName;
	}
	pos = s.find_last_of( '\\' );
	if ( pos == std::string::npos ) {
		pos = s.find_last_of( '/' );
	}
	if ( pos != s.length() - 1 && s.length() ) {
		s += "/";
	}
	return s;
}

bool isFileExists( const std::string& fileName )
{
	CFileFind finder;
	return finder.FindFile( fileName.c_str() ) ? true : false;
}

} // namespace rdo

namespace rdosim
{

RDOFrame::~RDOFrame()
{
	for(std::vector<RDOFrameElement *>::iterator it = elements.begin(); it != elements.end(); it++)
		delete (*it);

	elements.clear();
}

RDOTextElement::RDOTextElement(double _x, double _y, double _w, double _h, RDOColor &_background, RDOColor &_foreground, std::string &_text, RDOTextAlign _align)
	: RDOBoundedElement(_x, _y, _w, _h), RDOColoredElement(_background, _foreground), strText(_text), align(_align)
{ 
	type = text_type; 
}

RDOColoredElement::RDOColoredElement(RDOColor &_background, RDOColor &_foreground)
	: background(_background), foreground(_foreground)
{}

RDOBoundedElement::RDOBoundedElement(double _x, double _y, double _w, double _h)
	: x(_x), y(_y), w(_w), h(_h)
{}

RDORectElement::RDORectElement(double _x, double _y, double _w, double _h, RDOColor &_background, RDOColor &_foreground)
	: RDOBoundedElement(_x, _y, _w, _h), RDOColoredElement(_background, _foreground)
{ 
	type = rect_type; 
}

RDOLineElement::RDOLineElement(double _x, double _y, double _w, double _h, RDOColor &_foreground)
	: RDOBoundedElement(_x, _y, _w, _h), foreground(_foreground)
{ 
	type = line_type; 
}

RDOSBmpElement::RDOSBmpElement(double _x, double _y, double _w, double _h, std::string &_bmp, std::string &_mask)
	: RDOBoundedElement(_x, _y, _w, _h), bmp(_bmp), mask(_mask)
{ 
	type = s_bmp_type; 
	hasMask = true;
}

RDOSBmpElement::RDOSBmpElement(double _x, double _y, double _w, double _h, std::string &_bmp)
	: RDOBoundedElement(_x, _y, _w, _h), bmp(_bmp)
{ 
	type = s_bmp_type; 
	hasMask = false;
}

RDOBitmapElement::RDOBitmapElement(double _x, double _y, std::string &_bmp, std::string &_mask)
	: x(_x), y(_y), bmp(_bmp), mask(_mask)
{
	type = bitmap_type; 
	hasMask = true;
}

RDOBitmapElement::RDOBitmapElement(double _x, double _y, std::string &_bmp)
	: x(_x), y(_y), bmp(_bmp)
{
	type = bitmap_type; 
	hasMask = false;
}

RDOActiveElement::RDOActiveElement(double _x, double _y, double _w, double _h, std::string &_operName)
	: RDOBoundedElement(_x, _y, _w, _h), operName(_operName)
{ 
	type = active_type; 
}

RDORRectElement::RDORRectElement(double _x, double _y, double _w, double _h, RDOColor &_background, RDOColor &_foreground)
	: RDOBoundedElement(_x, _y, _w, _h), RDOColoredElement(_background, _foreground)
{ 
	type = r_rect_type; 
}

RDOEllipseElement::RDOEllipseElement(double _x, double _y, double _w, double _h, RDOColor &_background, RDOColor &_foreground)
	: RDOBoundedElement(_x, _y, _w, _h), RDOColoredElement(_background, _foreground)
{ 
	type = ellipse_type; 
}

RDOTriangElement::RDOTriangElement(double _x1, double _y1, double _x2, double _y2, double _x3, double _y3, RDOColor &_background, RDOColor &_foreground)
	: x1(_x1), y1(_y1), x2(_x2), y2(_y2), x3(_x3), y3(_y3), RDOColoredElement(_background, _foreground)
{ 
	type = triang_type; 
}

} // namespace rdosim

