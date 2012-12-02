/*!
  \copyright (c) RDO-Team, 2012
  \file      app/rdo_console/rdo_key_event_xml_reader.h
  \author    ��������� ������� (lord.tiran@gmail.com)
  \date      26.11.2012
  \brief     ���������� ������ RDO
  \indent    4T
*/

#ifndef _RDO_KEY_EVENT_XML_READER_H_
#define _RDO_KEY_EVENT_XML_READER_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/rdo_event_xml_reader.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

class key_event_xml_reader : public event_xml_reader
{
public:
    virtual ~key_event_xml_reader();

    virtual PTR(event) read(CREF(boost::property_tree::ptree::value_type) value);
};

CLOSE_RDO_NAMESPACE

#endif // _RDO_KEY_EVENT_XML_READER_H_