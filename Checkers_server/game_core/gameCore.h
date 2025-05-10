// файл содержит main, класс игроков(временно), и класс самой игры
#include <functional>

#include "graphics.h"
#include "checkers.h"


/**
 * @brief абстрактный класс игрока  
 */
// TODO вынести в отдельный файл
class gamer_t
{
public:
    /**
    * @brief виртуальный метод - сделать ход
    * 
    * @param possibleSteps -- список возможных ходов
    * @param step -- выбранный ход
    */
    virtual void makeStep(const std::list<step_t>& possibleSteps, const std::function<void(step_t&& step)> & callback) = 0;

    /**
    * @brief виртуальный деструктор
    */
    virtual ~gamer_t()
    {}
};
/**
 * @brief класс игрок - человек
 */
 // TODO вынести в отдельный файл
class human_t : public gamer_t
{
private:
    graphics_t& graphics; // ссылка на класс реализующий интерфейс с пользователем
public:
    /**
    * @brief конструктор
    * 
    * @param graphics -- класс интерфейса с полдьзователем (общий для всех)
    */
    human_t(graphics_t& graphics) : graphics(graphics)
    {}
    /**
    * @brief метод - сделать ход
    *
    * @param possibleSteps -- список возможных ходов
    * @param step -- выбранный ход
    */
    void makeStep(const std::list<step_t>& possibleSteps, const std::function<void(step_t&& step)>& callback) override
    {
        while (true) // пока не получим валидный ответ от игрока
        {
            step_t tempStep;
            graphics.updateStep(tempStep);
            for (auto& it : possibleSteps)
                if (tempStep == it) // проверка на валидность
                    callback(std::move(tempStep));
                
        }
    }
};
/**
 * @brief класс игрок - ИИ (пока ии дурак и ходит наугад, но будут реализованы алгоритмы принятия решений)
 */
 // TODO вынести в отдельный файл
class AI_t : public gamer_t
{
private:
    game_field_t& gameField; // класс реализцющий игровое поле
    /**
    * @brief метод - сделать случайный ход
    *
    * @param possibleSteps -- список возможных ходов
    * @param step -- выбранный ход
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
    * @brief конструктор
    *
    * @param field -- класс реализцющий игровое поле (общее для всех)
    */
    AI_t(game_field_t& field) : gameField(field)
    {
        srand(time(NULL));;
    }
    /**
    * @brief метод - сделать ход
    *
    * @param possibleSteps -- список возможных ходов
    * @param step -- выбранный ход
    */
    void makeStep(const std::list<step_t> & possibleSteps, const std::function<void(step_t&& step)>& callback) override
    {// пока ходим наугад
        step_t temp;
        makeRandomStep(possibleSteps, temp);
        callback(std::move(temp));
    }
};

/**
 * @brief класс реализующий основную логику игры
 */
class game_checkers_t
{
protected:
    /**
     * @brief структура содержащая контекст одного игрока
     */
    struct Gamer 
    {
        Gamer() = default;

        std::unique_ptr <gamer_t> ptr_gamer; // указатель на игрока
        step_t step; // ход игрока
        std::list<checker_t> l_checkers; // шашки игрока
        std::list<step_t> l_possible_steps; // возможные ходы игрока
    }   
        white /*белый игрок*/, 
        black/*черный игрок*/, 
        * current/*ссылка на текущего игрока*/;
    
    tipe_gamer gamer; // тип игры
    game_field_t gameField; // игровое поле
    bool end_game, b_stepBlack; // флаги - конец игры, ход черных0
public :
    /**
    * @brief метод запроса хода у игрока
    */
    void request_step();

    /**
    * @brief callback метод ответа с ходом от игрока
    */
    void answer_step(step_t&& step);
private :
    /**
    * @brief метод создания списка возможных ходов
    * 
    * @param possibleStep -- список возможных ходов
    * 
    * @return true -- список возможных шагов создан
    *         false -- нет возможных шагов
    */
    bool makePossibleStep(std::list<step_t>& possibleStep);
    /**
    * @brief метод совершения хода
    * 
    * @param l_checker -- список шашек
    * @param step -- шаг, который нужно выполнить
    */
    inline void steping(std::list<checker_t >& l_checker, step_t step)
    {// идем по списку, если позиция шашки и в шаге совпадают, ходим
        for (auto& it : l_checker) if (it.checkPosition(step.first())) it.moveChecker(step);
    }
    /**
    * @brief метод удаления взятой шашки
    *
    * @param l_checker -- список шашек
    */
    void deleteChecker(std::list<checker_t>& l_checker);
public:
    /**
    * @brief конструктор 
    */
    game_checkers_t();

    /**
    * @brief деструктор
    */
    ~game_checkers_t()
    {}
};
