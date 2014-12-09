#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <map>
#include <list>
#include <iostream>

#include <boost/shared_ptr.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "app/rdo_console/events/rdo_event.h"
#include "app/rdo_console/events/xml_parser/rdo_event_xml_reader.h"
// --------------------------------------------------------------------------------

namespace rdo {

class event_xml_parser
{
public:
    typedef std::map< event::etime, boost::shared_ptr<event> > event_container;

public:
    bool register_parser(const std::string& name, boost::shared_ptr<event_xml_reader> reader);

    void parse(std::istream& stream, event_container& list) const;

private:
    typedef std::map<std::string, boost::shared_ptr<event_xml_reader>> parsers;

    parsers m_parsers;
};

} // namespace rdo
