/*!
  \copyright (c) RDO-Team, 2012
  \file      app/rdo_console/rdo_mouse_event_xml_reader.cpp
  \author    ��������� ������� (lord.tiran@gmail.com)
  \date      26.11.2012
  \brief     ���������� ������ RDO
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/rdo_mouse_event.h"
#include "app/rdo_console/rdo_mouse_event_xml_reader.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

mouse_event_xml_reader::~mouse_event_xml_reader()
{
}

PTR(event) mouse_event_xml_reader::read(CREF(boost::property_tree::ptree::value_type) value)
{
    tstring name = value.second.get<tstring>("<xmlattr>.name", "");
    double time  = value.second.get<double>("<xmlattr>.time", 0.0);

    boost::property_tree::ptree const& param = value.second.get_child("param");

    int button = param.get<int>("<xmlattr>.button");
    int x = param.get<int>("<xmlattr>.x");
    int y = param.get<int>("<xmlattr>.y");

    return new mouse_event(name, time, static_cast<mouse_event::buttons>(button), x, y);
}

CLOSE_RDO_NAMESPACE
