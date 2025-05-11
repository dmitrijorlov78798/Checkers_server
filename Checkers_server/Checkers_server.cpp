// Checkers_server.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "Checkers_server.h"

void ssesion_t::makeStep(const std::list<step_t> & possibleSteps, const std::function<void(step_t&& step)>& callback) 
{
    std::lock_guard<std::mutex> lock(m_mtx_);

    Checkers_MSG_t msg(possibleSteps);
    m_callback_ = callback;

    m_TX_que_.push(std::move(msg));

}

void ssesion_t::make_request()
{
    std::lock_guard<std::mutex> lock(m_mtx_);

    std::string buffer;
    m_TX_que_.front().serialize(buffer);

    if (int result = Send(buffer, m_TX_que_.front().get_offset()) == 0)
        m_TX_que_.pop();
    else if (result > 0)
        m_TX_que_.front().set_offset(result);
    else if (result != -3)
        throw std::runtime_error("ssesion_t::make_step() error Send()");
}

void ssesion_t::make_answer()
{
    std::lock_guard<std::mutex> lock(m_mtx_);

    std::string buffer;
            
    if (int result = Recive(buffer, Checkers_MSG_t::EOM) == 0)
    {
        m_RX_buf_.append(buffer);

        Checkers_MSG_t msg(m_RX_buf_);

        switch (msg.get_type())
        {
        case typeMsg::answerStep:
        {
            if (auto option = msg.get_step())
                m_callback_(std::move(option.value()));
            else
                throw std::runtime_error("ssesion_t::make_answer() error step_t deser");
            break;
        }
        default:
            throw std::runtime_error("ssesion_t::make_answer() invalid type msg");
            break;
        }

        m_RX_buf_.clear();
    }
    else if (result > 0)
        m_RX_buf_.append(buffer);
    else if (result != -3)
        throw std::runtime_error("ssesion_t::make_answer() error Recive()");
}



int main()
{
    
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
