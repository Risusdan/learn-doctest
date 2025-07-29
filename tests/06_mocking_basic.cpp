#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest.h"
#include "../fakeit.hpp"
#include "../src/interfaces.hpp"

using namespace fakeit;

// Simple test classes that don't use unique_ptr for easier mocking
class SimpleUserService {
private:
    IDatabase* database;
    ILogger* logger;
    
public:
    SimpleUserService(IDatabase* db, ILogger* log) : database(db), logger(log) {}
    
    bool saveUser(const std::string& id, const std::string& data) {
        if (database->exists(id)) {
            logger->warning("User already exists: " + id);
            return false;
        }
        
        bool success = database->save(id, data);
        if (success) {
            logger->info("User saved successfully: " + id);
        } else {
            logger->error("Failed to save user: " + id);
        }
        return success;
    }
    
    std::string getUser(const std::string& id) {
        if (!database->exists(id)) {
            logger->warning("User not found: " + id);
            return "";
        }
        
        std::string data = database->load(id);
        logger->info("User retrieved: " + id);
        return data;
    }
};

TEST_CASE("Basic FakeIt mocking") {
    Mock<IDatabase> mockDb;
    Mock<ILogger> mockLogger;
    
    // Set up expectations
    When(Method(mockDb, exists).Using("123")).Return(false);
    When(Method(mockDb, save).Using("123", "John Doe")).Return(true);
    When(Method(mockLogger, info).Using(_)).AlwaysReturn();  // Use wildcard for flexible matching
    
    // Create service with mock pointers
    SimpleUserService service(&mockDb.get(), &mockLogger.get());
    
    // Test the functionality
    bool result = service.saveUser("123", "John Doe");
    
    // Verify results
    CHECK(result == true);
    
    // Verify mock interactions
    Verify(Method(mockDb, exists).Using("123")).Once();
    Verify(Method(mockDb, save).Using("123", "John Doe")).Once();
    // Use wildcard verification for the dynamically constructed string
    Verify(Method(mockLogger, info).Using(_)).Once();
}

TEST_CASE("Mock return values") {
    SUBCASE("User exists") {
        Mock<IDatabase> mockDb;
        Mock<ILogger> mockLogger;
        
        When(Method(mockDb, exists).Using("456")).Return(true);
        When(Method(mockDb, load).Using("456")).Return("Jane Smith");
        When(Method(mockLogger, info).Using(_)).AlwaysReturn();
        
        SimpleUserService service(&mockDb.get(), &mockLogger.get());
        
        std::string result = service.getUser("456");
        CHECK(result == "Jane Smith");
        
        // Verify with wildcard since string is dynamically constructed
        Verify(Method(mockLogger, info).Using(_)).Once();
    }
    
    SUBCASE("User doesn't exist") {
        Mock<IDatabase> mockDb;
        Mock<ILogger> mockLogger;
        
        When(Method(mockDb, exists).Using("999")).Return(false);
        When(Method(mockLogger, warning).Using(_)).AlwaysReturn();
        
        SimpleUserService service(&mockDb.get(), &mockLogger.get());
        
        std::string result = service.getUser("999");
        CHECK(result.empty());
        
        Verify(Method(mockLogger, warning).Using(_)).Once();
        VerifyNoOtherInvocations(Method(mockDb, load));
    }
}

TEST_CASE("Argument matching with wildcards") {
    Mock<IDatabase> mockDb;
    Mock<ILogger> mockLogger;
    
    // Use wildcards for flexible matching
    When(Method(mockDb, exists).Using(_)).AlwaysReturn(false);
    When(Method(mockDb, save).Using(_, _)).AlwaysReturn(true);
    When(Method(mockLogger, info).Using(_)).AlwaysReturn();
    
    SimpleUserService service(&mockDb.get(), &mockLogger.get());
    
    CHECK(service.saveUser("001", "Alice") == true);
    CHECK(service.saveUser("002", "Bob") == true);
    
    // Verify specific calls were made
    Verify(Method(mockDb, save)).Exactly(2_Times);
    Verify(Method(mockLogger, info)).Exactly(2_Times);
}

TEST_CASE("Exception handling in mocks") {
    Mock<IDatabase> mockDb;
    Mock<ILogger> mockLogger;
    
    When(Method(mockDb, exists)).Throw(std::runtime_error("Database connection failed"));
    When(Method(mockLogger, error)).AlwaysReturn();
    
    SimpleUserService service(&mockDb.get(), &mockLogger.get());
    
    CHECK_THROWS_AS(service.saveUser("123", "Test User"), std::runtime_error);
    
    // Even though exception was thrown, we can still verify the call was made
    Verify(Method(mockDb, exists)).Once();
}

TEST_CASE("Sequential return values") {
    Mock<IDatabase> mockDb;
    Mock<ILogger> mockLogger;
    
    // Return different values on successive calls
    When(Method(mockDb, exists).Using("test"))
        .Return(false, true, true);  // First call: false, then true
    
    When(Method(mockDb, save).Using("test", "data")).Return(true);
    When(Method(mockLogger, info).Using(_)).AlwaysReturn();
    When(Method(mockLogger, warning).Using(_)).AlwaysReturn();
    
    SimpleUserService service(&mockDb.get(), &mockLogger.get());
    
    // First call - user doesn't exist, should save
    CHECK(service.saveUser("test", "data") == true);
    
    // Second call - user now exists, should fail
    CHECK(service.saveUser("test", "data") == false);
    
    // Third call - user still exists, should fail
    CHECK(service.saveUser("test", "data") == false);
    
    Verify(Method(mockDb, exists).Using("test")).Exactly(3_Times);
    Verify(Method(mockDb, save).Using("test", "data")).Once();  // Only saved once
    Verify(Method(mockLogger, warning).Using(_)).Exactly(2_Times);  // Two "already exists" warnings
}