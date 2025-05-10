// Checkers_server.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <queue>

#include "system_lib/network.h"
#include "system_lib/poolThread.h"

#include "game_core/gameCore.h"
#include "protocols/CheckersMSG.h"

// работа с одним клиентом
class ssesion_t : public network::TCP_socketClient_t, public gamer_t
{

public :
    void makeStep(const std::list<step_t> & possibleSteps, const std::function<void(step_t&& step)>& callback) override
    {
        std::lock_guard<std::mutex> lock(m_mtx_);

        Checkers_MSG_t msg(possibleSteps);
        m_callback_ = callback;

        m_TX_que_.push(std::move(msg));

    }

    void make_request()
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

    void make_answer()
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

protected :
    std::function<void(step_t&& step)>& m_callback_;
    std::mutex m_mtx_;
    std::queue<Checkers_MSG_t> m_TX_que_;
    std::string m_RX_buf_;
};

// игровая сессия между клиентом - клиентом / клиентом - ботом
class game_ssesion_t : public game_checkers_t
{

};

// работа с ботом
class AI_t : /*public sql_t,*/public gamer_t
{

};

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
