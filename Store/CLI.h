#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <tuple>
#include <functional>
#include "Authenticator.h"
#include <unordered_map>
#include <concepts>
#include <set>

template<typename T>
concept isInputNode = requires(const T & t) {
	t.OptionInput();
};

template<typename T>
concept isProcessingNode = requires(const T & t) {
	t.Start();
};

class CLI {
public:
	struct Node;
	struct InputNode;

	std::shared_ptr<Account> acc;

	std::shared_ptr<Node> Current;

	enum InputType {
		Option,
		Manual
	};

	template<typename... Args>
	using VariadicFunctionType = std::function<void(Args...)>;
	template<class T>
	using FunctionType = std::function<void(T)>;

	using DefaultPNodeFParamsT = std::vector<std::string>&;
	using DefaultPNodeFType = std::function<void(DefaultPNodeFParamsT)>;
	
	template<class T = DefaultPNodeFType, class T2 = CLI::DefaultPNodeFParamsT> struct ProcessingNode;

	using DefaultPNodeT = CLI::ProcessingNode<DefaultPNodeFType, DefaultPNodeFParamsT>;

public:
	using Node_InitializationPack = std::tuple<const char*>;
	using InputNode_InitializationPack = std::tuple<Node_InitializationPack, CLI::InputType, std::map<std::string, std::shared_ptr<Node>>>;
	using ProcessingNode_InitializationPack = std::tuple<Node_InitializationPack, DefaultPNodeFType>;
	/*
	CLI& push_back(std::shared_ptr<Node> newNode) {
		if (!newNode) {
			throw std::invalid_argument("Cannot push a null node.");
		}

		if (!Head) {
			
			Head = newNode;
			Tail = newNode;
		}
		else {
			newNode->prev = Tail; 
			Tail->next = newNode;
			Tail = newNode; 
		}

		return *this;
	}
	*/
	CLI();
};

template<typename T>
concept isNode = std::is_base_of_v<CLI::Node, T>;

template<isNode T/*, typename I = typename ObjectData<T>::I  Overcomplicated this */>
static std::shared_ptr<T> BuildNode(CLI::InputNode_InitializationPack pack) {
	std::shared_ptr<T> res;

	return res;
}

template<isNode T/*, typename I = typename ObjectData<T>::I  Overcomplicated this */>
static std::shared_ptr<T> BuildNode(CLI::ProcessingNode_InitializationPack pack) {
	std::shared_ptr<T> res = std::make_shared(std::make_from_tuple<CLI::DefaultPNodeT>(pack));


	return res;
}

#pragma region Implementation


struct CLI::Node {
	std::string name;
	// Used for the forwarding to the next handler
	std::shared_ptr<Node> next{};
	std::set<std::shared_ptr<Node>> branches{};

	Node(std::string& n) : name{ n } {

	}

	virtual void Start() = 0;
};

template<class T, class T2>
struct CLI::ProcessingNode :
	Node
{
	ProcessingNode(CLI::Node_InitializationPack NP, T func)
		: func{ func }, Node{ std::make_from_tuple<Node>(NP) } {

	}

	T2 params;
	T func;
	virtual void Start() override {
		func(params);
	}
};

struct CLI::InputNode :
	CLI::Node
{
	std::map<std::string, std::shared_ptr<Node>> options;
	// If InputType::Manual; Node::next should be assigned manually
	CLI::InputType inType;

	virtual void Start() override {
		switch (inType)
		{
		case CLI::Option:
			OptionInput();
			break;
		case CLI::Manual:
			ManualInput();
			break;
		default:
			std::cerr << "catch::default\n";
			break;
		}
	}
	void OptionInput() {
		int i = 1;
		std::vector<std::pair<std::string, std::shared_ptr<Node>>> choices;
		for (const auto& option : options) {
			std::cout << i << ". " << option.first << std::endl;
			choices.emplace_back(option.first, option.second);
			++i;
		}
		std::string choice;
		std::cout << "> ";
		std::getline(std::cin, choice);

		int c = atoi(choice.c_str());

		if (c > 0 && c <= choices.size()) {
			next = choices[c - 1].second;
		}
		else {
			std::cout << "Invalid choice. Try again.\n";
		}
	}

	void ManualInput() {
		std::vector<std::string> inputParams{ options.size() };
		int i{};
		for (std::pair<std::string, std::shared_ptr<Node>> o : options) {
			std::string loc{};
			std::string& msg = o.first;
			std::cout << msg << " (.q to cancel) > ";
			std::getline(std::cin, loc);

			if (loc.starts_with(".q")) {
				return;
			}

			inputParams[i] = loc;
			++i;
		}
		if (auto c = dynamic_pointer_cast<CLI::DefaultPNodeT, CLI::Node>(next)) {
			c->params = inputParams;
			c->Start();
		}
		else { std::cerr << "The CLI::Node::next isn't a 'CLI::ProcessingNode<FunctionType<std::vector<std::string>>>' type\n"; }
	}
};

CLI::CLI() {
	/*auto n = BuildNode<CLI::InputNode>(
		CLI::InputNode_InitializationPack(
			CLI::Node_InitializationPack{ "Auth" },
			CLI::InputType::Option,
			{}
		)
	);
	n->options.insert({ "Log In", (BuildNode<CLI::DefaultPNodeT>(
		CLI::ProcessingNode_InitializationPack(
				CLI::Node_InitializationPack{ "Log_In_Validate" },
				[&](std::vector<std::string>& v) {
					acc = std::make_shared<Account>(*Authenticator::VerifyLogin(v[0], v[1]));
					if (acc) 
				}
			)
		))});*/

}

#pragma endregion


/*template<typename T, typename Enable = void>
struct ObjectData {
	using I = void;
};

// (higher priority)
template<typename T>
struct ObjectData<T, std::enable_if_t<isInputNode<T>>> {
	using I = CLI::InputNode_InitializationPack;
};

// (lower priority)
template<typename T>
struct ObjectData<T, std::enable_if_t<isProcessingNode<T> && !isInputNode<T>>> {
	// Assumes the ProcessingNode; 
	// Mod if more instances of are made of Node that needs to be checked
	using I = CLI::ProcessingNode_InitializationPack;
};
*/
