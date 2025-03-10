#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <list>
#include <map>
#include <functional>
#include "Authenticator.h"

// Do not proceed any further
// Risk getting an aneurysm

enum InputType {
    Option,
    Free,
    None
};

struct Node {
    std::string name;
    InputType in;
    std::map<std::string, std::shared_ptr<Node>> options;
    std::function<std::shared_ptr<Node>()> Action;

    Node(std::string n, InputType type,
         std::map<std::string, std::shared_ptr<Node>>&& ops = std::map<std::string, std::shared_ptr<Node>>{},
         std::function<std::shared_ptr<Node>()> action = nullptr)
        : name(std::move(n)), in(type), options(std::move(ops)), Action(action) {}
};

//decltype(auto) lambdaType = []() { return std::make_shared<Node>(); };

class CLI { 
    std::list<std::shared_ptr<Node>> handlers;
    std::shared_ptr<Node> Next = nullptr;
    std::shared_ptr<Node> root;
    Db* db = Db::GetInstance();
    Account* acc = nullptr;
public:
    friend class Account;

    CLI() {
        auto nullHandler = std::make_shared<Node>("Null", None);

        Product* p{};

        auto loginSuccess = std::make_shared<Node>("Login Successful!", None, std::map<std::string, std::shared_ptr<Node>>{});
        auto loginFail = std::make_shared<Node>("Login Failed. Try Again.", Free);

        auto login = std::make_shared<Node>("Log in", Free, std::map<std::string, std::shared_ptr<Node>>{});

        std::shared_ptr<Node> auth = nullHandler;

        auto signup = std::make_shared<Node>("Sign up", Free, std::map<std::string, std::shared_ptr<Node>>{},
            [&]() -> std::shared_ptr<Node> {
                std::cout << "Sign-up feature not implemented yet.\n";
                return auth;
            });


        auth = std::make_shared<Node>("Auth", Option, std::map<std::string, std::shared_ptr<Node>>{
            {"Log in", login},
            { "Sign up", signup }
        });

        root = auth;

        login->Action = [&]() -> std::shared_ptr<Node> {
            std::string username, password;

            std::cout << "Username (.q to cancel) > ";
            std::getline(std::cin, username);

            if (username.starts_with(".q")) return root;

            std::cout << "Password > ";
            std::getline(std::cin, password);

            std::shared_ptr<Node> menu = nullptr;

            Account* l{};

            if (!(l = Authenticator::VerifyLogin(username, password))) 
            {
                return loginFail;
            }

            acc = l;

            if (l->GetAuthority() < 50) {
                // Customer Menu 
                menu = std::make_shared<Node>("Menu (Customer)", Option,
                    std::map<std::string, std::shared_ptr<Node>>{
                        { "See the Catalog", std::make_shared<Node>("Catalog", None, std::map<std::string, std::shared_ptr<Node>>{},
                            [&]() -> std::shared_ptr<Node> {
                                return menu;
                            })},
                        { "Buy", std::make_shared<Node>("Buy", Free, std::map<std::string, std::shared_ptr<Node>>{},
                            [&]() {
                                std::string input{};
                                std::cout << "Product_Id (.q to cancel) > ";
                                std::getline(std::cin, input);
                                if (input.starts_with(".q")) this->Next = menu;

                                for (auto pr : db->Products) {
                                    if (pr->GetId() == atoi(input.c_str())) {
                                        p = pr;
                                        break;
                                    }
                                }

                                return std::make_shared<Node>("Purchasing", None, std::map<std::string, std::shared_ptr<Node>>{},
                                    [&p]()-> std::shared_ptr<Node> {
                                        return nullptr;
                                    });
                            })}
                            // Additional Options
                });
            }
            else if (l->GetAuthority() < 100) {
                // TODO: implementation for higher authoritie's menu's
                // Possibly extending customers
            }
            this->Next = menu;
            return loginSuccess;
            };

        loginFail->Action = [login]() -> std::shared_ptr<Node> {
            return login; // Retry login
            };
        
        handlers = {
            nullHandler,
            auth,
            login,
            loginSuccess,
            loginFail,
            signup
        };
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

                std::string choice;
                std::cout << "Choose an option: ";
                std::getline(std::cin, choice);

                int c = atoi(choice.c_str());

                if (c > 0 && c <= static_cast<int>(choices.size())) {
                    current = choices[c - 1].second;
                }
                else {
                    std::cout << "Invalid choice. Try again.\n";
                }
            }
            else if (current->in == Free) {
                if (current->Action) {
                    current = current->Action();
                }
            }
            else if (current->in == None) {
                if (current->Action) {
                    current->Action();
                }
                if (Next) current = Next;
                Next = nullptr;
            }
        }
    }
};
