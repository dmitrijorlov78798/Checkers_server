#pragma once
#ifndef __CHECKERS_MSG_H_
#define __CHECKERS_MSG_H_

#include "game_core/game_core.h"
#include <optional>

enum class typeMsg
{
	requestStep,
	answerStep,
	undefined
};

class Checkers_MSG_t
{
public:
	
	static const std::string EOM;
	
	Checkers_MSG_t(const std::string& __buffer) 
	{
		deserialize(__buffer);
	}
	Checkers_MSG_t(const std::list<step_t>& possibleSteps) : m_type_(typeMsg::requestStep)
	{
		m_payload_.append("size=" + std::to_string(possibleSteps.size()));
		for (const auto& elem : possibleSteps)
			m_payload_.append(delimiter + elem.toString());
	}

	bool deserialize(const std::string& buffer)
	{
		bool result = false;

		return result;
	}

	std::optional<step_t> get_step() const
	{
		std::optional<step_t> result = std::nullopt;

		return result;
	}

	void serialize(std::string& buffer) const
	{
		buffer.clear();
		buffer.reserve(m_payload_.size() + STX.size() + EOM.size() + 3 * delimiter.size() + 3/*type && crc*/);

		buffer = STX + delimiter + char(m_type_) + delimiter + m_payload_ + delimiter;
		_calc_crc(buffer);
	}
//protected :
	inline typeMsg get_type() const { return m_type_; }
	inline unsigned get_offset() const { return m_offset_; }
	inline void set_offset(unsigned __offset) { m_offset_ = __offset; }
private :
	void _calc_crc(std::string& buffer) const
	{
		unsigned short crc = 0;

		for (const auto& elem : buffer)
			crc += elem;

		buffer += (char((crc >> 8) & 0xFF)) + (char(crc & 0xFF));
	}
protected :
	std::string m_payload_;
	static const std::string STX;
	static const std::string delimiter;
	typeMsg m_type_ = typeMsg::undefined;
	unsigned m_offset_ = 0;
};

const std::string Checkers_MSG_t::STX = "STX";
const std::string Checkers_MSG_t::EOM = "EOM";
const std::string Checkers_MSG_t::delimiter = "|";


#endif