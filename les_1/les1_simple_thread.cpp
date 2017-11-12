
#include "app.h"
#include <iostream>

/*
 * Следующий заголовок нужно подключать, если вы хотите использовать потоки STL
 */
#include <thread>
 
using namespace Tools;

class Lesson_1 : public Application {
public:
	void start() const;
	

}; // class Lesson_1

//----------------------------------------------------------------
/*
 * В С++ используют callable объекты, которые выполняют работу
 * в потоке. До появления лямбда выражений обычно использовали функциональные
 * объекты (функторы). Если поток выполняет небольшую работу, то удобно использовать
 * лямбда выражения, так как код получается более компактным.
 *
 * Следующий класс реализует простую фоновую задачу и построен как функтор.
 */
class background_task {
public:
	void operator()() const
	{
		/* 
		 * Сюда вы пишите тело потока. Для простоты мы выведем строку на экран.
		 * Обратите внимание, что мы перехватываем исключения в потоке - этот
		 * момент будет объяснен позже, - поэтому все потоки обычно имеют 
		 * try...catch конструкцию в своем теле.
		 */
		try {
			std::cout << "Hello, World! I'm background task that launched from a functor" << std::endl;
		}
		catch (...) {
			std::cout << __FILE__ << " (" << __LINE__ << ")" << ":" << __FUNCTION__
			          << std::endl;
		}
	}
	
 }; // class background_task

/* 
 * Вы можете использовать простые функции, которые будут запущены в отдельном потоке.
 * 
 * Следующую функцию мы запустим в отдельном потоке. 
 */
 
void background_task_as_function() {
	try {
			std::cout << "Hello, World! I'm background task that launched from a simple function" << std::endl;
	}
	catch (...) {
		std::cout << __FILE__ << " (" << __LINE__ << ")" << ":" << __FUNCTION__
				  << std::endl;
	}
}
 
void Lesson_1::start() {
	std::cout << "Lesson 1\nSimple threads\n" << std::endl;
/* !========
 * Чтобы создать поток из функтора:
 * 
 * 1. Объявите объект как обычно
 */
	background_task 	bt;

/* 2. Используйте низкоуровневый интерфейс потоков STL, чтобы запустить фоновую задачу */
	std::thread my_thread(bt);

/* 3. Выберите, что должен сделать текущий поток: дождаться того, что был запущен, или отсоединиться от него 
 * 
 * В этом примере мы просто отсоединяемся от потока, который был создан
 */
	bt.detach();
/* На этом процесс создания отдельного потока заканчивается. */

/* !========
 * Чтобы создать поток из функции:
 *
 * 1. Воспользуйтесь одной из следующих конструкций
 */
	std::thread my_thread_1((background_task_as_function()));
	my_thread_1.detach();
	// ПРИМЕЧАНИЕ:
	//  обратите внимание, чтобы вызываемая функция была заключена в пару скобок.
	//  Это сделано потому, что без них компилятор воспринимает конструкцию как
	//  вызов функции без параметров.
	
	// Можно воспользоваться универсальным синтаксисом инициализации, введенным в С++11.
	std::thread my_thread_2 {background_task_as_function()};
	my_thread_2.detach();
	
/* !========
 * Чтобы создать поток из лямбда функции, ее необходимо передать каким-либо передать объекту
 * std::thread
 */	
	std::thread my_thread_3([]() {
		try {
			std::cout << "Hello, World! I'm background task that launched from a lambda function" << std::endl;
		}
		catch (...) {
			std::cout << __FILE__ << " (" << __LINE__ << ")" << ":" << __FUNCTION__
					  << std::endl;
		}
	});
	my_thread_3.detach();
}

//----------------------------------------------------------------

Application* Tools::app_init() {
	return (Application*) new Lesson_1();
}
