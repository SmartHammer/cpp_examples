# Compile-Time Feature Decorator

This application demonstrates a compile-time feature decorator pattern where features can be enabled or disabled at compile time using preprocessor directives.

## Available Features

The application supports the following decorators:

1. **FEATURE_LOGGING** - Logs execution start and completion
2. **FEATURE_VALIDATION** - Validates input before processing
3. **FEATURE_CACHING** - Caches results to avoid reprocessing
4. **FEATURE_METRICS** - Tracks execution time and call count

## How to Enable/Disable Features

### Method 1: Modify the source file

Edit the `#define` directives at the top of [src/main.cpp](src/main.cpp):

```cpp
#define FEATURE_LOGGING 1     // Set to 0 to disable
#define FEATURE_VALIDATION 1  // Set to 0 to disable
#define FEATURE_CACHING 1     // Set to 0 to disable
#define FEATURE_METRICS 1     // Set to 0 to disable
```

### Method 2: Use compiler flags

Pass defines during compilation without modifying the source:

```bash
# Disable all features
cd build
cmake .. -DCMAKE_CXX_FLAGS="-DFEATURE_LOGGING=0 -DFEATURE_VALIDATION=0 -DFEATURE_CACHING=0 -DFEATURE_METRICS=0"
cmake --build .
./c++-examples
```

```bash
# Enable only logging and metrics
cd build
cmake .. -DCMAKE_CXX_FLAGS="-DFEATURE_LOGGING=1 -DFEATURE_VALIDATION=0 -DFEATURE_CACHING=0 -DFEATURE_METRICS=1"
cmake --build .
./c++-examples
```

```bash
# Enable only validation
cd build
cmake .. -DCMAKE_CXX_FLAGS="-DFEATURE_LOGGING=0 -DFEATURE_VALIDATION=1 -DFEATURE_CACHING=0 -DFEATURE_METRICS=0"
cmake --build .
./c++-examples
```

## Example Outputs

### All Features Enabled
```
Active Configuration: CoreApplication + Metrics + Caching + Validation + Logging
[Logging] Starting execution with input: Hello World
[Validation] Checking input...
[Validation] Input is valid
[Cache] MISS - Processing and caching: Hello World
  [Core] Processing: Hello World
[Metrics] Execution #1 took 2 microseconds
[Logging] Execution completed
```

### Only Core Application (No Features)
```
Active Configuration: CoreApplication
  [Core] Processing: Hello World
```

### Only Validation
```
Active Configuration: CoreApplication + Validation
[Validation] Checking input...
[Validation] Input is valid
  [Core] Processing: Hello World
```

## How It Works

The pattern uses:
- **Preprocessor directives** (`#if`, `#define`) to conditionally compile code
- **Decorator pattern** to wrap the core application with additional behaviors
- **Factory function** (`createApplication()`) to assemble the decorated application based on enabled features

When a feature is disabled (set to 0), its entire decorator class is excluded from compilation, resulting in:
- **Zero runtime overhead** - No virtual function calls for disabled features
- **Smaller binary size** - Unused code is not included
- **Compile-time configuration** - No runtime checks needed

## Adding New Features

To add a new feature decorator:

1. Add a `#define` for your feature flag
2. Create a decorator class inside `#if FEATURE_NAME == 1`
3. Add it to the factory function `createApplication()`
4. Update the feature list in `main()` to display its status

Example:
```cpp
#define FEATURE_ENCRYPTION 1

#if FEATURE_ENCRYPTION == 1
class EncryptionDecorator : public ApplicationDecorator {
    // Implementation
};
#endif
```
