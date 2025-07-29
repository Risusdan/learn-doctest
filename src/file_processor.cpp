#include "file_processor.hpp"
#include <algorithm>
#include <stdexcept>

FileProcessor::FileProcessor(std::unique_ptr<IFileSystem> fs, 
                           std::unique_ptr<INetworkClient> net, 
                           std::unique_ptr<ILogger> log)
    : fileSystem(std::move(fs)), networkClient(std::move(net)), logger(std::move(log)), totalProcessedSize(0) {}

bool FileProcessor::processFile(const std::string& inputFile, const std::string& outputFile) {
    if (inputFile.empty() || outputFile.empty()) {
        logger->error("Invalid file names provided");
        return false;
    }
    
    if (!fileSystem->fileExists(inputFile)) {
        logger->error("Input file does not exist: " + inputFile);
        return false;
    }
    
    logger->info("Processing file: " + inputFile);
    
    std::string content = fileSystem->readFile(inputFile);
    if (content.empty()) {
        logger->warning("Input file is empty: " + inputFile);
        return false;
    }
    
    if (!validateContent(content)) {
        logger->error("Content validation failed for: " + inputFile);
        return false;
    }
    
    std::string transformedContent = transformContent(content);
    bool success = fileSystem->writeFile(outputFile, transformedContent);
    
    if (success) {
        totalProcessedSize += content.size();
        logger->info("File processed successfully: " + inputFile + " -> " + outputFile);
    } else {
        logger->error("Failed to write output file: " + outputFile);
    }
    
    return success;
}

bool FileProcessor::downloadAndProcess(const std::string& url, const std::string& outputFile) {
    if (url.empty() || outputFile.empty()) {
        logger->error("Invalid URL or output file name");
        return false;
    }
    
    logger->info("Downloading from URL: " + url);
    
    networkClient->setTimeout(30);
    std::string content = networkClient->get(url);
    
    if (networkClient->getResponseCode() != 200) {
        logger->error("Failed to download from URL: " + url);
        return false;
    }
    
    if (content.empty()) {
        logger->warning("Downloaded content is empty from: " + url);
        return false;
    }
    
    if (!validateContent(content)) {
        logger->error("Downloaded content validation failed from: " + url);
        return false;
    }
    
    std::string transformedContent = transformContent(content);
    bool success = fileSystem->writeFile(outputFile, transformedContent);
    
    if (success) {
        totalProcessedSize += content.size();
        logger->info("URL content processed successfully: " + url + " -> " + outputFile);
    } else {
        logger->error("Failed to save processed content to: " + outputFile);
    }
    
    return success;
}

bool FileProcessor::backupFile(const std::string& filename) {
    if (filename.empty()) {
        logger->error("Cannot backup: empty filename");
        return false;
    }
    
    if (!fileSystem->fileExists(filename)) {
        logger->error("Cannot backup non-existent file: " + filename);
        return false;
    }
    
    std::string backupName = filename + ".backup";
    std::string content = fileSystem->readFile(filename);
    
    bool success = fileSystem->writeFile(backupName, content);
    
    if (success) {
        logger->info("File backed up successfully: " + filename + " -> " + backupName);
    } else {
        logger->error("Failed to backup file: " + filename);
    }
    
    return success;
}

std::vector<std::string> FileProcessor::processMultipleFiles(const std::vector<std::string>& files) {
    std::vector<std::string> results;
    
    logger->info("Processing multiple files, count: " + std::to_string(files.size()));
    
    for (const auto& file : files) {
        std::string outputFile = file + ".processed";
        if (processFile(file, outputFile)) {
            results.push_back(outputFile);
        }
    }
    
    logger->info("Successfully processed " + std::to_string(results.size()) + " out of " + std::to_string(files.size()) + " files");
    
    return results;
}

size_t FileProcessor::getTotalProcessedSize() const {
    return totalProcessedSize;
}

std::string FileProcessor::transformContent(const std::string& content) {
    std::string transformed = content;
    std::transform(transformed.begin(), transformed.end(), transformed.begin(), ::toupper);
    return "PROCESSED: " + transformed;
}

bool FileProcessor::validateContent(const std::string& content) {
    return !content.empty() && content.size() <= 1000000;
}