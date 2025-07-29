#include "user_service.hpp"
#include <sstream>
#include <stdexcept>

UserService::UserService(std::unique_ptr<IDatabase> db, std::unique_ptr<ILogger> log)
    : database(std::move(db)), logger(std::move(log)) {}

bool UserService::createUser(const User& user) {
    if (user.id.empty() || user.name.empty() || user.email.empty()) {
        logger->error("Invalid user data: missing required fields");
        return false;
    }
    
    if (database->exists(user.id)) {
        logger->warning("User already exists: " + user.id);
        return false;
    }
    
    std::string userData = userToString(user);
    bool success = database->save(user.id, userData);
    
    if (success) {
        logger->info("User created successfully: " + user.id);
    } else {
        logger->error("Failed to create user: " + user.id);
    }
    
    return success;
}

User* UserService::getUser(const std::string& userId) {
    if (userId.empty()) {
        logger->error("Invalid user ID: empty string");
        return nullptr;
    }
    
    if (!database->exists(userId)) {
        logger->warning("User not found: " + userId);
        return nullptr;
    }
    
    std::string userData = database->load(userId);
    if (userData.empty()) {
        logger->error("Failed to load user data: " + userId);
        return nullptr;
    }
    
    try {
        User user = stringToUser(userData);
        logger->info("User retrieved successfully: " + userId);
        return new User(user);
    } catch (const std::exception& e) {
        logger->error("Failed to parse user data: " + userId);
        return nullptr;
    }
}

bool UserService::updateUser(const User& user) {
    if (user.id.empty()) {
        logger->error("Cannot update user: empty ID");
        return false;
    }
    
    if (!database->exists(user.id)) {
        logger->warning("Cannot update non-existent user: " + user.id);
        return false;
    }
    
    std::string userData = userToString(user);
    bool success = database->save(user.id, userData);
    
    if (success) {
        logger->info("User updated successfully: " + user.id);
    } else {
        logger->error("Failed to update user: " + user.id);
    }
    
    return success;
}

bool UserService::deleteUser(const std::string& userId) {
    if (userId.empty()) {
        logger->error("Cannot delete user: empty ID");
        return false;
    }
    
    if (!database->exists(userId)) {
        logger->warning("Cannot delete non-existent user: " + userId);
        return false;
    }
    
    bool success = database->remove(userId);
    
    if (success) {
        logger->info("User deleted successfully: " + userId);
    } else {
        logger->error("Failed to delete user: " + userId);
    }
    
    return success;
}

std::vector<std::string> UserService::getAllUserIds() {
    logger->debug("Retrieving all user IDs");
    return database->getAllKeys();
}

std::string UserService::userToString(const User& user) {
    return user.id + "|" + user.name + "|" + user.email;
}

User UserService::stringToUser(const std::string& data) {
    std::istringstream ss(data);
    std::string id, name, email;
    
    if (!std::getline(ss, id, '|') || 
        !std::getline(ss, name, '|') || 
        !std::getline(ss, email)) {
        throw std::invalid_argument("Invalid user data format");
    }
    
    return User(id, name, email);
}