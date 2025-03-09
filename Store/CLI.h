#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <list>
#include <functional>
#include "Authenticator.h"
#include <unordered_map>

class CLI {
	Account* acc{};
public:

	enum class StateType {
		OPTION,
		FREE
	};

	enum class State {
		Auth,
		LogIn,
		SignUp,
		INPUT_USERNAME,
		INPUT_PASSWORD,
		EXIT
	};

	CLI() {
		currentState = State::Auth;
	}

	void run() {
		while (currentState != State::EXIT) {
			StateInfo& stateInfo = handlers[currentState];

			stateInfo.handler();

			if (stateInfo.type == StateType::OPTION) {
				int choice;
				std::cin >> choice;

				if (stateInfo.nextStates.find(choice) != stateInfo.nextStates.end()) {
					currentState = stateInfo.nextStates[choice];
				}
				else {
					std::cout << "Invalid choice. Please try again.\n";
				}
			}
			else if (stateInfo.type == StateType::FREE) {
				currentState = stateInfo.nextState;
			}
		}

		std::cout << "Exiting CLI. Goodbye!\n";
	}

private:
	struct StateInfo {
		StateType type;
		std::function<void()> handler;
		std::unordered_map<int, State> nextStates;
		State nextState;
	};

	State currentState;

	std::string username;
	std::string password;

	std::unordered_map<State, StateInfo> handlers {
			{State::Auth, {StateType::OPTION, [this]() { Start(); }, {{1, State::LogIn}, {2, State::SignUp}}, State::EXIT}},
			{State::LogIn, {StateType::FREE, [this]() { LogIn(); }, {{1, State::INPUT_USERNAME}, {2, State::Auth}}, State::EXIT}},
			{State::SignUp, {StateType::FREE, [this]() { handleMenu2(); }, {{1, State::INPUT_PASSWORD}, {2, State::Auth}}, State::EXIT}},
			{State::INPUT_USERNAME, {StateType::FREE, [this]() { handleUsernameInput(); }, {}, State::LogIn}},
			{State::INPUT_PASSWORD, {StateType::FREE, [this]() { handlePasswordInput(); }, {}, State::SignUp}}
	};

	void Start() {
		std::cout << "[Auth]\n";
		std::cout << "1. Log in\n";
		std::cout << "2. Sign up\n";
		std::cout << "> ";
	}

	void LogIn() {
		std::string username, password;

		std::cout << "[Log in]\n";
		std::cout << "Username (.q to cancel) > ";
		std::getline(std::cin, username);

		if (username.starts_with(".q"));

		std::cout << "Password > ";
		std::getline(std::cin, password);

		if (auto l = Authenticator::VerifyLogin(username, password))
		{
			acc = l;
		}
	}

	void handleMenu2() {
		std::cout << "You are in Menu 2.\n";
		std::cout << "1. Request Password\n";
		std::cout << "2. Go Back\n";
		std::cout << "> ";
	}

	void handleUsernameInput() {
		std::cout << "Enter your username: ";
		std::cin >> username;
		std::cout << "Username set to: " << username << "\n";
	}

	void handlePasswordInput() {
		std::cout << "Enter your password: ";
		std::cin >> password;
		std::cout << "Password set.\n";
	}
};