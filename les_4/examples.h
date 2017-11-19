
#ifndef LES_4_EXAMPLES_H_
#define LES_4_EXAMPLES_H_

#include <list>
#include <mutex>
#include <algorithm>
#include <iostream>

#include "thread_guard.h"

class Example {
public:
	virtual void demonstrate() =0;
	virtual ~Example() {}
};

/*
 * В этом примере у нас есть список, с которым работает одновременно несколько потоков.
 * Мы используем такой механизм синхронизации как мьютекс (mutex), который не позволяет
 * обращаться одновременно к этому списку одновременно с каждого потока, так как операции,
 * которые они выполняют не могут производиться одновременно впринципе.
 */

class Example_1 : public Example {
	std::list<int> m_List;
	std::mutex m_ListMitex;
	int RIGHT_EDGE;

public:
	void demonstrate() {
		std::cout << "Example 1" << std::endl << "Example demonstrates using of a mutex for adding and searching elements in the list from two routines" << std::endl;

		using namespace Tools;
		// запускаем поток для поиска. Запускаем его немного пораньше
		scoped_thread start_searching_routine(std::thread(&Example_1::routine_thread_2, this));
		// запускаем поток для создания
		scoped_thread start_creation_routine(std::thread(&Example_1::routine_thread_1, this));
	}

	explicit Example_1 ()
		: RIGHT_EDGE(5)
	{}
	explicit Example_1(int num)
		: RIGHT_EDGE(num)
	{}

	/*
	 * Добавляет в конец списка новый элемент. Это может быть не быстрая операция, и в это время
	 * ни один поток не должен работать со списком.
	 */
	void addToList(int new_num) {
		/*
		 * Мы используем встроенный класс lock_guard, который захватывает мьютекс, если он
		 * не занят, и освобождает его, когда удаляется. Если мьютекс уже кем то занят, то
		 * поток блокируется на нем.
		 */
		std::lock_guard<std::mutex> guard(m_ListMitex);
		m_List.push_back(new_num);
	}

	/*
	 * Возвращает true, если список содержит передаваемый элемент. Эта операция использует
	 * библиотечный алгоритм поиска, поэтому список не должен изменяться, пока поиск не
	 * закончился.
	 */
	bool list_contains(int value_to_find) {
		std::lock_guard<std::mutex> guard(m_ListMitex);
		return std::find(m_List.begin(), m_List.end(), value_to_find) != m_List.end();
	}

	void routine_thread_1() {
		/*
		 * В этом потоке мы будем генерировать числа в списке
		 */
		for (int i = 0; i <= RIGHT_EDGE; i++) {
			addToList(i);
		}
	}

protected:
	void routine_thread_2() {
		/*
		 * В этом потоке мы будем искать правое значение, а так как правое значение
		 * может быть еще не создано, то поток будет периодически включаться для повторного
		 * поиска.
		 */
		while (!list_contains(RIGHT_EDGE)) {
			std::cout << __FUNCTION__ << ": " << RIGHT_EDGE << " not found" << std::endl;
		}
	}

}; // class Example_1

#endif /* LES_4_EXAMPLES_H_ */
