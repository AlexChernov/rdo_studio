#if !defined (REPORT_H)
#define REPORT_H

#define _ATL_ATTRIBUTES
#include <atlbase.h>
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
	std::vector <std::string> Parameters ;
} ;



class Groop
{
public:
	// ��� ������
	std::string GroopName ;
	// ��� ����������� ������
	enum GrType {DATA = 0, CHART} ;
	GrType GroopType ;
	//������ ����������, ���������� ������
	std::vector <ReportVar> Variables ;
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
int StringToList(std::string, std::vector <std::string> *) ;

// ������� ������� ����������� �� ������     !!!����� � ������ - ������ :)
int DeleteComments(std::string, std::string *) ;

// ������� ��������� ������ � ������ �����, ������ �� �������� ��� ����������  !!!����� � ������ �����
int AddGroop(std::vector <Groop> *, std::string, bool ) ;

// ������� ���� �������� ����� � ������ � ���������� �������� �� ��������� ������� !!!����� � ������ - ������)
int FindKeyWord(std::vector <std::string> List, int StartIndex, int *WhatWord) ;

// ������� ��������� ���������� � �������� ������ !!! ����� � ������ ������
int InsertVar(std::vector <Groop> *Groops, std::string VarName, int VarType_) ;

// ������� ������������ ���������� � ���������������� �� ����������� !!! ����� � ������ ������
int GetParam(std::vector <Groop> *Groops, std::vector <std::string> List) ;


// ������� � �������� ��� ������ ������
class CfrxGroopsDataSetEvents : 
	public CComObjectRoot,
	public IDispatchImpl< 
		IfrxUserDataSetEvents, 
		&__uuidof(IfrxUserDataSetEvents), 
		&LIBID_FastReport>
{
	
	int						idx ;
	int                     *Master_Detail_counter ;
	std::vector <Groop> *Groops_output ;

	DWORD					dwCookie ;

	BEGIN_COM_MAP(CfrxGroopsDataSetEvents)
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
		    // �������� ��������� ��������, ��� ������� ���������� �� 1
			(*Master_Detail_counter)++ ;
		}
	
		return S_OK;
	}

    STDMETHODIMP raw_OnCheckEOF (VARIANT_BOOL * IsEOF ) 
	{ 
			*IsEOF = ! ( idx < Groops_output->size()) ;
			return S_OK ; 
	}

