#pragma once

#include "positionANDgame_field.h"

/**
 * @brief класс реализующий интерфейс с игроком (через консоль)
 */
// TODO реализовать графический интерфейс. 
class graphics_t
{
private:
    HANDLE h_console;// консоль
    COORD h_anchor;// якорная координата на консоли
    int countLog;// счетчик логов
    game_field_t last_frame;// состояние игрового поля на прошлом ходу
    game_field_t mask;//цветовая маска для подсветки ходов ходов
    game_field_t& current_frame;// ссылка на игровое поле (общее для всех)
    // вывод единичного символа
    void draw(int i_pinctogram, COORD cursore, int color);
    // отрисовка поля
    void drawField();
public:
    graphics_t(game_field_t& fldtmp);
    // обновить изображение на консоли (и сделать паузу sec - секунд, для удобства восприятия игрового процесса)
    void updateScreen(int sec);
    // вывести имя победителя
    void winner(bool black);
    // запрос хода от игрока
    void updateStep(step_t& step);
    // обновить цветовую маску
    void updateMask(step_t step, bool stepBlack);
    // запрос типа игры от игрока
    void updateTipegamer(tipe_gamer& gamer);
    // вывод в консоль логов
    void doLog(const char* sz_log);
};
