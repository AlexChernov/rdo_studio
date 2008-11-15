#define _ATL_ATTRIBUTES

#include <atlbase.h>
//CComModule _Module;
#include <atlcom.h>

#include <string>
#include <vector>

// ���������� Fastreports
#if _MSC_VER < 1300

	//////////////////////////////////////////////////////////////////////////
	//
	// Use this for MS Visual Studio 6
	//
	//////////////////////////////////////////////////////////////////////////
	#import "..\\..\\..\\bin\\FastReport3.dll" named_guids
	#import "..\\..\\..\\frxCOM\\frxCOM.dll" named_guids

#else

	//////////////////////////////////////////////////////////////////////////
	//
	// This code preffered for MS Visual Studio.NET
	//
	//////////////////////////////////////////////////////////////////////////
	#import "libid:d3c6fb9b-9edf-48f3-9a02-6d8320eaa9f5" named_guids

#endif

using namespace FastReport;
using namespace std ;

// �������� ������� ���������� ���������
class ReportVar
{
public:
	// ��� ����������
	std::string Name ;
	// ��� ����������
	enum VarTypeValues {WATCH_STATE = 2, WATCH_PAR = 3, GET_VALUE = 4, WATCH_QUANT = 5, WATCH_VALUE = 6} ;
    // ��� ������� enum??????
	int VarType ;
	// ������ ��������� ���������� �� ����������
	vector <std::string> Parameters ;
} ;



class Groop
{
public:
	// ��� ������
	std::string GroopName ;
	// ��� ����������� ������
	enum GroopType {GR_NORMAL = 0, GR_CHART} ;
	//������ ����������, ���������� ������
	vector<ReportVar> Variables ;
	// �������� �� ������ ��������
	bool IsActive ;
} ;


// ���������� �������� ����
#define KEYWORDS_QUANTITY 7

#define NOCOMMENT  1
#define LINECOMMENT 2
#define LONGCOMMENT 3
#define ENDLINECOMMENT 4
#define ENDLONGCOMMENT 5

// ������� ����������� ����� � ������ ����   !!!����� � ������ - ������ :)
int StringToList(std::string, vector <std::string> *) ;

// ������� ������� ����������� �� ������     !!!����� � ������ - ������ :)
int DeleteComments(std::string, std::string *) ;

// ������� ��������� ������ � ������ �����, ������ �� �������� ��� ����������  !!!����� � ������ �����
int AddGroop(vector <Groop> *, std::string, bool ) ;

// ������� ���� �������� ����� � ������ � ���������� �������� �� ��������� ������� !!!����� � ������ - ������)
int FindKeyWord(vector <std::string> List, int StartIndex, int *WhatWord) ;

// ������� ��������� ���������� � �������� ������ !!! ����� � ������ ������
int InsertVar(vector <Groop> *Groops, std::string VarName, int VarType_) ;

// ������� ������������ ���������� � ���������������� �� ����������� !!! ����� � ������ ������
int GetParam(vector <Groop> *Groops, vector <std::string> List) ;


// ������� � �������� ��� ������ ������
class CfrxUserDataSetEvents1 : 
	public CComObjectRoot,
	public IDispatchImpl< 
		IfrxUserDataSetEvents, 
		&__uuidof(IfrxUserDataSetEvents), 
		&LIBID_FastReport>
{
	
	int						idx ;
	int                     *int_counter ;
	vector <Groop> *Groops_output ;

	DWORD					dwCookie ;

	BEGIN_COM_MAP(CfrxUserDataSetEvents1)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IfrxUserDataSetEvents)
	END_COM_MAP()

    STDMETHODIMP raw_OnFirst ( ) 
	{ 
		idx = 0 ;
	    return S_OK ; 
	}
    
	STDMETHODIMP raw_OnNext ( ) 
	{ 
	
	    idx++ ;
	
	return S_OK; 
	}

    STDMETHODIMP raw_OnPrior( ) 
	{ 
	
		idx-- ;
	
	return S_OK ; 
	}
	STDMETHODIMP raw_OnGetValue ( VARIANT ColumnName, VARIANT * Value ) 
	{ 
		if (_variant_t("GroopName") == ColumnName) 
		{		
			_variant_t		v(Groops_output->at(idx).GroopName.c_str()) ;
			VariantCopy(Value, &v) ;
			v.Clear() ;				
		    (*int_counter)++ ;
		}
		
		//if (_variant_t("VarName") == ColumnName) 
		//{
	//		if (flag)
	//		{
	//			flag = false ;
	//			idxx = 0 ;
	//		}
	//		_variant_t		v(Groops_output->at(idx).Variables.at(idxx++).Name.c_str());
	//		VariantCopy(Value, &v) ;
	//		v.Clear() ;	
	//	    if (idxx  == Groops_output->at(idx).Variables.size()) 
	//			EOFflag = false ;
	//	}

		return S_OK;
	}

    STDMETHODIMP raw_OnCheckEOF (VARIANT_BOOL * IsEOF ) 
	{ 
			*IsEOF = ! ( idx < Groops_output->size()) ;
			return S_OK ; 
	}

