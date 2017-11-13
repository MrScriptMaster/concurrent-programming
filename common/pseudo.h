
#ifndef PSEUDO_H_INCLUDED
#define PSEUDO_H_INCLUDED

/**
 * Для имитации работы. 
 * Поток засыпает в случайные моменты времени на время от 10 до 1000 миллисекунд.
 * 
 * @param c - символ, который печатает поток, когда просыпается
 * @param times - количество повторений
 */
int random_sleep(char c, unsigned int times);

#endif
