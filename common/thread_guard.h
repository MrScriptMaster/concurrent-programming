/*
 * thread_guard.h
 *
 *  Created on: 18 нояб. 2017 г.
 *      Author: Script
 */

#ifndef COMMON_THREAD_GUARD_H_
#define COMMON_THREAD_GUARD_H_

#include <thread>
#include <utility>
#include <stdexcept>

namespace Tools {

class thread_guard {
	std::thread& _thr;

public:
	explicit thread_guard(std::thread& thread)
	: _thr(thread)
	{

	}

	virtual ~thread_guard() {
		if (_thr.joinable()) {
			_thr.join();
		}
	}
	/*
	 * Создавать копии объектов этого класса потенциально опасно, так как
	 * срок их жизни может быть больше, чем у объекта, на который ссылается
	 * _thr.
	 */
	thread_guard(thread_guard const& ) =delete;
	thread_guard& operator= (thread_guard const&) =delete;

}; // class thread_guard

/*
 * Следующий класс является улучшенной версией thread_guard, так как
 * он позволяет получить права на передаваемый ему поток и полностью
 * контроллировать время его существования.
 */

class scoped_thread {
	std::thread _thr;

public:
	explicit scoped_thread(std::thread t)
	: _thr(std::move(t))
	{
		if (!_thr.joinable())
			throw std::logic_error("No thread");
	}

	virtual ~scoped_thread() {
		_thr.join();
	}

	scoped_thread(scoped_thread const&) =delete;
	scoped_thread& operator=(scoped_thread const&) =delete;

}; // class scoped_thread

} // namespace Tools

#endif /* COMMON_THREAD_GUARD_H_ */
