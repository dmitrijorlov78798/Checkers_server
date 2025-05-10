// checkers.cpp

#include "checkers.h"

checker_t::checker_t(game_field_t& game_field, position_t position, bool b_black)
    : game_field(game_field), position(position), b_black(b_black), b_queen(false)
{
    savePosition();
}

void checker_t::setQueen()
{
    if (!b_queen) // если флаг еще не установлен
        if (b_black && position.blackQueen() // мы черные и позиция для создания черной дамки (первая горизонталь шахматной доски)
            || !b_black && position.whiteQueen()) // или то же самое, только для белой
            b_queen = true;
}

int checker_t::checkCollision(position_t position) const
{
    return game_field[position];
}

bool checker_t::checkAttack(std::list<step_t>& l_PossibleStep) const
{
    std::map<int, std::list<step_t>> m_tempPossibleStep; // временный контейнер списков атак, отсортированных по кол-ву взятых шашек противника
    step_t step(position, position_t::end()); // временный шаг для рекурсивных вызовов

    if (checkAttackRecursion(m_tempPossibleStep, step, tipe_direction::end)) // если в рекурсивных проверках были найдены атаки
    {// выбираем список возможных атак с максимальным количеством взятых шашек
        std::map<int, std::list<step_t>>::reverse_iterator iter = m_tempPossibleStep.rbegin();
        l_PossibleStep = iter->second;
        return true;
    }

    return false;
}

bool checker_t::checkAttackRecursion(std::map<int, std::list<step_t>>& m_PossibleStep, step_t step,
    tipe_direction direction) const
{
    bool b_result = false;

    if (step.size() < 13) // кол-во рекурсий не может превысить колво шашек противника
    {
        if (step.second() == position.end()) step.pop_back(); // если первый шаг, убираем последнюю пустую позицию 

        // для 4-х направлений
        for (tipe_direction it_direction = tipe_direction::q; it_direction != tipe_direction::end; ++it_direction)
        {
            if (it_direction == direction) // если направление то от куда мы походили пропуск цикла
                continue;
            position_t tempPosition = step.second(); // создания временной позиции и шага
            step_t tempStep = step;
            if (b_queen) // если мы дамка
            {
                bool b_resultMove = false;
                // пока можем, подходим максимально близко к потенциальной шашке противника
                while (tempPosition.updateToDiagonal(it_direction, 1)) // пока не дошли к краю доски
                {
                    b_resultMove = true;
                    if (checkCollision(tempPosition) != constants::board::blackCheck) // если текущая позиция уже занята
                        break;
                }
                // если мы сдивинулись с места, и вышли из предыдущего цикла по коллизии, необходимо вернуть позицию на шаг назад
                if (b_resultMove) tempPosition.updateToDiagonal(!it_direction, 1);
            }

            if (tempPosition.updateToDiagonal(it_direction, 1) // если мы не у края доски
                && (checkCollision(tempPosition) ==
                    (b_black ? constants::checker::white : constants::checker::black)
                    || checkCollision(tempPosition) ==
                    (b_black ? constants::checker::whiteQueen : constants::checker::blackQueen))) // и нашли шашку противника
                if (tempPosition.updateToDiagonal(it_direction, 1) && checkCollision(tempPosition) == 0) // и за ней пусто
                    if (!tempStep.find(tempPosition)) // и текущий ход еще не найден (защита от замкнутого цикла)
                    {
                        b_result = true; // мы нашли ход для атаки
                        bool b_firstIteration = true;
                        // ищем следующую возможную цель 
                        do 
                        { 
                            if (!b_firstIteration) // если цикл не первый (мы дамка) обновляем последнюю позицию в шаге 
                                tempStep.pop_back();
                            tempStep.push(tempPosition);
                            if (!checkAttackRecursion(m_PossibleStep, tempStep, !it_direction)) // проверяем, можем ли еще убить шашки
                                m_PossibleStep[tempStep.size()].push_back(tempStep); // если это была последняя, сохраняем результат
                            b_firstIteration = false;
                        } while (tempPosition.updateToDiagonal(it_direction, 1) // также если мы дамка, необходимо проверить все диагонали
                            && checkCollision(tempPosition) == 0 && b_queen);
                    }
        }
    }
    return b_result;
}

void checker_t::savePosition()
{
    setQueen(); // позиция поменялась, не стали ли мы дамкой
    // в зависимости от того кто мы, сохраняем свой код объекта на игровом поле
    if (b_queen) game_field[position] = b_black ?
        constants::checker::blackQueen : constants::checker::whiteQueen;
    else game_field[position] = b_black ?
        constants::checker::black : constants::checker::white;
    //TO DO добавить обновления рисунка при оконном режиме
}

void checker_t::killChecker(position_t position) const
{
    // проверяем, можем ли мы убить шашку противника и убиваем её
    if ((game_field[position] == (b_black ?
        constants::checker::white : constants::checker::black)
        || game_field[position] == (b_black ?
            constants::checker::whiteQueen : constants::checker::blackQueen)))
        game_field[position] = constants::checker::killed;
}

void checker_t::moveChecker(step_t step)
{
    // если шаг валидный
    if (step.first() == position && step.size() > 1)
        while (!step.empty()) // пока он не пуст
        {
            position_t first = step.first() == position ? step.pop_front() : position; // берем первую позицию
            position_t second = step.pop_front(); // берем вторую позицию
            game_field[first] = constants::board::blackCheck; // зануляем первую позицию на игровом поле
            position = second; // сохраняем новое значение
            savePosition();
            std::list<position_t> l_position = position_t::median(first, second); // создаем список позиций в которых нужно убить шашки противника
            std::list<position_t>::iterator iter = l_position.begin();
            for (; iter != l_position.end(); ++iter) // убиваем шашки противника
                killChecker(*iter);
        }

}

bool checker_t::getKill() const
{
    bool b_result = false;
    // если в данной позиции есть убитая шашка
    if (game_field[position] == constants::checker::killed)
    {
        b_result = true;
        game_field[position] = constants::board::blackCheck; // зануляем позицию
    }

    return b_result;
}

bool checker_t::checkPosition(position_t position) const
{
    return (this->position == position);
}

std::list<step_t> checker_t::getPossibleStep(bool& b_checkAttack) const
{
    std::list<step_t> l_tempPossibleStep;
    // проверка, есть ли возможная атка
    if (checkAttack(l_tempPossibleStep))
        b_checkAttack = true;
    else // если атаки нет, для всех направлений, проверяем, можем ли мы походить
        for (tipe_direction it_direction = tipe_direction::q; it_direction != tipe_direction::end; ++it_direction)
        {
            position_t tempPosition = position;
            int stepUpdate = 0;
            while (tempPosition.updateToDiagonal(it_direction, ++stepUpdate) // пока мы не у края доски
                && checkCollision(tempPosition) == constants::board::blackCheck // и позиция не занята
                && ((b_black ? it_direction > tipe_direction::e : it_direction < tipe_direction::z) || b_queen)) // и ходим только по правилам
            {
                l_tempPossibleStep.push_back(step_t(position, tempPosition)); // добавляем новый возможный шаг
                if (!b_queen) break; // если мы не дамка, наш путь закончен
                else tempPosition = position; // иначе, пробуем идти дальше
            }
        }

    return l_tempPossibleStep;
}



