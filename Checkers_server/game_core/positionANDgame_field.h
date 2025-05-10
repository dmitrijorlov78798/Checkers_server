#pragma once

#include "includes.h"

/**
* @brief класс реализующий модель позиции на шахматной доске (шаблон "медиатор" для связи игровых классов между собой)
*       (также используется как "итератор" для обхода игрового поля)
*/
class position_t
{
private:
    char c_gorizont; // буквенное обозначение на шахматной доске по горизонтали
    int i_vertical; // числовое обозначение на шахматной доске по вертикали
public:
    /**
    * @brief конструктор по умолчанию
    */
    position_t() : c_gorizont('a'), i_vertical(1)
    {}
    /**
    * @brief конструктор 
    * 
    * @pamar c_gorizont -- числовое обозначение на шахматной доске по вертикали
    * @param i_vertical -- числовое обозначение на шахматной доске по вертикали
    */
    position_t(char c_gorizont, int i_vertical);
    /**
    * @brief метод обновления позиции
    *
    * @pamar c_gorizont -- числовое обозначение на шахматной доске по вертикали
    * @param i_vertical -- числовое обозначение на шахматной доске по вертикали
    * 
    * @return true -- входные параметры валидны(в пределах шахматной доски), позиция обновлена
    *         false -- данные не валидны, позиция осталась прежней
    */
    bool update(char c_gorizont, int i_vertical);
    /**
    * @brief метод обновления позиции по диагонали
    *
    * @pamar direction -- направление проверки, одно из четырех.
    * @param i_steps -- количество клеток, на которые необходимо перейти от текущий позиции
    *
    * @return true -- входные параметры валидны(в пределах шахматной доски), позиция обновлена
    *         false -- данные не валидны, позиция осталась прежней
    */
    bool updateToDiagonal(tipe_direction direction, int i_steps);
    /**
    * @brief метод возврата числового представления горизонтальной координаты
    *
    * @return числовое представление горизонтальной координаты
    */
    int getGorizont() const;
    /**
    * @brief метод возврата числового представления вертикальной координаты
    *
    * @return числовое представление вертикальной координаты
    */
    int getVertical() const;
    /**
    * @brief перегрузка оператора равенства
    */
    bool operator == (const position_t& position) const;
    /**
    * @brief перегрузка оператора неравенства
    */
    bool operator != (const position_t& position) const;
    /**
    * @brief перегрузка оператора меньше
    */
    bool operator < (const position_t& position) const;
    /**
    * @brief перегрузка оператора больше
    */
    bool operator > (const position_t& position) const;
    /**
    * @brief перегрузка оператора инкремента
    */
    position_t& operator++();
    /**
    * @brief перегрузка оператора декримента
    */
    position_t& operator--();
    /**
    * @brief метод получения позиции из строки
    * 
    * @param string -- строка для парсинга
    * 
    * @return true -- позиция распознана
    *         false -- распознать позицию не получилось
    */
    bool fromString(std::string string);
    /**
    * @brief метод перевода позицию в строку
    * 
    * @return строка с позицией
    */
    std::string toString() const;
    /**
    * @brief метод 
    *
    * @param string -- строка для парсинга
    *
    * @return true -- позиция распознана
    *         false -- распознать позицию не получилось
    */
    static std::list<position_t> median(position_t first, position_t second);
    // методы реализующие часть интерфейса итераторов
    static position_t end();
    static position_t begin();
    static position_t rbegin();
    static position_t rend();
    // возвращает, находится ли текущая шашка на нужной горизонтали для превращения в дамку (для черных)
    bool blackQueen() const;
    // возвращает, находится ли текущая шашка на нужной горизонтали для превращения в дамку (для белых)
    bool whiteQueen() const;
    // оператор преобразования позиции в координату на консоли(winapi)
    operator COORD();
};

/**
* @brief класс реализующий модель единичного шага(хода) игрока ("адаптер" связного списка стандартной библиотеки)
*/
class step_t
{
private:
    std::list<position_t> step; // список позиций(2-е при обычном передвижении, 2-е и более при взятии шашек противника )
public:
    step_t()
    {
        step.push_back(position_t());
        step.push_back(position_t());
    }
    step_t(position_t first, position_t second)
    {
        step.push_front(first);
        step.push_back(second);
    }
    ~step_t()
    {
        step.clear();
    }
    position_t first() const;
    position_t second() const;
    bool find(position_t position) const;
    bool empty() const;
    void push(position_t position);
    position_t pop_back();
    position_t pop_front();
    int size() const;
    bool fromString(std::string string);
    std::string toString() const;
    bool operator == (const step_t& rValueStep) const;
};


/**
* @brief класс реализующий модель шахматной доски
*/
class game_field_t
{
private:
    const int W = constants::board::whiteCheck; // код белой клетки
    const int B = constants::board::blackCheck; // код черной клетки
    int massive[8][8] = // двухмерный массив - модель доски
    {
       {W, B, W, B, W, B, W, B},
       {B, W, B, W, B, W, B, W},
       {W, B, W, B, W, B, W, B},
       {B, W, B, W, B, W, B, W},
       {W, B, W, B, W, B, W, B},
       {B, W, B, W, B, W, B, W},
       {W, B, W, B, W, B, W, B},
       {B, W, B, W, B, W, B, W}
    };
    int error; // код ошибки доступа к массиву
public:
    game_field_t() : error(-1)
    {}
    // оператор доступа к ячейке 
    int& operator[](position_t pos);
};
