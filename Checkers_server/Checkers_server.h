#pragma once

#include <queue>

#include "system_lib/network.h"
#include "system_lib/poolThread.h"

#include "game_core/gameCore.h"
#include "protocols/CheckersMSG.h"


// ������ � ����� ��������
class ssesion_t : public network::TCP_socketClient_t, public gamer_t
{

public:
    void makeStep(const std::list<step_t>& possibleSteps, const std::function<void(step_t&& step)>& callback) override;

    void make_request();

    void make_answer();

protected:
    std::function<void(step_t&& step)>& m_callback_;
    std::mutex m_mtx_;
    std::queue<Checkers_MSG_t> m_TX_que_;
    std::string m_RX_buf_;
};

// ������� ������ ����� �������� - �������� / �������� - �����
class game_ssesion_t : public game_checkers_t
{

};

// ������ � �����
class AI_t : /*public sql_t,*/public gamer_t
{

};

