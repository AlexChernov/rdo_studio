/*
  \copyright (c) RDO-Team, 2011
  \file      update_i.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      29.10.2010
  \brief     
  \indent    4T
 */

#ifndef _CONVERTOR_UPDATE_I_H_
#define _CONVERTOR_UPDATE_I_H_

// ====================================================================== INCLUDES
#include <fstream>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/namespace.h"
#include "rdo_lib/rdo_converter/rdo_common/model_objects_convertor.h"
#include "rdo_lib/rdo_converter/update/document_i.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- IDocUpdate
// ----------------------------------------------------------------------------
S_INTERFACE(IDocUpdate)
{
public:
	class Position
	{
	public:
		enum
		{
			POSITION_BEGIN = ~0,
			POSITION_END   = ~1
		};

		Position(ruint          pos);
		Position(CREF(Position) pos);

		ruint get  () const;

		rbool begin() const;
		rbool end  () const;
		rbool real () const;

		void     operator+= (CREF(Position) pos);
		void     operator-= (CREF(Position) pos);
		Position operator+  (CREF(Position) pos) const;
		Position operator-  (CREF(Position) pos) const;
		rbool    operator<= (CREF(Position) pos) const;
		rbool    operator>= (CREF(Position) pos) const;
		rbool    operator<  (CREF(Position) pos) const;
		rbool    operator>  (CREF(Position) pos) const;
		rbool    operator== (CREF(Position) pos) const;
		rbool    operator!= (CREF(Position) pos) const;

	private:
		ruint m_position;
	};

	virtual void apply (REF(LPIDocument) pDocument) const                             = 0;
	virtual void insert(IDocument::Type type, CREF(Position) to,   ruint size)        = 0;
	virtual void remove(IDocument::Type type, CREF(Position) from, CREF(Position) to) = 0;
	virtual void dump  (REF(LPIDocument) pDocument) const                             = 0;
};
#define DECLARE_IDocUpdate                                                     \
	void apply (REF(LPIDocument) pDocument) const;                             \
	void insert(IDocument::Type type, CREF(Position) to, ruint size);          \
	void remove(IDocument::Type type, CREF(Position) from, CREF(Position) to); \
	void dump  (REF(LPIDocument) pDocument) const;

OBJECT(DocUpdate) IS IMPLEMENTATION_OF(IDocUpdate)
{
protected:
	DocUpdate(IDocument::Type file = IDocument::UNDEFINED);

	IDocument::Type m_file;

	IDocument::Type getCurrentType() const;
};

CLOSE_RDO_CONVERTER_NAMESPACE

#include "rdo_lib/rdo_converter/update/update_i.inl"

#endif //! _CONVERTOR_UPDATE_I_H_
