
#ifndef LES_4_EXAMPLES_H_
#define LES_4_EXAMPLES_H_

#include <list>
#include <mutex>
#include <algorithm>
#include <iostream>
#include <string>

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
	// Стандартный список библиотеки STL не поддерживает многопоточности, поэтому нужно
	// предпринять меры, чтобы его в один момент времени нельзя было бы
	// и читать и редактировать.
	std::list<int> m_List;
	// Этот мьютекс защищает список m_List от использования его разными потоками
	std::mutex m_ListMitex;
	int RIGHT_EDGE;

	/*
	 * ПРИМЕЧАНИЕ:
	 * Очень важно, чтобы доступ к списку не был осуществлен в обход захвата мьютекса, иначе
	 * неизбежны ошибки. Никогда нельзя открывать к защищенным объектом доступ другим объектам ни в виде
	 * изменения видимости членов класса, ни через методы класса.
	 */

public:
	void demonstrate() {
		std::cout << "Example 1" << std::endl << "Example demonstrates using of a mutex "
			"for adding and searching elements in the list from two routines" << std::endl;

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
		 * не занят, и освобождает его, когда вызывается деструктор. Если мьютекс уже кем то занят, то
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
		/*
		 * Здесь мы также закхватываем мьютекс, чтобы во время поиска
		 * потоку никто не мешал.
		 */
		std::lock_guard<std::mutex> guard(m_ListMitex);
		return std::find(m_List.begin(), m_List.end(), value_to_find) != m_List.end();
	}

	void routine_thread_1() {
		/*
		 * Этот метод мы будем исполнять в потоке, чтобы порождать элементы в списке.
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

//------------------------------------------------------------------

/*
 * На практике иногда приходится захватывать несколько мьютексов за один раз.
 * Чтобы избежать возможной взимоблокировки между потоками, которые могут захватывать
 * по несколько мьютексов за один раз, следует придерживаться следующих рекомендаций:
 *	1. Захватывать и освобождать мьютексы всегда в одном и том же порядке.
 *	2. Пытаться захватить все необходимые для процедуры мьютексы, а если это не удается сделать, прервать выполнение порцедуры.
 *	
 * В следующем примере показан класс, который вызывает дружественную ему процедуру, которая ничего не делает, но предполагается,
 * что для ее работы оба аргумента должны сохранять свою целостность.
 */

class SomeBigObject {
	std::string m_Name;
public:
	SomeBigObject(const char* str)
	: m_Name(str)
	{}
	
	const std::string& getName() const {
		return m_Name;
	}
	
	void setName(const std::string& str) {
		m_Name = str;
	}
};	// class SomeBigObject

/*
 * Для примера этот метод делает обмен именами между объектами
 * класса SomeBigObject
 */
void swapper(SomeBigObject& lhs, SomeBigObject& rhs) {
	std::string buffer(lhs.getName());
	lhs.setName(rhs.getName());
	rhs.setName(buffer);
	std::cout << " swapped" << std::endl;
}

class Example_2 : public Example {	
	class SwapProcessor {
		SomeBigObject  m_Object;
		std::mutex     m_Mutex;
	public:
		SwapProcessor(SomeBigObject const& obj)
		: m_Object(obj)
		{}
		friend void swap(SwapProcessor& lhs, SwapProcessor& rhs) {
			// Чтобы случайно не звахватить мьютекс дважды, мы
			// проверяем, что оба аргумента не ссылаются на одно и то же
			if (&lhs == &rhs)
				return;
			// захватываем мьютексы в процедуре
			std::lock(lhs.m_Mutex,rhs.m_Mutex);
			/*
			 * Чтобы передать объектам типа guard права на мьютексы, 
			 * мы используем параметр std::adopt_lock. Без него guard попытался 
			 * бы захватить мьютекс снова
			 */
			std::lock_guard<std::mutex> lock_a(lhs.m_Mutex,std::adopt_lock);
			std::lock_guard<std::mutex> lock_b(rhs.m_Mutex,std::adopt_lock);
			swapper(lhs.m_Object,rhs.m_Object);
		}
	}; // class SwapProcessor
public:
	void demonstrate() {
		/*
		 * В этом примере два потока попытаются вызвать swapper для одних и тех же данных
		 */
		std::cout << "Example 2" << std::endl << "Catch two mutexes" << std::endl;
		
		using namespace Tools;
		// запускаем первый поток
		scoped_thread r1(std::thread(&Example_2::routine, this));
		// запускаем второй поток
		scoped_thread r2(std::thread(&Example_2::routine, this));
	}
private:
	void routine() {
		//sp1.swap(sp1,sp2);
	} 
};	// class Example_2
 
#endif /* LES_4_EXAMPLES_H_ */
