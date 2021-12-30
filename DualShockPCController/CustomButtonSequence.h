#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <functional>

class CustomButtonSequence
{
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
	};

	CustomCommandNode* m_pNodeIterator;
	std::unordered_map<int, CustomCommandNode> m_customCommandNodes;
	std::unordered_map<std::string, CustomCommandInfo> m_customCommandInfos;
	bool m_invalidKeySequence;

	void SetInvalidKeySequence();
	bool DeleteRecursively(CustomCommandNode& currentNode, const unsigned int& nodeIndex, const std::vector<int>& buttonList);

public:
	CustomButtonSequence();

	bool AddCommand(const std::string& commandName, const std::vector<int>& buttonList, std::function<void()>& functionToExecute);
	bool DeleteCommand(const std::string& commandName);
	void IterateNext(int buttonPressed);

	void ResetIterator();
	bool CommandAvaialbleAtCurrentNode();
	std::string GetCommandNameAtCurrentNode();
	void ExecuteCommandAtCurrentNode();
};