#pragma once
#include "interfaces.hpp"
#include <string>
#include <memory>

struct User {
    std::string id;
    std::string name;
    std::string email;
    
    User(const std::string& userId, const std::string& userName, const std::string& userEmail)
        : id(userId), name(userName), email(userEmail) {}
};

class UserService {
private:
    std::unique_ptr<IDatabase> database;
    std::unique_ptr<ILogger> logger;
    
public:
    UserService(std::unique_ptr<IDatabase> db, std::unique_ptr<ILogger> log);
    
    bool createUser(const User& user);
    User* getUser(const std::string& userId);
    bool updateUser(const User& user);
    bool deleteUser(const std::string& userId);
    std::vector<std::string> getAllUserIds();
    
private:
    std::string userToString(const User& user);
    User stringToUser(const std::string& data);
};