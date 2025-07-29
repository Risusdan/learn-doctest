#pragma once
#include <string>
#include <vector>

class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual bool save(const std::string& key, const std::string& value) = 0;
    virtual std::string load(const std::string& key) = 0;
    virtual bool remove(const std::string& key) = 0;
    virtual std::vector<std::string> getAllKeys() = 0;
    virtual bool exists(const std::string& key) = 0;
};

class IFileSystem {
public:
    virtual ~IFileSystem() = default;
    virtual bool writeFile(const std::string& filename, const std::string& content) = 0;
    virtual std::string readFile(const std::string& filename) = 0;
    virtual bool deleteFile(const std::string& filename) = 0;
    virtual bool fileExists(const std::string& filename) = 0;
    virtual size_t getFileSize(const std::string& filename) = 0;
};

class INetworkClient {
public:
    virtual ~INetworkClient() = default;
    virtual std::string get(const std::string& url) = 0;
    virtual bool post(const std::string& url, const std::string& data) = 0;
    virtual int getResponseCode() const = 0;
    virtual void setTimeout(int seconds) = 0;
};

class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void info(const std::string& message) = 0;
    virtual void warning(const std::string& message) = 0;
    virtual void error(const std::string& message) = 0;
    virtual void debug(const std::string& message) = 0;
};