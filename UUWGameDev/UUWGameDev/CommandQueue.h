#pragma once

#include "Command.h"
#include <queue>


class CommandQueue
{
public:
	void Push(const Command& command);
	Command	Pop();
	bool IsEmpty() const;


private:
	std::queue<Command>	 m_oQueue;
};