#include "gameCore.h"
#include "../Checkers_server.h" 

/**
* @brief ����� ������� ���� � ������
*/
void game_checkers_t::request_step()
{
    if (
        not (white.l_checkers.empty())
        && not (black.l_checkers.empty())
        && makePossibleStep(current->l_possible_steps))
    { // ���������, �������� �� � ������� ����� � ���� �� � ��� ��������� ����
        current->ptr_gamer->makeStep(current->l_possible_steps,
            [this](step_t&& step) { this->answer_step(std::move(step)); });
    }
    else
    {
        end_game = true; // ����� ����
        //graphics.winner(!b_stepBlack); // ������� ����������
    }
}

/**
 * @brief callback ����� ������ � ����� �� ������
 */
void game_checkers_t::answer_step(step_t&& step)
{
    steping(current->l_checkers, current->step);
    deleteChecker(current->l_checkers);
    //graphics.updateMask(current->step, b_stepBlack); // ���������� ����������
    //graphics.updateScreen(3);
    // �������� ��� ������� ������
    b_stepBlack = not (b_stepBlack);
    current = b_stepBlack ? &black : &white;
    // TODO 
}

/**
 * @brief ����� �������� ������ ��������� �����
 *
 * @param possibleStep -- ������ ��������� �����
 *
 * @return true -- ������ ��������� ����� ������
 *         false -- ��� ��������� �����
 */
bool game_checkers_t::makePossibleStep(std::list<step_t>& possibleStep)
{
    int sizeStep = 0; // ������ ����
    bool attack = false; // ���� ����������� �����
    std::list<step_t> l_possibleStep_temp;
    std::list<checker_t> l_chekers_temp;
    possibleStep.clear(); // ���������� ������ ��������� �����
    for (auto& it : current->l_checkers)
    { // ��� ������ ����� �� ������ ���� ����� ������
        bool checkAttack = false; // ���� �������� �� ����� 
        l_possibleStep_temp = it.getPossibleStep(checkAttack); // ���������� ��������� ���� ������ ����� 
        if (!attack && checkAttack) // ���� ������� ���������� ��� � ������
        {// ���������� ������ ��������
            possibleStep.clear();
            attack |= checkAttack;
        }
        if (l_possibleStep_temp.empty())
            continue; // ���� ������ ����, ������� �����
        if (attack && !checkAttack)
            continue; // ���� ��� ������������ ���� � ������, � � ������ ����� ������� ����� �� ���, ������� �����
        if (l_possibleStep_temp.front().size() < sizeStep)
            continue; // ���� � ���������� ������ �����, ���-�� ������ ����� ���������� ������ ��� � �������, ������� �����
        else if (l_possibleStep_temp.front().size() > sizeStep)
        { // �����, ���� ���-�� ������ ����� ���������� � ������� ������ ������, �������� ������� ������
            sizeStep = l_possibleStep_temp.front().size();
            possibleStep.clear();
        }
        while (!l_possibleStep_temp.empty())
        { // ���� ��� �������� ������, ���������� ������ ������� ����� � ����� ������ ��������� �����
            possibleStep.push_front(l_possibleStep_temp.front());
            l_possibleStep_temp.pop_front();
        }
    }
    return !possibleStep.empty(); // ���� ���� ��� �� ��������, �� ������ true
}


/**
* @brief ����� �������� ������ �����
*
* @param l_checker -- ������ �����
*/
void game_checkers_t::deleteChecker(std::list<checker_t>& l_checker)
{// ���� �� ������ �����
    std::list<checker_t>::iterator l_iter = l_checker.begin();
    for (; l_iter != l_checker.end();)
        if (l_iter->getKill()) // ���� ����� �������� ��� ��������(�����)
            l_iter = l_checker.erase(l_iter); // �������
        else ++l_iter;

}


/**
* @brief �����������
*/
game_checkers_t::game_checkers_t() :
    white(), 
    black(), 
    current(nullptr), 
    /*graphics(gameField),*/ 
    end_game(false), 
    b_stepBlack(false)
{
    //graphics.updateTipegamer(gamer); // ������ ���� ���� 
    switch (gamer) // � ����������� �� ���� ���� ������� ��������� ���������� ������������ ������
    {
    case tipe_gamer::HvsH: // ������� ������ ��������
        white.ptr_gamer = std::make_unique <ssesion_t>();
        black.ptr_gamer = std::make_unique <ssesion_t>();
        break;
    case tipe_gamer::HvsAI: // ������� ������ ��
        white.ptr_gamer = std::make_unique <ssesion_t>();
        black.ptr_gamer = std::make_unique <AI_t>(gameField);
        break;
    case tipe_gamer::AIvsAI: // �� ������ ��
        white.ptr_gamer = std::make_unique <AI_t>(gameField);
        black.ptr_gamer = std::make_unique <AI_t>(gameField);
        break;
    default:
        break;

        current = &white;
    }
    size_t count = 12; // ������� ����� �����, 12 ����
    for (position_t it = position_t::begin(); it != position_t::end() && count > 0; ++it)
        if (gameField[it] == 0)
        {
            white.l_checkers.push_front(checker_t(gameField, it, false));
            --count;
        }

    count = 12; // ������� ������ �����, 12 ����
    for (position_t it = position_t::rbegin(); it != position_t::rend() && count > 0; --it)
        if (gameField[it] == 0)
        {
            black.l_checkers.push_front(checker_t(gameField, it, true));
            --count;
        }
    //graphics.updateScreen(0); // ��������� ���������
}