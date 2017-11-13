
#include "app.h"
#include "pseudo.h"
#include <iostream>

#include <thread>
 
using namespace Tools;

class Lesson_2 : public Application {
public:
	void start() const;
};

//----------------------------------------------------------------

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
}

//----------------------------------------------------------------

Application* Tools::app_init() {
	return (Application*) new Lesson_2();
}