public:
	HRESULT Advise(IfrxUserDataSet	*	GroopsDataSet, std::vector <Groop> *Groops, int *counter)
	{
		HRESULT								hr;
		IConnectionPointContainerPtr		pCPC;
		IConnectionPointPtr					pCP;
		CComPtr<IUnknown> pUnk;
		Master_Detail_counter = counter ;
		Groops_output = Groops ;

		do {

			// Check that this is a connectable object.
			hr = GroopsDataSet->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
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

	HRESULT Unadvise(IfrxUserDataSet	*	GroopsDataSet)
	{
		HRESULT								hr;
		IConnectionPointContainerPtr		pCPC;
		IConnectionPointPtr					pCP;
		CComPtr<IUnknown> pUnk;
		
		do {

			hr = GroopsDataSet->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
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


class CfrxVarsDataSetEvents : 
	public CComObjectRoot,
	public IDispatchImpl< 
		IfrxUserDataSetEvents, 
		&__uuidof(IfrxUserDataSetEvents), 
		&LIBID_FastReport>
{
	
	int						idx ;
	int*  Master_Detail_counter ;
	std::vector <Groop> *Groops_output ;

	DWORD					dwCookie ;

	BEGIN_COM_MAP(CfrxVarsDataSetEvents)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IfrxUserDataSetEvents)
	END_COM_MAP()

    STDMETHODIMP raw_OnFirst ( ) 
	{ 
		idx = 0;
	    return S_OK; 
	}
    
	STDMETHODIMP raw_OnNext ( ) 
	{ 
	    idx++;
		return S_OK; 
	}

    STDMETHODIMP raw_OnPrior( ) 
	{ 
		idx--;
		return S_OK; 
	}

	STDMETHODIMP raw_OnGetValue ( VARIANT ColumnName, VARIANT*  Value ) 
	{ 
		if ( _variant_t( "VarName" ) == ColumnName ) 
		{
			_variant_t		v( Groops_output->at( *Master_Detail_counter ).Variables.at( idx ).Name.c_str() );
			VariantCopy( Value, &v );
			v.Clear();	
			return S_OK;
		}
		if ( _variant_t( "Par_1" ) == ColumnName ) 
		{
			_variant_t		v( Groops_output->at( *Master_Detail_counter ).Variables.at( idx ).Parameters.at(0).c_str() );
			VariantCopy( Value, &v );
			v.Clear();	
			return S_OK;
		}
		if ( _variant_t( "Par_2" ) == ColumnName ) 
		{
			_variant_t		v( Groops_output->at( *Master_Detail_counter ).Variables.at( idx ).Parameters.at(1).c_str() );
			VariantCopy( Value, &v );
			v.Clear();	
			return S_OK;
		}
		if ( _variant_t( "Par_3" ) == ColumnName ) 
		{
			_variant_t		v( Groops_output->at( *Master_Detail_counter ).Variables.at( idx ).Parameters.at(2).c_str() );
			VariantCopy( Value, &v );
			v.Clear();	
			return S_OK;
		}
		if ( _variant_t( "Par_4" ) == ColumnName ) 
		{
			_variant_t		v( Groops_output->at( *Master_Detail_counter ).Variables.at( idx ).Parameters.at(3).c_str() );
			VariantCopy( Value, &v );
			v.Clear();	
			return S_OK;
		}
		if ( _variant_t( "Par_5" ) == ColumnName ) 
		{
			_variant_t		v( Groops_output->at( *Master_Detail_counter ).Variables.at( idx ).Parameters.at(4).c_str() );
			VariantCopy( Value, &v );
			v.Clear();	
			return S_OK;
		}
		if ( _variant_t( "Par_6" ) == ColumnName ) 
		{
			_variant_t		v( Groops_output->at( *Master_Detail_counter ).Variables.at( idx ).Parameters.at(5).c_str() );
			VariantCopy( Value, &v );
			v.Clear();	
			return S_OK;
		}
	}

    STDMETHODIMP raw_OnCheckEOF (VARIANT_BOOL * IsEOF ) 
	{ 
			if ( *Master_Detail_counter == -1 )
				*IsEOF = ! ( idx < Groops_output->at(0).Variables.size()) ;
			else
		    *IsEOF = ! ( idx < Groops_output->at(*Master_Detail_counter).Variables.size()) ;
			return S_OK ; 
	}

public:
	HRESULT Advise(IfrxUserDataSet*	GroopsDataSet, std::vector <Groop> *Groops, int *counter)
	{
		HRESULT								hr;
		IConnectionPointContainerPtr		pCPC;
		IConnectionPointPtr					pCP;
		CComPtr<IUnknown> pUnk;
		Master_Detail_counter = counter ;
		Groops_output = Groops ;

		do {

			// Check that this is a connectable object.
			hr = GroopsDataSet->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
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

	HRESULT Unadvise(IfrxUserDataSet*	GroopsDataSet)
	{
		HRESULT								hr;
		IConnectionPointContainerPtr		pCPC;
		IConnectionPointPtr					pCP;
		CComPtr<IUnknown> pUnk;
		
		do {

			hr = GroopsDataSet->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
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

// ������� � �������� ��� ������ ������
class CfrxChartDataSetEvents : 
	public CComObjectRoot,
	public IDispatchImpl< 
		IfrxUserDataSetEvents, 
		&__uuidof(IfrxUserDataSetEvents), 
		&LIBID_FastReport>
{
	
	int						idx, i ;
	
	Groop ChartGroop ;

	DWORD					dwCookie ;

	BEGIN_COM_MAP(CfrxChartDataSetEvents)
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
		if ( _variant_t("Name") == ColumnName ) 
		{		
			_variant_t		v( ChartGroop.Variables.at( idx ).Name.c_str() );
			VariantCopy( Value, &v );
			v.Clear();				
			return S_OK;
		}
		if ( _variant_t("Par") == ColumnName ) 
		{		
			_variant_t		v( ChartGroop.Variables.at( idx ).Parameters.at(2).c_str() );
			VariantCopy( Value, &v );
			v.Clear();				
			return S_OK;
		}
	}

    STDMETHODIMP raw_OnCheckEOF (VARIANT_BOOL * IsEOF ) 
	{ 
			*IsEOF = ! ( idx < ChartGroop.Variables.size()) ;
			return S_OK ; 
	}

public:
	HRESULT Advise(IfrxUserDataSet	*	GroopsDataSet, std::vector <Groop> *Groops)
	{
		HRESULT								hr;
		IConnectionPointContainerPtr		pCPC;
		IConnectionPointPtr					pCP;
		CComPtr<IUnknown> pUnk;
		
		for ( i = 0; i < Groops->size(); i++ )
			if ( Groops->at( i ).GroopType == Groops->at( i ).CHART )
				break;
			
		if ( i < Groops->size() )
			ChartGroop = Groops->at( i );
		
		do {

			// Check that this is a connectable object.
			hr = GroopsDataSet->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
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

	HRESULT Unadvise(IfrxUserDataSet	*	GroopsDataSet)
	{
		HRESULT								hr;
		IConnectionPointContainerPtr		pCPC;
		IConnectionPointPtr					pCP;
		CComPtr<IUnknown> pUnk;
		
		do {

			hr = GroopsDataSet->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
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

#endif