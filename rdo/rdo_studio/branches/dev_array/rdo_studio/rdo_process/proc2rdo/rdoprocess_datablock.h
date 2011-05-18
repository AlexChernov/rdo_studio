/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdoprocess_datablock.h
 * author   : ������� ������
 * date     : 04.05.2011
 * bref     : 
 * indent   : 4T
 */

#ifndef RDO_PROCESS_DATABLOCK_H
#define RDO_PROCESS_DATABLOCK_H

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// ===============================================================================

// ----------------------------------------------------------------------------
// ---------- RPShapeDataBlock
// ----------------------------------------------------------------------------
OBJECT(RPShapeDataBlock)
{
public:
	enum zakonRaspr
	{
		Const,
		Normal,
		Uniform,
		Exp
	};

	void setBase  (int base   );
	void setAmount(int amount );
	void setDisp  (double disp);
	void setExp   (double exp );

	zakonRaspr    getZakon() const { return m_zakon; }
	CREF(tstring) getName () const { return m_name;  }
	int           getBase () const { return m_base;  }
	double        getDisp () const { return m_disp;  }
	double        getExp  () const { return m_exp;   }

protected:
	RPShapeDataBlock(zakonRaspr zakon, CREF(tstring) name);
	virtual ~RPShapeDataBlock() {}

	zakonRaspr m_zakon;
	tstring    m_name;
	int        m_base;
	double     m_exp;
	double     m_disp;
};

// ----------------------------------------------------------------------------
// ---------- RPShapeDataBlockCreate
// ----------------------------------------------------------------------------
class RPShapeDataBlockCreate: public RPShapeDataBlock
{
DECLARE_FACTORY(RPShapeDataBlockCreate)
public:
	int  getAmount() const      { return m_amount; }
	void setAmount(int amount);

private:
	RPShapeDataBlockCreate(RPShapeDataBlock::zakonRaspr zakon, CREF(tstring) name);
	virtual ~RPShapeDataBlockCreate();

	int m_amount;
};
DECLARE_POINTER(RPShapeDataBlockCreate);

// ----------------------------------------------------------------------------
// ---------- RPShapeDataBlockTerminate
// ----------------------------------------------------------------------------
OBJECT(RPShapeDataBlockTerminate)
{
DECLARE_FACTORY(RPShapeDataBlockTerminate)
public:
	void    setTermInc(int term_inc);
	int     getTermInc() const        { return m_term_inc; }
	tstring getName   () const        { return m_name;     }

private:
	RPShapeDataBlockTerminate(CREF(tstring) name);
	virtual ~RPShapeDataBlockTerminate();

	int     m_term_inc;
	tstring m_name;
};

// ----------------------------------------------------------------------------
// ---------- RPShapeDataBlockProcess
// ----------------------------------------------------------------------------
class RPShapeDataBlockProcess: public RPShapeDataBlock
{
DECLARE_FACTORY(RPShapeDataBlockProcess)
public:
	enum resAction
	{
		Advance,
		Seize,
		Release
	};

	typedef std::list<resAction> ActionList;
	typedef std::list<tstring  > ResList;

	void addAction(resAction action);
	void addRes   (CREF(tstring) res);
	
	CREF(ActionList) getAction() const;
	CREF(ResList)    getRes   () const;

private:
	RPShapeDataBlockProcess(RPShapeDataBlock::zakonRaspr zakon, CREF(tstring) name);
	virtual ~RPShapeDataBlockProcess();

	ActionList  m_actions;
	ResList     m_res;
};
DECLARE_POINTER(RPShapeDataBlockProcess);

#endif //! RDO_PROCESS_DATABLOCK_H
