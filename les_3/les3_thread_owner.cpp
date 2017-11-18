
#include "app.h"
#include "pseudo.h"
#include "thread_guard.h"
#include <iostream>
#include <utility>	  // Для метода std::move
#include <thread>
 
using namespace Tools;

class Lesson_3 : public Application {
public:
	void start() const;

};

//----------------------------------------------------------------

using namespace Tools;

/*virtual*/
void Lesson_3::start() const {
	/*
	 * Объект std::thread может в один момент времени контроллировать
	 * только один поток, но вы можете передавать этот контроль между остальными
	 * экземплярами этого класса.
	 *
	 * В следующем примере мы попробуем попередовать владение потоком
	 * между разными экземплярами std::thread.
	 */

	std::thread t1(random_sleep, '#', 20);
	/*
	 * Передаем владение потоком
	 */
	std::thread t2 = std::move(t1);
	/*
	 * Теперь t1 свободен и его можно занять новым потоком
	 */
	t1 = std::thread(random_sleep, '*', 20);
	/*
	 * Сделаем еще одну передачу
	 */
	std::thread t3;
	t3 = std::move(t2);
	/*
	 * Но так мы делать уже не можем, потому что t1 владеет потоком на данный момент
	 */
	// t1 = std::move(t3);
	thread_guard tg1(t1), tg2(t3);

	/*
	 * Представим улучшенную версию класса thread_guard, который позволяет получить
	 * права на поток и контроллировать его завершение.
	 *
	 * С помощью этого класса создание потока занимает одну строку.
	 */
	scoped_thread scthr(std::thread(random_sleep, '+', 5));
	/*
	 * Теперь мы можем делать в этом потоке любые свои дела
	 */
	std::cout << std::endl;

}

//----------------------------------------------------------------

Application* Tools::app_init() {
	return (Application*) new Lesson_3();
}
