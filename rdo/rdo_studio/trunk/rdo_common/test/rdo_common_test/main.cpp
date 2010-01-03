/*
 * copyright: (c) RDO-Team, 2009
 * filename : main.cpp
 * author   : ������ ������
 * date     : 10.05.09
 * bref     : ���� common-����������
 * indent   : 4T
 */

// ====================================================================== PCH
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdocommon.h"
#include "rdo_common/rdofile.h"
#include "rdo_common/rdotime.h"
#include "rdo_common/test/rdo_common_test/resource.h"
// ===============================================================================

void main()
{
	tstring str1 = rdo::format(IDS_STRING101);
	tstring str2 = rdo::format(IDS_STRING102, 22);
	tstring str3 = rdo::format(IDS_STRING103, str1.c_str(), 33, str2.c_str());

	rbool exist     = rdo::File::exist    (_T("����� main.cpp"));
	rbool read_only = rdo::File::read_only(_T("����� main.cpp"));
	rbool unlink    = rdo::File::unlink   (_T("����� main.cpp"));

	rdo::Time time     = rdo::Time::local();
	tstring   time_str = time.asString();

	int i = 1;
}
