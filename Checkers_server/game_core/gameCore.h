// ���� �������� main, ����� �������(��������), � ����� ����� ����
#include <functional>

#include "graphics.h"
#include "checkers.h"


/**
 * @brief ����������� ����� ������  
 */
// TODO ������� � ��������� ����
class gamer_t
{
public:
    /**
    * @brief ����������� ����� - ������� ���
    * 
    * @param possibleSteps -- ������ ��������� �����
    * @param step -- ��������� ���
    */
    virtual void makeStep(const std::list<step_t>& possibleSteps, const std::function<void(step_t&& step)> & callback) = 0;

    /**
    * @brief ����������� ����������
    */
    virtual ~gamer_t()
    {}
};
/**
 * @brief ����� ����� - �������
 */
 // TODO ������� � ��������� ����
class human_t : public gamer_t
{
private:
    graphics_t& graphics; // ������ �� ����� ����������� ��������� � �������������
public:
    /**
    * @brief �����������
    * 
    * @param graphics -- ����� ���������� � �������������� (����� ��� ����)
    */
    human_t(graphics_t& graphics) : graphics(graphics)
    {}
    /**
    * @brief ����� - ������� ���
    *
    * @param possibleSteps -- ������ ��������� �����
    * @param step -- ��������� ���
    */
    void makeStep(const std::list<step_t>& possibleSteps, const std::function<void(step_t&& step)>& callback) override
    {
        while (true) // ���� �� ������� �������� ����� �� ������
        {
            step_t tempStep;
            graphics.updateStep(tempStep);
            for (auto& it : possibleSteps)
                if (tempStep == it) // �������� �� ����������
                    callback(std::move(tempStep));
                
        }
    }
};
/**
 * @brief ����� ����� - �� (���� �� ����� � ����� ������, �� ����� ����������� ��������� �������� �������)
 */
 // TODO ������� � ��������� ����
class AI_t : public gamer_t
{
private:
    game_field_t& gameField; // ����� ����������� ������� ����
    /**
    * @brief ����� - ������� ��������� ���
    *
    * @param possibleSteps -- ������ ��������� �����
    * @param step -- ��������� ���
    */
    void makeRandomStep(const std::list<step_t> & possibleSteps, step_t& step)
    {
        step = possibleSteps.front();
        size_t i = 0, randomValue = rand() % possibleSteps.size() - 1;
        for (auto& it : possibleSteps)
            if (randomValue == i++) step = it;
    }
public:
    /**
    * @brief �����������
    *
    * @param field -- ����� ����������� ������� ���� (����� ��� ����)
    */
    AI_t(game_field_t& field) : gameField(field)
    {
        srand(time(NULL));;
    }
    /**
    * @brief ����� - ������� ���
    *
    * @param possibleSteps -- ������ ��������� �����
    * @param step -- ��������� ���
    */
    void makeStep(const std::list<step_t> & possibleSteps, const std::function<void(step_t&& step)>& callback) override
    {// ���� ����� ������
        step_t temp;
        makeRandomStep(possibleSteps, temp);
        callback(std::move(temp));
    }
};

/**
 * @brief ����� ����������� �������� ������ ����
 */
class game_checkers_t
{
protected:
    /**
     * @brief ��������� ���������� �������� ������ ������
     */
    struct Gamer 
    {
        Gamer() = default;

        std::unique_ptr <gamer_t> ptr_gamer; // ��������� �� ������
        step_t step; // ��� ������
        std::list<checker_t> l_checkers; // ����� ������
        std::list<step_t> l_possible_steps; // ��������� ���� ������
    }   
        white /*����� �����*/, 
        black/*������ �����*/, 
        * current/*������ �� �������� ������*/;
    
    tipe_gamer gamer; // ��� ����
    game_field_t gameField; // ������� ����
    bool end_game, b_stepBlack; // ����� - ����� ����, ��� ������0
public :
    /**
    * @brief ����� ������� ���� � ������
    */
    void request_step();

    /**
    * @brief callback ����� ������ � ����� �� ������
    */
    void answer_step(step_t&& step);
private :
    /**
    * @brief ����� �������� ������ ��������� �����
    * 
    * @param possibleStep -- ������ ��������� �����
    * 
    * @return true -- ������ ��������� ����� ������
    *         false -- ��� ��������� �����
    */
    bool makePossibleStep(std::list<step_t>& possibleStep);
    /**
    * @brief ����� ���������� ����
    * 
    * @param l_checker -- ������ �����
    * @param step -- ���, ������� ����� ���������
    */
    inline void steping(std::list<checker_t >& l_checker, step_t step)
    {// ���� �� ������, ���� ������� ����� � � ���� ���������, �����
        for (auto& it : l_checker) if (it.checkPosition(step.first())) it.moveChecker(step);
    }
    /**
    * @brief ����� �������� ������ �����
    *
    * @param l_checker -- ������ �����
    */
    void deleteChecker(std::list<checker_t>& l_checker);
public:
    /**
    * @brief ����������� 
    */
    game_checkers_t();

    /**
    * @brief ����������
    */
    ~game_checkers_t()
    {}
};
