void matrixTestCreate()
{
#include "simulator/runtime/test/rdo_matrix_test/matrixCreate.inl"

	if(matrixVal1.getAsString() != _T("[1, 2, 3]")) TRACE("������ � ���������� �������\n");
}

void matrixTestInsert()
{
#include "simulator/runtime/test/rdo_matrix_test/matrixCreate.inl"

	matrixVal1.insert(matrixVal1.begin()+RDOValue(1),matrixVal2.begin(),matrixVal2.end());
	if(matrixVal1.getAsString() != _T("[1, 4, 5, 6, 2, 3]")) TRACE("������ � ������ insert()\n");
}

void matrixTestErase()
{	
#include "simulator/runtime/test/rdo_matrix_test/matrixCreate.inl"

	matrixVal1.erase(matrixVal1.begin()+RDOValue(1),matrixVal1.begin()+RDOValue(3));
	if(matrixVal1.getAsString() != _T("[1]")) TRACE("������ � ������ erase()\n");
}

void matrixTestPPOperator()
{
#include "simulator/runtime/test/rdo_matrix_test/matrixCreate.inl"

	tstring itStr = "";
	for (RDOValue it = matrixVal1.begin(); it != matrixVal1.end(); ++it)
	{
		itStr += it.getAsString();
	}
	if(itStr != _T("123")) TRACE("������ � ++���������\n");
}

void matrixTestOperatorPP()
{
#include "simulator/runtime/test/rdo_matrix_test/matrixCreate.inl"

	tstring itStr = "";
	for (RDOValue it = matrixVal1.begin(); it != matrixVal1.end(); it++)
	{
		itStr += it.getAsString();
	}
	if(itStr != _T("123")) TRACE("������ � ���������++\n");
}

void matrixTestMMOperator()
{
#include "simulator/runtime/test/rdo_matrix_test/matrixCreate.inl"

	tstring itStr = "";
	RDOValue it1 = matrixVal2.end();
	do 
	{
		--it1;
		itStr += it1.getAsString();
	} while (it1 != matrixVal2.begin());
	if(itStr != _T("654")) TRACE("������ � --���������\n");
}

void matrixTestOperatorMM()
{
#include "simulator/runtime/test/rdo_matrix_test/matrixCreate.inl"

	tstring itStr = "";
	RDOValue it1 = matrixVal2.end();
	do 
	{
		it1--;
		itStr += it1.getAsString();
	} while (it1 != matrixVal2.begin());
	if(itStr != _T("654")) TRACE("������ � ���������--\n");
}