/*
  \copyright (c) RDO-Team, 2011
  \file      document.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      01.11.2010
  \brief     
  \indent    4T
 */

#ifndef _CONVERTOR_DOCUMENT_H_
#define _CONVERTOR_DOCUMENT_H_

// ====================================================================== INCLUDES
#include <fstream>
#include <boost/shared_ptr.hpp>
// ====================================================================== SYNOPSIS
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#include "rdo_common/rdomacros.h"

#include "rdo_lib/rdo_converter/namespace.h"
#include "rdo_lib/rdo_converter/rdo_common/model_objects_convertor.h"
#include "rdo_lib/rdo_converter/update/document_i.h"
#include "rdo_lib/rdo_converter/update/update_i.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- Document
// ----------------------------------------------------------------------------
OBJECT(Document)
	IS IMPLEMENTATION_OF(IDocument)
{
DECLARE_FACTORY(Document)
public:
	typedef rdoModelObjectsConvertor::RDOFileTypeOut TypeOut;

	void    create      (CREF(tstring) filePath, CREF(tstring) modelName);
	void    init        (rdoModelObjectsConvertor::RDOFileTypeIn type, REF(std::ifstream) stream);
	void    insertUpdate(CREF(LPDocUpdate) pUpdate);
	void    convert     ();
	void    close       ();
	tstring getName     (TypeOut typeOut) const;

private:
	 Document();
	~Document();

	class MemoryStream
	{
	public:
		typedef std::vector<char> Buffer;

		void    init  (REF(std::ifstream) stream);
		void    get   (REF(std::ofstream) stream) const;

		void    insert(ruint to, CREF(tstring) value);
		void    remove(ruint from, ruint to);

		tstring get   (ruint from, ruint to);

	private:
		Buffer m_buffer;
	};

	typedef boost::shared_ptr<MemoryStream>  LPMemoryStream;
	typedef boost::shared_ptr<std::ofstream> LPFileStream;
	typedef std::map<Type, LPMemoryStream>   MemoryFileList;
	typedef std::map<TypeOut, LPFileStream>  StreamFileList;

	typedef std::pair<LPDocUpdate, rbool> Update;
	typedef std::list<Update>             UpdateContainer;

	tstring         m_filePath;
	tstring         m_modelName;
	MemoryFileList  m_memoryFileList;
	StreamFileList  m_streamFileList;
	UpdateContainer m_updateContainer;

	LPMemoryStream getMemoryStream(Type    type);
	LPFileStream   getFileStream  (TypeOut type);

	TypeOut typeToOut(CREF(Type) typeIn) const;

	DECLARE_IDocument;
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_DOCUMENT_H_
