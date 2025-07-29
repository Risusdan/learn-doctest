#pragma once
#include "interfaces.hpp"
#include <string>
#include <memory>
#include <vector>

class FileProcessor {
private:
    std::unique_ptr<IFileSystem> fileSystem;
    std::unique_ptr<INetworkClient> networkClient;
    std::unique_ptr<ILogger> logger;
    
public:
    FileProcessor(std::unique_ptr<IFileSystem> fs, 
                  std::unique_ptr<INetworkClient> net, 
                  std::unique_ptr<ILogger> log);
    
    bool processFile(const std::string& inputFile, const std::string& outputFile);
    bool downloadAndProcess(const std::string& url, const std::string& outputFile);
    bool backupFile(const std::string& filename);
    std::vector<std::string> processMultipleFiles(const std::vector<std::string>& files);
    size_t getTotalProcessedSize() const;
    
private:
    std::string transformContent(const std::string& content);
    bool validateContent(const std::string& content);
    
    size_t totalProcessedSize;
};