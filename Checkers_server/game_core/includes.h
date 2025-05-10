// Содержит подключаемые заголовочные файлы,
// энамы и глобальные переменные для работы
// всего проекта.

#pragma once

#ifndef INCLUDES
#define INCLUDES

// Подключаемые заголовочные файлы

#include <windows.h>
#include <list>
#include <map>
#include <string>
#include <iostream>

// Энамы

/**
 * @brief тип игроков
 *
 * @param HvsH -- игрок против игрока
 * @param HvsAI -- игрок против компьютера
 * @param AIvsAI -- компьютер против компьютера
 */
enum class tipe_gamer
{
    HvsH, // игрок против игрока
    HvsAI, // игрок против компьютера
    AIvsAI // компьютер против компьютера
};

/**
 * @brief тип направления перемещения шашки
 *
 * @param q -- левый верхний угол относительно шашки
 * @param e -- правый верхний угол относительно шашки
 * @param z -- левый нижний угол относительно шашки
 * @param c -- правый нижний угол относительно шашки
 * @param end -- последний элемент,необходим для итераций
 */
enum class tipe_direction
{
    q, // левый верхний угол относительно шашки 
    e, // правый верхний угол относительно шашки
    z, // левый нижний угол относительно шашки
    c, // правый нижний угол относительно шашки
    end // последний элемент,необходим для итераций

};

/**
 * @brief оператор инкрементирования направления перемещения шашки
 *
 * @param direction -- исходный объект
 *
 * @return обЪект после инкремента
 */
inline tipe_direction& operator++ (tipe_direction& direction)
{
    if (direction != tipe_direction::end) // если мы не в конце списка направление
        direction = (tipe_direction)((int)direction + 1); // то через преобразования типов увеличиваем на единицу текущее значение
    return direction;
}

/**
 * @brief оператор инвертирования направления
 *
 * @param direction -- исходный объект
 *
 * @return обЪект после инвертирования
*/
inline tipe_direction operator! (tipe_direction direction)
{
    switch (direction) // направление меняем на противоположное
    { // для ориентирования можно взглянуть на клавиатуру и соответствующие клавишы
    case tipe_direction::q:
        direction = tipe_direction::c;
        break;
    case tipe_direction::e:
        direction = tipe_direction::z;
        break;
    case tipe_direction::z:
        direction = tipe_direction::e;
        break;
    case tipe_direction::c:
        direction = tipe_direction::q;
        break;
    default:
        break;
    }
    return direction;
}

inline char int2char(int value)
{
    return (char)(value + (int)'0');
}
inline int char2int(char value)
{
    return ((int)value - (int)'0');
}


// Глобальные константы

/**
 * @brief класс обертка для констант
 *
 */
class constants
{
public:
    struct checker
    {
        static const int white = 1;
        static const int black = 2;
        static const int whiteQueen = 3;
        static const int blackQueen = 4;
        static const int killed = 5;
    };
    struct board
    {
        static const int whiteCheck = -1;
        static const int blackCheck = 0;
        static const int minVertical = 1;
        static const int maxVertical = 8;
        static const char minGorizontal = 'a';
        static const char maxGorizontal = 'h';
    };
    struct mask
    {
        static const int null = 0;
        static const int white = 10;
        static const int black = 20;
    };
    struct color
    {
        static const int white = 15;
        static const int yellow = 14;
        static const int green = 2;
        static const int blue = 3;
        static const int red = 4;
        static const int orange = 6;
    };
};


#endif // INCLUDES