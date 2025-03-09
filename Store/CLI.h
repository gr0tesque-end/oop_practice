#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <list>
#include <map>
#include <functional>
#include "Authenticator.h"

enum InputType {
    Option,
    Free,
    None
};

struct Node {
    std::string name;
    InputType in;
    std::map<std::string, std::shared_ptr<Node>> options;
    std::function<std::shared_ptr<Node>(std::string&)> PrecallAction;

    Node(std::string n, InputType type,
         std::map<std::string, std::shared_ptr<Node>>&& ops = {},
         std::function<std::shared_ptr<Node>(std::string&)> action = nullptr)
        : name(std::move(n)), in(type), options(std::move(ops)), PrecallAction(action) {}
};
class CLI {
    std::shared_ptr<Node> root;

public:
    CLI() {
        auto loginSuccess = std::make_shared<Node>("Login Successful!", None);
        auto loginFail = std::make_shared<Node>("Login Failed. Try Again.", Free);

        auto login = std::make_shared<Node>("Log in", Free, std::map<std::string, std::shared_ptr<Node>>{},
            [loginSuccess, loginFail](std::string& input) -> std::shared_ptr<Node> {
                std::string username, password;

                std::cout << "Username > ";
                std::getline(std::cin, username);

                std::cout << "Password > ";
                std::getline(std::cin, password);

                if (Authenticator::VerifyLogin(username, password)) {
                    return loginSuccess;
                }
                return loginFail;
            }
        );
        loginFail->PrecallAction = [login](std::string&) -> std::shared_ptr<Node> {
            return login; // Retry login
            };

        auto signup = std::make_shared<Node>("Sign up", Free, std::map<std::string, std::shared_ptr<Node>>{},
            [](std::string& input) -> std::shared_ptr<Node> {
                std::cout << "Sign-up feature not implemented yet.\n";
                return nullptr;
            });

        root = std::make_shared<Node>("Auth", Option, std::map<std::string, std::shared_ptr<Node>>{
            {"Log in", login},
            {"Sign up", signup}
            });
    }

    void Start() {
        std::shared_ptr<Node> current = root;

        while (current) {
            std::cout << '[' << current->name << ']' << "\n";

            if (current->in == Option) {
                int i = 1;
                std::vector<std::pair<std::string, std::shared_ptr<Node>>> choices;
                for (const auto& option : current->options) {
                    std::cout << i << ". " << option.first << std::endl;
                    choices.emplace_back(option.first, option.second);
                    ++i;
                }

                int choice;
                std::cout << "Choose an option: ";
                std::cin >> choice;
                std::cin.ignore();

                if (choice > 0 && choice <= static_cast<int>(choices.size())) {
                    current = choices[choice - 1].second;
                }
                else {
                    std::cout << "Invalid choice. Try again.\n";
                }
            }
            else if (current->in == Free) {
                std::string input;
                if (current->PrecallAction) {
                    current = current->PrecallAction(input);
                }
            }
            else if (current->in == None) {
                if (current->PrecallAction) {
                    std::string dummy;
                    current->PrecallAction(dummy);
                }
                current = nullptr;
            }
        }
    }
};
