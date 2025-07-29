#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest.h"
#include "../fakeit.hpp"
#include "../src/interfaces.hpp"
#include <stdexcept>

using namespace fakeit;

// Simplified file processor for easier testing
class SimpleFileProcessor {
private:
    IFileSystem* fileSystem;
    INetworkClient* networkClient;
    ILogger* logger;
    
public:
    SimpleFileProcessor(IFileSystem* fs, INetworkClient* net, ILogger* log)
        : fileSystem(fs), networkClient(net), logger(log) {}
    
    bool processFile(const std::string& filename) {
        logger->info("Processing file: " + filename);
        
        if (!fileSystem->fileExists(filename)) {
            logger->error("File not found: " + filename);
            return false;
        }
        
        std::string content = fileSystem->readFile(filename);
        std::string processed = "PROCESSED: " + content;
        
        return fileSystem->writeFile(filename + ".out", processed);
    }
    
    bool downloadFile(const std::string& url) {
        logger->info("Downloading: " + url);
        
        std::string content = networkClient->get(url);
        
        if (networkClient->getResponseCode() != 200) {
            logger->error("Download failed: " + url);
            return false;
        }
        
        return fileSystem->writeFile("downloaded.txt", content);
    }
};

TEST_CASE("Advanced mocking - Exception handling") {
    Mock<IFileSystem> mockFs;
    Mock<ILogger> mockLogger;
    
    // File system throws an exception
    When(Method(mockFs, fileExists).Using("error.txt"))
        .Throw(std::runtime_error("Disk I/O error"));
    When(Method(mockLogger, info).Using(_)).AlwaysReturn();
    When(Method(mockLogger, error).Using(_)).AlwaysReturn();
    
    SimpleFileProcessor processor(&mockFs.get(), nullptr, &mockLogger.get());
    
    CHECK_THROWS_AS(processor.processFile("error.txt"), std::runtime_error);
    
    Verify(Method(mockLogger, info).Using(_)).Once();
    Verify(Method(mockFs, fileExists).Using("error.txt")).Once();
}

TEST_CASE("Sequential return values - Network responses") {
    Mock<IFileSystem> mockFs;
    Mock<INetworkClient> mockNet;
    Mock<ILogger> mockLogger;
    
    // Simulate different network responses
    When(Method(mockNet, get).Using("http://test.com"))
        .Return("Content 1", "Content 2", "");
    
    When(Method(mockNet, getResponseCode))
        .Return(200, 200, 404);
    
    When(Method(mockFs, writeFile).Using(_, _)).AlwaysReturn(true);
    When(Method(mockLogger, info).Using(_)).AlwaysReturn();
    When(Method(mockLogger, error).Using(_)).AlwaysReturn();
    
    SimpleFileProcessor processor(&mockFs.get(), &mockNet.get(), &mockLogger.get());
    
    // First request succeeds
    CHECK(processor.downloadFile("http://test.com") == true);
    
    // Second request succeeds
    CHECK(processor.downloadFile("http://test.com") == true);
    
    // Third request fails (404)
    CHECK(processor.downloadFile("http://test.com") == false);
    
    Verify(Method(mockNet, get).Using("http://test.com")).Exactly(3_Times);
    Verify(Method(mockNet, getResponseCode)).Exactly(3_Times);
    Verify(Method(mockFs, writeFile)).Exactly(2_Times);  // Only 2 successful downloads
}

TEST_CASE("Mock call counting") {
    Mock<ILogger> mockLogger;
    Mock<IFileSystem> mockFs;
    
    // Set up normal mocks instead of spies
    When(Method(mockLogger, info).Using(_)).AlwaysReturn();
    When(Method(mockLogger, error).Using(_)).AlwaysReturn();
    When(Method(mockFs, fileExists).Using(_)).AlwaysReturn(true);
    When(Method(mockFs, readFile).Using(_)).AlwaysReturn("test content");
    When(Method(mockFs, writeFile).Using(_, _)).AlwaysReturn(true);
    
    SimpleFileProcessor processor(&mockFs.get(), nullptr, &mockLogger.get());
    
    bool result = processor.processFile("test.txt");
    CHECK(result == true);
    
    // Verify the calls were recorded
    Verify(Method(mockLogger, info).Using(_)).Once();
    VerifyNoOtherInvocations(Method(mockLogger, error));
}

