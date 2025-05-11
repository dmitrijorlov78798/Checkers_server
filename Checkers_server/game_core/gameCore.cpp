#include "gameCore.h"
#include "../Checkers_server.h" 

/**
* @brief метод запроса хода у игрока
*/
void game_checkers_t::request_step()
{
    if (
        not (white.l_checkers.empty())
        && not (black.l_checkers.empty())
        && makePossibleStep(current->l_possible_steps))
    { // проверяем, остались ли у игроков шашки и есть ли у них доступные шаги
        current->ptr_gamer->makeStep(current->l_possible_steps,
            [this](step_t&& step) { this->answer_step(std::move(step)); });
    }
    else
    {
        end_game = true; // конец игры
        //graphics.winner(!b_stepBlack); // выводим победителя
    }
}

/**
 * @brief callback метод ответа с ходом от игрока
 */
void game_checkers_t::answer_step(step_t&& step)
{
    steping(current->l_checkers, current->step);
    deleteChecker(current->l_checkers);
    //graphics.updateMask(current->step, b_stepBlack); // обновления интерфейса
    //graphics.updateScreen(3);
    // передаем ход другому игроку
    b_stepBlack = not (b_stepBlack);
    current = b_stepBlack ? &black : &white;
    // TODO 
}

/**
 * @brief метод создания списка возможных ходов
 *
 * @param possibleStep -- список возможных ходов
 *
 * @return true -- список возможных шагов создан
 *         false -- нет возможных шагов
 */
bool game_checkers_t::makePossibleStep(std::list<step_t>& possibleStep)
{
    int sizeStep = 0; // размер шага
    bool attack = false; // флаг возможности атаки
    std::list<step_t> l_possibleStep_temp;
    std::list<checker_t> l_chekers_temp;
    possibleStep.clear(); // подготовка списка возможных шагов
    for (auto& it : current->l_checkers)
    { // для каждой шашки из списка всех шашок игрока
        bool checkAttack = false; // флаг проверки на атаку 
        l_possibleStep_temp = it.getPossibleStep(checkAttack); // возвращаем возможные ходы данной шашки 
        if (!attack && checkAttack) // если впервые встретился шаг с атакой
        {// предыдущий список затираем
            possibleStep.clear();
            attack |= checkAttack;
        }
        if (l_possibleStep_temp.empty())
            continue; // если список пуст, пропуск цикла
        if (attack && !checkAttack)
            continue; // если уже присутствуют шаги с атакой, а в списке шагов текущей шашки из нет, пропуск цикла
        if (l_possibleStep_temp.front().size() < sizeStep)
            continue; // если в предыдущем списке шагов, кол-во взятых шашек противника больше чем в текущем, пропуск цикла
        else if (l_possibleStep_temp.front().size() > sizeStep)
        { // иначе, если кол-во взятых шашек противника в текущем списке больше, затираем прошлый список
            sizeStep = l_possibleStep_temp.front().size();
            possibleStep.clear();
        }
        while (!l_possibleStep_temp.empty())
        { // если все проверки прошли, записываем список текущей шашки в общий список возможных ходов
            possibleStep.push_front(l_possibleStep_temp.front());
            l_possibleStep_temp.pop_front();
        }
    }
    return !possibleStep.empty(); // если хоть что то записали, то вернем true
}


/**
* @brief метод удаления взятой шашки
*
* @param l_checker -- список шашек
*/
void game_checkers_t::deleteChecker(std::list<checker_t>& l_checker)
{// идем по списку шашек
    std::list<checker_t>::iterator l_iter = l_checker.begin();
    for (; l_iter != l_checker.end();)
        if (l_iter->getKill()) // если шашка помечена как съеденая(убита)
            l_iter = l_checker.erase(l_iter); // удаляем
        else ++l_iter;

}


/**
* @brief конструктор
*/
game_checkers_t::game_checkers_t() :
    white(), 
    black(), 
    current(nullptr), 
    /*graphics(gameField),*/ 
    end_game(false), 
    b_stepBlack(false)
{
    //graphics.updateTipegamer(gamer); // запрос типа игры 
    switch (gamer) // в зависимотси от типа игры создаем различные реализации абстрактного игрока
    {
    case tipe_gamer::HvsH: // человек против человека
        white.ptr_gamer = std::make_unique <ssesion_t>();
        black.ptr_gamer = std::make_unique <ssesion_t>();
        break;
    case tipe_gamer::HvsAI: // человек против ИИ
        white.ptr_gamer = std::make_unique <ssesion_t>();
        black.ptr_gamer = std::make_unique <AI_t>(gameField);
        break;
    case tipe_gamer::AIvsAI: // ИИ против ИИ
        white.ptr_gamer = std::make_unique <AI_t>(gameField);
        black.ptr_gamer = std::make_unique <AI_t>(gameField);
        break;
    default:
        break;

        current = &white;
    }
    size_t count = 12; // создаем белые шашки, 12 штук
    for (position_t it = position_t::begin(); it != position_t::end() && count > 0; ++it)
        if (gameField[it] == 0)
        {
            white.l_checkers.push_front(checker_t(gameField, it, false));
            --count;
        }

    count = 12; // создаем черные шашки, 12 штук
    for (position_t it = position_t::rbegin(); it != position_t::rend() && count > 0; --it)
        if (gameField[it] == 0)
        {
            black.l_checkers.push_front(checker_t(gameField, it, true));
            --count;
        }
    //graphics.updateScreen(0); // обновляем интерфейс
}