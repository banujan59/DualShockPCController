#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <map>
#include <functional>

class CustomButtonSequence
{
public:
	enum class ActionType { OPEN_FILE_OR_PROGRAM };

	static void GetActionTypeNames(std::map<CustomButtonSequence::ActionType, std::string>& container);

	CustomButtonSequence();

	bool AddCommand(const std::string& commandName, const std::vector<int>& buttonList, std::function<void()>& functionToExecute, const
	                CustomButtonSequence::ActionType& actionType);
	bool DeleteCommand(const std::string& commandName);
	void IterateNext(int buttonPressed);

	void ResetIterator();
	bool CommandAvailableAtCurrentNode();
	std::string GetCommandNameAtCurrentNode();
	void ExecuteCommandAtCurrentNode();

	void GetAllCustomCommands(std::vector<std::string>& commandNames, std::vector<std::string>& buttonList, std::vector<std::string>& actionType);

private:
	struct CustomCommandNode
	{
		std::string actionName;
		std::unordered_map<int, CustomCommandNode> nextNodes;
	};

	struct CustomCommandInfo
	{
		std::vector<int> keySequence;
		std::function<void()> functionToExecute;
		ActionType actionType;
	};

	CustomCommandNode* m_pNodeIterator;
	std::unordered_map<int, CustomCommandNode> m_customCommandNodes;
	std::unordered_map<std::string, CustomCommandInfo> m_customCommandInfos;
	bool m_invalidKeySequence;

	void SetInvalidKeySequence();
	bool DeleteRecursively(CustomCommandNode& currentNode, const unsigned int& nodeIndex, const std::vector<int>& buttonList);
};