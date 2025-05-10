#pragma once

#include "positionANDgame_field.h"

/**
 * @brief класс реализующий модель шашки
 */
class checker_t
{
private:
    game_field_t& game_field; // ссылка на игровое поле
    position_t position; // текущая позиция шашки
    bool b_black; // флаг - шашка черная
    bool b_queen; // флаг - шашка дамка
    
    /**
    * @brief метод - сделать шашку дамкой
    */
    void setQueen(); 
    /**
    * @brief метод проверки коллизи (столкновения)  
    *
    * @param position -- позиция для проверки
    * 
    * @return код объекта занимающего текущую позицию либо 0, если позиция пуста   
    */
    int checkCollision(position_t position) const;
    /**
    * @brief метод проверки и заполнения списка возможных атак данной шашки
    * (применяется алгоритм поиск в глубину)
    *
    * @param l_PossibleStep -- список атак для заполнения
    *
    * @return true -- возможная атака найдена
    *         false -- атаки не найдены
    */
    bool checkAttack(std::list<step_t>& l_PossibleStep) const;
    /**
    * @brief рекурсивный метод поиска возможной атаки (для реализации поиска в глубину)
    *
    * @param PossibleStep -- map возможных ходов для записи (ключ - количество убитых шашек противника)
    * @param step -- шаг предыдущего рекурсивного вызова
    * @param direction -- направление шага предыдущего рекурсивного вызова
    *
    * @return true --  атака для следующего рекурсивного вызова найдена
    *         false -- атаки не найдены, данный рекурсивный вызов последний
    */
    bool checkAttackRecursion(std::map<int, std::list<step_t>>& PossibleStep,
        step_t step, tipe_direction direction) const;
    /**
    * @brief метод - сохранить текущую позицию шашки в игровом поле
    */
    void savePosition();
    /**
    * @brief метод - убить шашку в данной позиции
    * 
    * @param position -- позиция на которой необходимо убить шашку противника
    */
    void killChecker(position_t position) const;
public:
    /**
    * @brief конструктор
    * 
    * @param game_field -- ссылка на игровое поле (общее для всех шашек)
    * @param position -- позиция, на которой создается шашка
    * @param b_black -- если 1, шашка черная
    */
    checker_t(game_field_t& game_field, position_t position, bool b_black);
    /**
    * @brief метод - передвинуть шашку
    *
    * @param position -- позиция на которую необходимо передвинуть шашку
    */
    void moveChecker(step_t step);
    /**
    * @brief метод возврата флага убитой шашки
    *
    * @return true -- шашка убита
    *         false -- шашка жива
    */
    bool getKill() const;
    /**
    * @brief метод проверки текущей шашки позиции 
    *
    * @param position -- позиция для проверик
    * 
    * @return true -- шашка находится на данной позиции
    *         false -- шашки нет на данной позиции
    */
    bool checkPosition(position_t position) const;
    /**
    * @brief метод проверки и заполнения списка возможных ходов данной шашки
    *
    * @param b_checkAttak -- флаг для записи - была ли найдена возможность для атаки
    *
    * @return список возможных ходов
    */
    std::list<step_t> getPossibleStep(bool& b_checkAttak) const;
};