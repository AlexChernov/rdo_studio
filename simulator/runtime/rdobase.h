/*!
  \copyright (c) RDO-Team, 2011
  \file      rdobase.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.06.2006
  \brief     Управление симулятором
  \indent    4T
*/

#ifndef _LIB_RUNTIME_BASE_H_
#define _LIB_RUNTIME_BASE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <list>
#include <map>
#include <boost/function.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/runtime/rdo_object.h"
#include "simulator/runtime/mode.h"
#include "simulator/runtime/rdo_runtime_interface_registrator.h"
#ifdef COMPILER_VISUAL_STUDIO
	#ifdef RDO_MT
		#include <afxwin.h>
	#else
		#include <windows.h>
	#endif
#endif // COMPILER_VISUAL_STUDIO
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class     RDOSimulatorBase
  \brief     Один из базовых классов для RDORuntime
*/
PREDECLARE_POINTER(RDOSimulatorBase);
class RDOSimulatorBase: public rdo::counter_reference
{
DECLARE_FACTORY(RDOSimulatorBase)
public:
	// Публичные методы управления симулятором
	virtual void rdoInit();
	virtual bool rdoNext();
	virtual void rdoPostProcess();

	void   setStartTime  (double value);
	double getCurrentTime() const;

	RunTimeMode getMode() const;
	void        setMode(RunTimeMode _mode);

	double getSpeed() const;
	void setSpeed(double persent);

	double getShowRate() const;
	void setShowRate(double value);

	typedef  boost::function<void ()>  EventFunction;
	void addTimePoint   (double timePoint, const LPIBaseOperation& eventID, const EventFunction& eventFunction);
	void removeTimePoint(const LPIBaseOperation& eventID);

	void inc_cnt_events();
	void inc_cnt_choice_from();

	std::size_t get_cnt_events();
	std::size_t get_cnt_choice_from();
	std::size_t get_cnt_calc_arithm() const;
	std::size_t get_cnt_calc_logic() const;

	static std::size_t getMSec(CREF(boost::posix_time::ptime) ptime);

protected:
	RDOSimulatorBase();
	virtual ~RDOSimulatorBase();

	typedef  std::pair<LPIBaseOperation, EventFunction>  BOPlannedItem;
	typedef  std::list<BOPlannedItem>                    BOPlannedList;
	typedef  std::map<double, BOPlannedList>             BOPlannedMap;

	BOPlannedMap m_timePoints;
	bool m_checkOperation;

	void setCurrentTime(double value);

	// Выполнение любых операций (паттерны, DPT и процессы)
	// Если вернулось значение true, то необходимо вызвать doOperation
	// и в следующий раз/ без перевода модельного времени вперед
	virtual bool doOperation() = 0;

	//! Проверка на условие конца моделирования
	virtual bool endCondition() = 0;

	//! Проверка на точки останова
	virtual bool breakPoints() = 0;

	//! Инициализация симулятора
	virtual void onInit()    = 0;

	//! Очистка симулятора
	virtual void onDestroy() = 0;

	// Различные функции конца моделирования
	// Вызывется, если нет больше событий
	virtual void onNothingMoreToDo() = 0;
	// Вызывется, если сработало условие конца моделирования
	virtual void onEndCondition()    = 0;
	// Вызывется, tесли возникла ошибка прогона
	virtual void onRuntimeError()    = 0;
	// Вызывается, если модель остановлена пользователем
	virtual void onUserBreak()       = 0;

	// Вызывается в самом начале прогона
	virtual void preProcess()  = 0;
	// Вызывается непосредвтсенно перед окончанием прогона
	virtual void postProcess() = 0;

	// Проверка на нажатие клавиши или активной области
	virtual bool isKeyDown() const = 0;

	/// @todo не ошибочная ли это реализация по умолчанию?
	// Вызывается при увеличении модельного времени
	virtual void onNewTimeNow();

private:
	double m_startTime;
	double m_currentTime;
	double m_nextTime;


	RunTimeMode  m_mode;

	double m_speed;
	std::size_t m_speed_range_max;
	std::size_t m_next_delay_count;
	std::size_t m_next_delay_current;

	double m_showRate;
	double m_msec_wait;
	std::size_t m_msec_prev;

	std::size_t m_cnt_events;
	std::size_t m_cnt_choice_from;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdobase.inl"

#endif // _LIB_RUNTIME_BASE_H_