TEST_CASE("Complex argument matching") {
    Mock<IFileSystem> mockFs;
    Mock<ILogger> mockLogger;
    
    // Use argument matchers for flexible verification
    When(Method(mockFs, fileExists).Using(_)).AlwaysReturn(true);
    When(Method(mockFs, readFile).Using(_)).AlwaysReturn("content");
    When(Method(mockFs, writeFile).Using(_, _)).AlwaysReturn(true);
    When(Method(mockLogger, info).Using(_)).AlwaysReturn();
    
    SimpleFileProcessor processor(&mockFs.get(), nullptr, &mockLogger.get());
    
    processor.processFile("file1.txt");
    processor.processFile("file2.log");
    processor.processFile("data.xml");
    
    // Verify patterns with wildcards
    Verify(Method(mockFs, fileExists).Using(_)).Exactly(3_Times);
    Verify(Method(mockFs, readFile).Using(_)).Exactly(3_Times);
    Verify(Method(mockFs, writeFile).Using(_, _)).Exactly(3_Times);
    Verify(Method(mockLogger, info).Using(_)).Exactly(3_Times);
}

SCENARIO("Advanced mocking workflow") {
    GIVEN("A file processor with mocked dependencies") {
        Mock<IFileSystem> mockFs;
        Mock<INetworkClient> mockNet;
        Mock<ILogger> mockLogger;
        
        SimpleFileProcessor processor(&mockFs.get(), &mockNet.get(), &mockLogger.get());
        
        WHEN("processing a file that exists") {
            When(Method(mockFs, fileExists).Using("input.txt")).Return(true);
            When(Method(mockFs, readFile).Using("input.txt")).Return("hello world");
            When(Method(mockFs, writeFile).Using(_, _)).AlwaysReturn(true);
            When(Method(mockLogger, info).Using(_)).AlwaysReturn();
            
            bool result = processor.processFile("input.txt");
            
            THEN("processing should succeed") {
                CHECK(result == true);
            }
            
            AND_THEN("all expected interactions should occur") {
                Verify(Method(mockFs, fileExists).Using("input.txt")).Once();
                Verify(Method(mockFs, readFile).Using("input.txt")).Once();
                Verify(Method(mockFs, writeFile).Using(_, _)).Once();
                Verify(Method(mockLogger, info).Using(_)).Once();
            }
        }
        
        WHEN("processing a file that doesn't exist") {
            When(Method(mockFs, fileExists).Using("missing.txt")).Return(false);
            When(Method(mockLogger, info).Using(_)).AlwaysReturn();
            When(Method(mockLogger, error).Using(_)).AlwaysReturn();
            
            bool result = processor.processFile("missing.txt");
            
            THEN("processing should fail") {
                CHECK(result == false);
            }
            
            AND_THEN("appropriate error should be logged") {
                Verify(Method(mockLogger, error).Using(_)).Once();
                VerifyNoOtherInvocations(Method(mockFs, readFile));
                VerifyNoOtherInvocations(Method(mockFs, writeFile));
            }
        }
    }
}

TEST_CASE("Mock state management across calls") {
    Mock<ILogger> mockLogger;
    
    // Track how many times info is called
    When(Method(mockLogger, info).Using(_)).AlwaysReturn();
    
    Mock<IFileSystem> mockFs;
    When(Method(mockFs, fileExists).Using(_)).AlwaysReturn(true);
    When(Method(mockFs, readFile).Using(_)).AlwaysReturn("data");
    When(Method(mockFs, writeFile).Using(_, _)).AlwaysReturn(true);
    
    SimpleFileProcessor processor(&mockFs.get(), nullptr, &mockLogger.get());
    
    // Process multiple files
    processor.processFile("file1.txt");
    processor.processFile("file2.txt");
    processor.processFile("file3.txt");
    
    // Verify total invocations
    Verify(Method(mockLogger, info).Using(_)).Exactly(3_Times);
    Verify(Method(mockFs, fileExists).Using(_)).Exactly(3_Times);
    Verify(Method(mockFs, readFile).Using(_)).Exactly(3_Times);
    Verify(Method(mockFs, writeFile).Using(_, _)).Exactly(3_Times);
}