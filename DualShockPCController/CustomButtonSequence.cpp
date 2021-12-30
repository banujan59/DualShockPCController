#include "CustomButtonSequence.h"

CustomButtonSequence::CustomButtonSequence() :
	m_pNodeIterator(nullptr),
	m_invalidKeySequence(false)
{
}

bool CustomButtonSequence::AddCommand(const std::string& commandName, const std::vector<int>& buttonList, std::function<void()>& functionToExecute)
{
	if (buttonList.size() <= 1 || commandName.empty() || m_customCommandInfos.count(commandName) > 0)
		return false;

	if (m_customCommandNodes.count(buttonList[0]) == 0)
	{
		m_customCommandNodes[buttonList[0]] = CustomCommandNode();
	}

	CustomCommandNode* currentNode = &m_customCommandNodes[buttonList[0]];

	for (size_t i = 1; i < buttonList.size(); i++)
	{
		if (currentNode->nextNodes.count(buttonList[i]) == 0)
		{
			currentNode->nextNodes[buttonList[i]] = CustomCommandNode();
		}

		currentNode = &currentNode->nextNodes[buttonList[i]];

		if (i == buttonList.size() - 1)
		{
			currentNode->actionName = commandName;
		}
	}

	CustomCommandInfo commandInfo;
	commandInfo.keySequence = buttonList;
	commandInfo.functionToExecute = functionToExecute;
	m_customCommandInfos[commandName] = commandInfo;

	return true;
}

bool CustomButtonSequence::DeleteCommand(const std::string& commandName)
{
	if (m_customCommandInfos.count(commandName) == 0)
		return false;

	int button = m_customCommandInfos[commandName].keySequence[0];
	auto iterator = m_customCommandNodes.find(button);

	if (iterator != m_customCommandNodes.end())
	{
		if (DeleteRecursively(iterator->second, 1, m_customCommandInfos[commandName].keySequence))
		{
			m_customCommandNodes.erase(iterator);
		}

		m_customCommandInfos.erase(commandName);
		return true;
	}

	return false;
}

void CustomButtonSequence::IterateNext(int buttonPressed)
{
	if (!m_invalidKeySequence)
	{
		if (m_customCommandNodes.empty())
			SetInvalidKeySequence();

		else
		{
			if (m_pNodeIterator == nullptr)
			{
				// we are at the beginning of a sequence
				if (m_customCommandNodes.count(buttonPressed) > 0)
				{
					m_pNodeIterator = &m_customCommandNodes[buttonPressed];
				}
			}

			else
			{
				// check if button pressed is available on the following node
				if (m_pNodeIterator->nextNodes.count(buttonPressed) > 0)
				{
					m_pNodeIterator = &m_pNodeIterator->nextNodes[buttonPressed];
				}

				else
					SetInvalidKeySequence();
			}
		}
	}
}

void CustomButtonSequence::ResetIterator()
{
	m_pNodeIterator = nullptr;
	m_invalidKeySequence = false;
}

bool CustomButtonSequence::CommandAvaialbleAtCurrentNode()
{
	return m_pNodeIterator != nullptr && !m_pNodeIterator->actionName.empty();
}

std::string CustomButtonSequence::GetCommandNameAtCurrentNode()
{
	if (CommandAvaialbleAtCurrentNode())
	{
		return m_pNodeIterator->actionName;
	}

	return "";
}

void CustomButtonSequence::ExecuteCommandAtCurrentNode()
{
	if(CommandAvaialbleAtCurrentNode())
	{
		std::string commandName = GetCommandNameAtCurrentNode();
		m_customCommandInfos[commandName].functionToExecute();
	}
}


void CustomButtonSequence::SetInvalidKeySequence()
{
	m_pNodeIterator = nullptr;
	m_invalidKeySequence = true;
}

bool CustomButtonSequence::DeleteRecursively(CustomCommandNode& currentNode, const unsigned int& nodeIndex, const std::vector<int>& buttonList)
{
	if (nodeIndex == buttonList.size())
	{
		currentNode.actionName = "";
		if (currentNode.nextNodes.empty())
			return true;
		else
			return false;
	}


	int button = buttonList[nodeIndex];
	auto iterator = currentNode.nextNodes.find(button);
	if (iterator != currentNode.nextNodes.end())
	{
		if (DeleteRecursively(iterator->second, nodeIndex + 1, buttonList))
		{
			currentNode.nextNodes.erase(iterator);

			if (currentNode.nextNodes.empty() && currentNode.actionName.empty())
				return true;

			else
				return false;
		}

		else
			return false;
	}

	return false; // normally we should not get here...
}