// Feature flags - set to 1 to enable, 0 to disable
#ifndef FEATURE_LOGGING
#define FEATURE_LOGGING 1
#endif

#ifndef FEATURE_VALIDATION
#define FEATURE_VALIDATION 1
#endif

#ifndef FEATURE_CACHING
#define FEATURE_CACHING 1
#endif

#ifndef FEATURE_METRICS
#define FEATURE_METRICS 1
#endif

#include <iostream>
#include <string>
#include <memory>
#include <chrono>
#include <unordered_map>

// Base Application interface
class Application {
public:
    virtual ~Application() = default;
    virtual void run(const std::string& input) = 0;
    virtual std::string getName() const = 0;
};

// Core Application implementation
class CoreApplication : public Application {
public:
    void run(const std::string& input) override {
        std::cout << "  [Core] Processing: " << input << std::endl;
        result_ = "Result for: " + input;
    }
    
    std::string getName() const override {
        return "CoreApplication";
    }
    
    std::string getResult() const { return result_; }
    
private:
    std::string result_;
};

// Base Decorator
class ApplicationDecorator : public Application {
protected:
    std::unique_ptr<Application> app_;
    
public:
    explicit ApplicationDecorator(std::unique_ptr<Application> app)
        : app_(std::move(app)) {}
    
    std::string getName() const override {
        return app_->getName();
    }
};

#if FEATURE_LOGGING == 1
// Logging Feature
class LoggingDecorator : public ApplicationDecorator {
public:
    explicit LoggingDecorator(std::unique_ptr<Application> app)
        : ApplicationDecorator(std::move(app)) {}
    
    void run(const std::string& input) override {
        std::cout << "[Logging] Starting execution with input: " << input << std::endl;
        app_->run(input);
        std::cout << "[Logging] Execution completed" << std::endl;
    }
    
    std::string getName() const override {
        return app_->getName() + " + Logging";
    }
};
#endif

#if FEATURE_VALIDATION == 1
// Validation Feature
class ValidationDecorator : public ApplicationDecorator {
public:
    explicit ValidationDecorator(std::unique_ptr<Application> app)
        : ApplicationDecorator(std::move(app)) {}
    
    void run(const std::string& input) override {
        std::cout << "[Validation] Checking input..." << std::endl;
        if (input.empty()) {
            std::cout << "[Validation] ERROR: Empty input!" << std::endl;
            return;
        }
        std::cout << "[Validation] Input is valid" << std::endl;
        app_->run(input);
    }
    
    std::string getName() const override {
        return app_->getName() + " + Validation";
    }
};
#endif

#if FEATURE_CACHING == 1
// Caching Feature
class CachingDecorator : public ApplicationDecorator {
private:
    std::unordered_map<std::string, bool> cache_;
    
public:
    explicit CachingDecorator(std::unique_ptr<Application> app)
        : ApplicationDecorator(std::move(app)) {}
    
    void run(const std::string& input) override {
        if (cache_.find(input) != cache_.end()) {
            std::cout << "[Cache] HIT - Using cached result for: " << input << std::endl;
            return;
        }
        std::cout << "[Cache] MISS - Processing and caching: " << input << std::endl;
        app_->run(input);
        cache_[input] = true;
    }
    
    std::string getName() const override {
        return app_->getName() + " + Caching";
    }
};
#endif

#if FEATURE_METRICS == 1
// Metrics/Timing Feature
class MetricsDecorator : public ApplicationDecorator {
private:
    int executionCount_ = 0;
    
public:
    explicit MetricsDecorator(std::unique_ptr<Application> app)
        : ApplicationDecorator(std::move(app)) {}
    
    void run(const std::string& input) override {
        auto start = std::chrono::high_resolution_clock::now();
        
        app_->run(input);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        executionCount_++;
        
        std::cout << "[Metrics] Execution #" << executionCount_ 
                  << " took " << duration.count() << " microseconds" << std::endl;
    }
    
    std::string getName() const override {
        return app_->getName() + " + Metrics";
    }
};
#endif

// Factory function to build the decorated application
std::unique_ptr<Application> createApplication() {
    std::unique_ptr<Application> app = std::make_unique<CoreApplication>();
    
    // Wrap with features in reverse order (innermost to outermost)
#if FEATURE_METRICS == 1
    app = std::make_unique<MetricsDecorator>(std::move(app));
#endif

#if FEATURE_CACHING == 1
    app = std::make_unique<CachingDecorator>(std::move(app));
#endif

#if FEATURE_VALIDATION == 1
    app = std::make_unique<ValidationDecorator>(std::move(app));
#endif

#if FEATURE_LOGGING == 1
    app = std::make_unique<LoggingDecorator>(std::move(app));
#endif

    return app;
}

int main() {
    std::cout << "=== Compile-Time Feature Decorator Pattern ===" << std::endl;
    std::cout << std::endl;
    
    auto app = createApplication();
    
    std::cout << "Active Configuration: " << app->getName() << std::endl;
    std::cout << std::endl;
    
    std::cout << "Enabled Features:" << std::endl;
#if FEATURE_LOGGING == 1
    std::cout << "  ✓ Logging" << std::endl;
#else
    std::cout << "  ✗ Logging" << std::endl;
#endif

#if FEATURE_VALIDATION == 1
    std::cout << "  ✓ Validation" << std::endl;
#else
    std::cout << "  ✗ Validation" << std::endl;
#endif

#if FEATURE_CACHING == 1
    std::cout << "  ✓ Caching" << std::endl;
#else
    std::cout << "  ✗ Caching" << std::endl;
#endif

#if FEATURE_METRICS == 1
    std::cout << "  ✓ Metrics" << std::endl;
#else
    std::cout << "  ✗ Metrics" << std::endl;
#endif

    std::cout << std::endl;
    std::cout << "--- Test Run 1 ---" << std::endl;
    app->run("Hello World");
    
    std::cout << std::endl;
    std::cout << "--- Test Run 2 (same input for cache test) ---" << std::endl;
    app->run("Hello World");
    
    std::cout << std::endl;
    std::cout << "--- Test Run 3 (empty input for validation test) ---" << std::endl;
    app->run("");
    
    std::cout << std::endl;
    std::cout << "--- Test Run 4 (different input) ---" << std::endl;
    app->run("Goodbye World");
    
    return 0;
}