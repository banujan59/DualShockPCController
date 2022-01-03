#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <map>
#include "OSHelper.h"

#include <boost/serialization/map.hpp>

class CustomButtonSequence
{
public:
	enum class ActionType { OPEN_FILE_OR_PROGRAM };

	static void GetActionTypeNames(std::map<CustomButtonSequence::ActionType, std::string>& container);

	CustomButtonSequence();

	bool AddCommand(const std::string& commandName, const std::vector<int>& buttonList, const
	                CustomButtonSequence::ActionType& actionType, OSHelper::FunctionEnum functionToExecute, std::vector<std::string>&
	                actionParameters);
	bool DeleteCommand(const std::string& commandName);
	void IterateNext(int buttonPressed);

	void ResetIterator();
	bool CommandAvailableAtCurrentNode();
	std::string GetCommandNameAtCurrentNode();
	void ExecuteCommandAtCurrentNode();

	void GetAllCustomCommands(std::vector<std::string>& commandNames, std::vector<std::string>& buttonList, std::vector<std::string>& actionType);

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& m_customCommandNodes;
		ar& m_customCommandInfos;
	}

	struct CustomCommandNode
	{
		std::string actionName;
		std::unordered_map<int, CustomCommandNode> nextNodes;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& actionName;
			ar& nextNodes;
		}
	};

	struct CustomCommandInfo
	{
		std::vector<int> keySequence;
		OSHelper::FunctionEnum functionToExecute;
		std::vector<std::string> functionToExecuteParams;
		ActionType actionType;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& keySequence;
			ar& functionToExecute;
			ar& functionToExecuteParams;
			ar& actionType;
		}
	};

	CustomCommandNode* m_pNodeIterator;
	std::unordered_map<int, CustomCommandNode> m_customCommandNodes;
	std::unordered_map<std::string, CustomCommandInfo> m_customCommandInfos;
	bool m_invalidKeySequence;

	void SetInvalidKeySequence();
	bool DeleteRecursively(CustomCommandNode& currentNode, const unsigned int& nodeIndex, const std::vector<int>& buttonList);
};