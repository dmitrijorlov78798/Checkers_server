#include "positionANDgame_field.h"

position_t::position_t(char c_gorizont, int i_vertical)
{
    if (c_gorizont < constants::board::minGorizontal // проверка валидности параметров
        || c_gorizont > constants::board::maxGorizontal
        || i_vertical < constants::board::minVertical
        || i_vertical > constants::board::maxVertical)
        position_t();
    else
    {
        this->i_vertical = i_vertical;
        this->c_gorizont = c_gorizont;
    }
}

bool position_t::update(char c_gorizont, int i_vertical)
{
    if (c_gorizont < constants::board::minGorizontal // проверка валидности параметров
        || c_gorizont > constants::board::maxGorizontal
        || i_vertical < constants::board::minVertical
        || i_vertical > constants::board::maxVertical)
        return false;
    else
    {
        this->i_vertical = i_vertical;
        this->c_gorizont = c_gorizont;
        return true;
    }
}

bool position_t::updateToDiagonal(tipe_direction direction, int i_steps)
{
    bool b_result = false;

    switch (direction)
    {// в завасимости от направления, двигаемся в соседнии клетки на i_steps шагов
    case tipe_direction::q: 
        if (update(this->c_gorizont - i_steps, this->i_vertical + i_steps))
            b_result = true;
        break;
    case tipe_direction::e:
        if (update(this->c_gorizont + i_steps, this->i_vertical + i_steps))
            b_result = true;
        break;
    case tipe_direction::z:
        if (update(this->c_gorizont - i_steps, this->i_vertical - i_steps))
            b_result = true;
        break;
    case tipe_direction::c:
        if (update(this->c_gorizont + i_steps, this->i_vertical - i_steps))
            b_result = true;
        break;
    default:
        b_result = false;
        break;
    }

    return b_result;
}

int position_t::getGorizont() const
{
    return ((int)c_gorizont - (int)constants::board::minGorizontal);
}

int position_t::getVertical() const
{// инвертирование для коордиант консоли
    return (8 - i_vertical);
}

bool position_t::operator == (const position_t& position) const
{
    return ((c_gorizont == position.c_gorizont) && (i_vertical == position.i_vertical));
}

bool position_t::operator!=(const position_t& position) const
{
    return !(*this == position);
}

bool position_t::operator < (const position_t& position) const
{
    if (i_vertical != position.i_vertical) return i_vertical < position.i_vertical;
    else return c_gorizont < position.c_gorizont;
}

bool position_t::operator>(const position_t& position) const
{
    if (i_vertical != position.i_vertical) return i_vertical > position.i_vertical;
    else return c_gorizont > position.c_gorizont;
}

position_t& position_t::operator++()
{
    if (c_gorizont == constants::board::maxGorizontal)
    {
        c_gorizont = constants::board::minGorizontal;
        ++i_vertical;
    }
    else ++c_gorizont;
    return *this;
}

position_t& position_t::operator--()
{
    if (c_gorizont == constants::board::minGorizontal)
    {
        c_gorizont = constants::board::maxGorizontal;
        --i_vertical;
    }
    else --c_gorizont;
    return *this;
}

bool position_t::fromString(std::string tempString)
{
    return tempString.size() == 2 ? update(tempString.front(), char2int(tempString.back())) : false;
}

std::string position_t::toString() const
{
    std::string string;
    string += c_gorizont;
    string += int2char(i_vertical);
    return std::string();
}

std::list<position_t> position_t::median(position_t first, position_t second)
{
    std::list<position_t> l_position;
    // для всех направлений
    for (tipe_direction it = tipe_direction::q; it != tipe_direction::end; ++it)
    {
        position_t temp = first;
        while (temp.updateToDiagonal(it, 1)) // пока мы не у края доски
            if (temp != second) // если не нашли вторую позицию
                l_position.push_back(temp); // собираем позиции в лист
            else
                return l_position; // если нашли что искали, возвращаем лист позиций между первой и второй позицией

        l_position.clear(); 
    }

    return l_position;
}

position_t position_t::begin()
{
    return position_t();
}

position_t position_t::end()
{
    position_t position(constants::board::minGorizontal,
        constants::board::maxVertical);
    ++position.i_vertical;
    return position;
}

position_t position_t::rbegin()
{
    position_t position(constants::board::maxGorizontal,
        constants::board::maxVertical);
    return position;
}

position_t position_t::rend()
{
    position_t position(constants::board::maxGorizontal,
        constants::board::minVertical);
    --position.i_vertical;
    return position;
}

bool position_t::blackQueen() const
{
    return i_vertical == constants::board::minVertical;
}

bool position_t::whiteQueen() const
{
    return i_vertical == constants::board::maxVertical;
}

position_t::operator COORD()
{
    COORD cursore;
    cursore.Y = getVertical();
    cursore.X = getGorizont();
    return cursore;
}

position_t step_t::first() const
{
    return step.front();
}

position_t step_t::second() const
{
    return step.back();
}

bool step_t::find(position_t position) const
{
    std::list<position_t>::const_iterator iter = step.cbegin();
    for (; iter != step.cend(); ++iter)
        if (*iter == position)
            return true;
    return false;
}

bool step_t::empty() const
{
    return step.empty();
}

void step_t::push(position_t position)
{
    step.push_back(position);
}

position_t step_t::pop_back()
{
    position_t temp = temp.end();
    if (step.size() > 0)
    {
        temp = step.back();
        step.pop_back();
    }
    return temp;
}

position_t step_t::pop_front()
{
    position_t temp = temp.end();
    if (step.size() > 0)
    {
        temp = step.front();
        step.pop_front();
    }
    return temp;
}

int step_t::size() const
{
    return step.size();
}

bool step_t::fromString(std::string string)
{
    bool b_result = false;
    size_t size = string.size();
    if (size > 4 && size % 3 == 2) // проверяем валидность строки
    {
        b_result = true;
        std::string s_temp;
        position_t pos_temp;
        step.clear();
        for (size_t i = 0; i < size; ++i)
            switch (i % 3)
            {
            case 0:
                s_temp = string[i];
                break;
            case 1:
                s_temp += string[i];
                if (b_result &= pos_temp.fromString(s_temp)) step.push_back(pos_temp);
                break;
            case 2:
                s_temp.clear();
                break;
            default:
                break;
            }
    }
    return b_result;
}

std::string step_t::toString() const
{
    std::string s_temp;;
    std::list<position_t>::const_iterator it = step.cbegin();
    for (; it != step.cend(); ++it)
    {
        s_temp += it->toString();
    }
    return s_temp;
}

bool step_t::operator == (const step_t& rValueStep) const
{
    bool b_result = false;
    if (step.size() == rValueStep.step.size())
    {
        b_result = true;
        std::list<position_t>::const_iterator it = step.cbegin();
        std::list<position_t>::const_iterator itrValueStep = rValueStep.step.cbegin();
        for (; (it != step.cend()) && (itrValueStep != rValueStep.step.cend()); ++it, ++itrValueStep)
            b_result &= (*it == *itrValueStep);
    }
    return b_result;
}


int& game_field_t::operator[](position_t pos)
{
    error = -1;
    int y = pos.getVertical();
    int x = pos.getGorizont();
    if (y > -1 && y < 8 && x > -1 && x < 8) return massive[y][x];
    else return error;
}