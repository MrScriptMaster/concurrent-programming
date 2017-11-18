
#include "app.h"
#include "pseudo.h"
#include <iostream>

#include <thread>
#include <string>
#include <functional> // Для std::ref
#include <utility>	  // Для метода std::move
#include <memory>	  // Для std::unique_ptr
 
using namespace Tools;

class Lesson_2 : public Application {
public:
	void start() const;
};

//----------------------------------------------------------------

/*
 * Объект этого класса мы передадим, чтобы вызвать его функцию-член.
 */
class SomeClass {
	std::string data;
public:
	void do_some_work() {
		std::cout << "Hello" << std::endl;
		return;
	}
	void prepareData(const std::string& s) {
		data = s;
	}
	void showData() {
		std::cout << data << std::endl;
	}

};

void process_big_object(std::unique_ptr<SomeClass> ptr) {
	ptr->showData();
}

/*virtual*/
void Lesson_2::start() const {
  /*
   * В потоки можно передавать аргументы.
   * 
   * В следующем примере мы создаем потоки, используя функцию, которая принимает два аргумента.
   */
  
  std::thread thrd(random_sleep, '#', 7);
  thrd.detach();
  std::thread thrd1(random_sleep, '*', 15);
  thrd1.join();
  
  std::cout << std::endl;
  /*
   * Вы должны увидеть, как в потоке вывода будут отображаться символы # (7 раз) и * (15 раз).
   * Например,
   * #**##**#*##*#*********
   */

  //----------------------------

  /*
   * Важно, что, если передаваемые параметры не копируются в поток, их время жизни должно быть
   * больше, чем время жизни потока.
   *
   * Предпочтительно передавать параметры по значению, но в реальных программах
   * не все объекты можно копировать. Объекты копируются даже тогда, когда ожидается
   * ссылка.
   *
   * При передаче указателей нужно быть очень внимательным, так как предполагая, что вы копируете
   * объект, вы можете просто передать поток указатель.
   *
   * Пример
   */

  auto routine = [](int number, std::string const& s) -> void {
	  std::cout << s << " " << number << std::endl;
	  return;
  };

  char buffer[1024] = "String for routine";
  /*
   * В этом примере было бы ошибочным вызывать новый поток так
   *
   * std::thread t(routine, 1, buffer);
   *
   * Потому что преобразование buffer в string будет происходить в контексте
   * потока, а указатель уже может быть не актуален (висячий указатель), когда мы делаем
   * для потока detach.
   *
   * Предпочтительно будет сделать преобразование в string до создания потока.
   */

  std::thread t(routine, 1, std::string(buffer));
  t.detach();
  /*
   * Но иногда вы действительно хотите передать ссылку, а конструктор std::thread
   * вам это сам по себе не сделает.
   *
   * Правильнее в этом случае обернуть объект в std::ref.
   */
  std::string str("Referenced string");
  std::thread t1(routine, 1, std::ref(str));
  t1.join();

  //------------------------------
  /*
   * В поток может быть передана функция-член при условии, что в аргументах первым передается
   * указатель на правильный объект.
   *
   * Такая ситуация возникает, когда копировать объект нельзя потому что впринципе нельзя, либо
   * он слишком большой для копирования.
   */
  SomeClass routiner;
  std::thread t2(&SomeClass::do_some_work, &routiner);
  t2.join();

  //------------------------------
  /*
   * Иногда объекты нужно не копировать и не ссылаться на них, а перемещать, например,
   * перемещать в поток, чтобы последний владел им. В этом случае прибегают к помощи
   * умного указателя unique_ptr.
   *
   * Следующий пример демонстрирует перемещающую семантику.
   */
  std::unique_ptr<SomeClass> ptr(new SomeClass);
  ptr->prepareData("From moved object");
  std::thread t3(process_big_object, std::move(ptr));
  t3.join();

}

//----------------------------------------------------------------

Application* Tools::app_init() {
	return (Application*) new Lesson_2();
}
