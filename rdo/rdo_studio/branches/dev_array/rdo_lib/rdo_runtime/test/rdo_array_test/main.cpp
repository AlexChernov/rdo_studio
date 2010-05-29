// ====================================================================== INCLUDES
#include <iostream>
// ====================================================================== SYNOPSIS
#include "rdo_lib\rdo_runtime\rdo_value.h"
#include "rdo_lib\rdo_runtime\rdo_enum.h"
#include "rdo_lib\rdo_runtime\rdo_array.h"
#include "rdo_lib\rdo_runtime\rdo_type.h"
// ===============================================================================

using namespace rdoRuntime;

void main()
{
	RDOEnumType* enumType = new RDOEnumType(NULL, RDOEnumType::Enums("��������_1")("��������_2")("��������_3"));
	RDOValue integer = 1;
	RDOValue real    = 1.5;
	RDOValue booling = true;
	RDOValue string  = "string";
	RDOArrayType* arrayType_int    = new RDOArrayType(NULL,&integer.type());
	RDOArrayType* arrayType_real   = new RDOArrayType(NULL,&real.type());
	RDOArrayType* arrayType_string = new RDOArrayType(NULL,&string.type());
	RDOArrayType* arrayType_bool   = new RDOArrayType(NULL,&booling.type());
	RDOArrayType* arrayType_enum   = new RDOArrayType(NULL,enumType);
	RDOArrayType* arrayType5       = new RDOArrayType(NULL,arrayType_enum);
	RDOArrayType* arrayType6       = new RDOArrayType(NULL,(new RDOArrayType(NULL,arrayType_int)));
	//std::string className1 = arrayType_int->asString();
	//std::string className2 = arrayType_real->asString();
	//std::string className3 = arrayType_string->asString();
	//std::string className4 = arrayType_bool->asString();
	//std::string className5 = arrayType_enum->asString();
	//std::string className6 = arrayType5->asString();
	//std::string className7 = arrayType6->asString();

	int i = 1;
}