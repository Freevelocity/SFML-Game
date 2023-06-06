#include "CommandQueue.h"

#include "SceneNode.h"


void CommandQueue::Push(const Command& pCommand)
{
	m_oQueue.push(pCommand);
}

Command CommandQueue::Pop()
{
	Command oCommand = m_oQueue.front();
	m_oQueue.pop();
	return oCommand;
}

bool CommandQueue::IsEmpty() const
{
	return m_oQueue.empty();
}