public:
	HRESULT Advise(IfrxUserDataSet	*	pDataSet, vector <Groop> *Groops, int *counter)
	{
		HRESULT								hr;
		IConnectionPointContainerPtr		pCPC;
		IConnectionPointPtr					pCP;
		CComPtr<IUnknown> pUnk;
		int_counter = counter ;
		Groops_output = Groops ;

		do {

			// Check that this is a connectable object.
			hr = pDataSet->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
			if(FAILED(hr)) break;

			hr = pCPC->FindConnectionPoint(__uuidof(IfrxUserDataSetEvents), &pCP);
			if(FAILED(hr)) break;

			hr = QueryInterface(IID_IUnknown, (void**)&pUnk);
			if(FAILED(hr)) break;

			hr = pCP->Advise( pUnk, &dwCookie);
			if(FAILED(hr)) break;

		} while(false);

		return hr;
	}

	HRESULT Unadvise(IfrxUserDataSet	*	pDataSet)
	{
		HRESULT								hr;
		IConnectionPointContainerPtr		pCPC;
		IConnectionPointPtr					pCP;
		CComPtr<IUnknown> pUnk;
		
		do {

			hr = pDataSet->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
			if(FAILED(hr)) break;

			hr = pCPC->FindConnectionPoint(__uuidof(IfrxUserDataSetEvents), &pCP);
			if(FAILED(hr)) break;

			hr = QueryInterface(IID_IUnknown, (void**)&pUnk);
			if(FAILED(hr)) break;

			hr = pCP->Unadvise( dwCookie);
			if(FAILED(hr)) break;

		} while(false);
		
		return hr;
	}
}; 


class CfrxUserDataSetEvents2 : 
	public CComObjectRoot,
	public IDispatchImpl< 
		IfrxUserDataSetEvents, 
		&__uuidof(IfrxUserDataSetEvents), 
		&LIBID_FastReport>
{
	
	int						idx ;
	int                     *int_counter ;
	vector <Groop> *Groops_output ;

	DWORD					dwCookie ;

	BEGIN_COM_MAP(CfrxUserDataSetEvents2)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IfrxUserDataSetEvents)
	END_COM_MAP()

    STDMETHODIMP raw_OnFirst ( ) 
	{ 
		idx = 0 ;
	    return S_OK ; 
	}
    
	STDMETHODIMP raw_OnNext ( ) 
	{ 
	
	    idx++ ;
	
	return S_OK; 
	}

    STDMETHODIMP raw_OnPrior( ) 
	{ 
	
		idx-- ;
	
	return S_OK ; 
	}
	STDMETHODIMP raw_OnGetValue ( VARIANT ColumnName, VARIANT * Value ) 
	{ 
		//if (_variant_t("GroopName") == ColumnName) 
		//{		
	//		_variant_t		v(Groops_output->at(idx).GroopName.c_str()) ;
	//		VariantCopy(Value, &v) ;
//			v.Clear() ;				
//		}
		
		if (_variant_t("GroopName") == ColumnName) 
		{
	//		if (flag)
	//		{
	//			flag = false ;
	//			idxx = 0 ;
	//		}
			_variant_t		v(Groops_output->at(*int_counter).Variables.at(idx).Name.c_str());
			VariantCopy(Value, &v) ;
			v.Clear() ;	
	
			//_variant_t value = pReport->GetVariable(_bstr_t("second_var")) ;

//			if (idxx  == Groops_output->at(idx).Variables.size()) 
	//			EOFflag = false ;
		}

		return S_OK;
	}

    STDMETHODIMP raw_OnCheckEOF (VARIANT_BOOL * IsEOF ) 
	{ 
			if (*int_counter==-1)
				*IsEOF = ! ( idx < Groops_output->at(0).Variables.size()) ;
			else
		    *IsEOF = ! ( idx < Groops_output->at(*int_counter).Variables.size()) ;
			return S_OK ; 
	}

public:
	HRESULT Advise(IfrxUserDataSet	*	pDataSet, vector <Groop> *Groops, int *counter)
	{
		HRESULT								hr;
		IConnectionPointContainerPtr		pCPC;
		IConnectionPointPtr					pCP;
		CComPtr<IUnknown> pUnk;
		int_counter = counter ;
		Groops_output = Groops ;

		do {

			// Check that this is a connectable object.
			hr = pDataSet->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
			if(FAILED(hr)) break;

			hr = pCPC->FindConnectionPoint(__uuidof(IfrxUserDataSetEvents), &pCP);
			if(FAILED(hr)) break;

			hr = QueryInterface(IID_IUnknown, (void**)&pUnk);
			if(FAILED(hr)) break;

			hr = pCP->Advise( pUnk, &dwCookie);
			if(FAILED(hr)) break;

		} while(false);

		return hr;
	}

	HRESULT Unadvise(IfrxUserDataSet	*	pDataSet)
	{
		HRESULT								hr;
		IConnectionPointContainerPtr		pCPC;
		IConnectionPointPtr					pCP;
		CComPtr<IUnknown> pUnk;
		
		do {

			hr = pDataSet->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
			if(FAILED(hr)) break;

			hr = pCPC->FindConnectionPoint(__uuidof(IfrxUserDataSetEvents), &pCP);
			if(FAILED(hr)) break;

			hr = QueryInterface(IID_IUnknown, (void**)&pUnk);
			if(FAILED(hr)) break;

			hr = pCP->Unadvise( dwCookie);
			if(FAILED(hr)) break;

		} while(false);
		
		return hr;
	}
}; 