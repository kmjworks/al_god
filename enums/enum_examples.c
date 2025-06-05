#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Basic enum examples
enum Color {
    RED,
    GREEN,
    BLUE
};

// Enum with custom values
typedef enum {
    SUCCESS = 0,
    ERROR_FILE_NOT_FOUND = -1,
    ERROR_PERMISSION_DENIED = -2,
    ERROR_OUT_OF_MEMORY = -3
} ErrorCode;

// Bit flags enum
typedef enum {
    PERM_NONE = 0,
    PERM_READ = 1 << 0,    // 0001
    PERM_WRITE = 1 << 1,   // 0010
    PERM_EXECUTE = 1 << 2, // 0100
    PERM_DELETE = 1 << 3   // 1000
} Permission;

// State machine enum
typedef enum {
    STATE_INIT,
    STATE_LOADING,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
} ProcessState;

// Days of week with string mapping
typedef enum {
    SUNDAY,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY
} Weekday;

const char* weekdayNames[] = {
    "Sunday", "Monday", "Tuesday", "Wednesday", 
    "Thursday", "Friday", "Saturday"
};

// Menu options
typedef enum {
    MENU_NEW = 1,
    MENU_OPEN,
    MENU_SAVE,
    MENU_PRINT,
    MENU_EXIT
} MenuOption;

// Log levels
typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
} LogLevel;

const char* logLevelNames[] = {
    "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"
};

// Basic enum usage
void demonstrateBasicEnum() {
    printf("\n=== Basic Enum Demo ===\n");
    enum Color myColor = GREEN;
    
    printf("Color values: RED=%d, GREEN=%d, BLUE=%d\n", RED, GREEN, BLUE);
    printf("My color: %d\n", myColor);
    
    switch (myColor) {
        case RED:
            printf("The color is red\n");
            break;
        case GREEN:
            printf("The color is green\n");
            break;
        case BLUE:
            printf("The color is blue\n");
            break;
    }
}

// Error handling with enums
ErrorCode performOperation(int op) {
    if (op == 0) return SUCCESS;
    if (op == 1) return ERROR_FILE_NOT_FOUND;
    if (op == 2) return ERROR_PERMISSION_DENIED;
    return ERROR_OUT_OF_MEMORY;
}

void demonstrateErrorHandling() {
    printf("\n=== Error Handling Demo ===\n");
    
    for (int i = 0; i < 4; i++) {
        ErrorCode result = performOperation(i);
        printf("Operation %d: ", i);
        
        switch (result) {
            case SUCCESS:
                printf("Success!\n");
                break;
            case ERROR_FILE_NOT_FOUND:
                printf("Error: File not found\n");
                break;
            case ERROR_PERMISSION_DENIED:
                printf("Error: Permission denied\n");
                break;
            case ERROR_OUT_OF_MEMORY:
                printf("Error: Out of memory\n");
                break;
        }
    }
}

// Bit flags demonstration
void demonstrateBitFlags() {
    printf("\n=== Bit Flags Demo ===\n");
    
    Permission filePerms = PERM_READ | PERM_WRITE;
    printf("Initial permissions: %d\n", filePerms);
    
    // Check permissions
    printf("Can read: %s\n", (filePerms & PERM_READ) ? "Yes" : "No");
    printf("Can write: %s\n", (filePerms & PERM_WRITE) ? "Yes" : "No");
    printf("Can execute: %s\n", (filePerms & PERM_EXECUTE) ? "Yes" : "No");
    
    // Add execute permission
    filePerms |= PERM_EXECUTE;
    printf("\nAfter adding execute:\n");
    printf("Permissions: %d\n", filePerms);
    printf("Can execute: %s\n", (filePerms & PERM_EXECUTE) ? "Yes" : "No");
    
    // Remove write permission
    filePerms &= ~PERM_WRITE;
    printf("\nAfter removing write:\n");
    printf("Can write: %s\n", (filePerms & PERM_WRITE) ? "Yes" : "No");
    
    // Check multiple permissions
    Permission requiredPerms = PERM_READ | PERM_EXECUTE;
    if ((filePerms & requiredPerms) == requiredPerms) {
        printf("\nFile has all required permissions\n");
    }
}

// State machine demonstration
typedef struct {
    ProcessState state;
    int progress;
} Process;

void updateProcess(Process *proc) {
    switch (proc->state) {
        case STATE_INIT:
            printf("Initializing...\n");
            proc->state = STATE_LOADING;
            break;
        case STATE_LOADING:
            printf("Loading resources...\n");
            proc->state = STATE_READY;
            break;
        case STATE_READY:
            printf("Ready to process\n");
            proc->state = STATE_PROCESSING;
            proc->progress = 0;
            break;
        case STATE_PROCESSING:
            proc->progress += 25;
            printf("Processing... %d%%\n", proc->progress);
            if (proc->progress >= 100) {
                proc->state = STATE_COMPLETE;
            }
            break;
        case STATE_COMPLETE:
            printf("Process complete!\n");
            break;
        case STATE_ERROR:
            printf("Error occurred\n");
            break;
    }
}

void demonstrateStateMachine() {
    printf("\n=== State Machine Demo ===\n");
    Process proc = { STATE_INIT, 0 };
    
    // Run through states
    for (int i = 0; i < 8; i++) {
        updateProcess(&proc);
        if (proc.state == STATE_COMPLETE) break;
    }
}

// Weekday functions
const char* getWeekdayName(Weekday day) {
    if (day >= SUNDAY && day <= SATURDAY) {
        return weekdayNames[day];
    }
    return "Invalid day";
}

Weekday getNextDay(Weekday day) {
    return (Weekday)((day + 1) % 7);
}

void demonstrateWeekdays() {
    printf("\n=== Weekday Demo ===\n");
    
    Weekday today = WEDNESDAY;
    printf("Today is %s\n", getWeekdayName(today));
    
    printf("Next 7 days:\n");
    for (int i = 0; i < 7; i++) {
        today = getNextDay(today);
        printf("  %s\n", getWeekdayName(today));
    }
}

// Menu demonstration
void displayMenu() {
    printf("\n1. New File\n");
    printf("2. Open File\n");
    printf("3. Save File\n");
    printf("4. Print\n");
    printf("5. Exit\n");
    printf("Enter choice: ");
}

void handleMenuChoice(MenuOption choice) {
    switch (choice) {
        case MENU_NEW:
            printf("Creating new file...\n");
            break;
        case MENU_OPEN:
            printf("Opening file...\n");
            break;
        case MENU_SAVE:
            printf("Saving file...\n");
            break;
        case MENU_PRINT:
            printf("Printing...\n");
            break;
        case MENU_EXIT:
            printf("Exiting program...\n");
            break;
        default:
            printf("Invalid choice\n");
    }
}

void demonstrateMenu() {
    printf("\n=== Menu Demo ===\n");
    displayMenu();
    
    // Simulate user choices
    int choices[] = {1, 3, 5};
    for (int i = 0; i < 3; i++) {
        printf("\nSimulating choice: %d\n", choices[i]);
        handleMenuChoice((MenuOption)choices[i]);
    }
}

// Logging demonstration
void log_message(LogLevel level, const char* message) {
    printf("[%s] %s\n", logLevelNames[level], message);
}

void demonstrateLogging() {
    printf("\n=== Logging Demo ===\n");
    
    log_message(LOG_DEBUG, "Debug information");
    log_message(LOG_INFO, "Application started");
    log_message(LOG_WARNING, "Low memory warning");
    log_message(LOG_ERROR, "Failed to open file");
    log_message(LOG_CRITICAL, "System failure!");
    
    // Log filtering
    LogLevel minLevel = LOG_WARNING;
    printf("\nFiltered logs (WARNING and above):\n");
    
    LogLevel testLevels[] = {LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR};
    const char* testMessages[] = {
        "This won't show",
        "Neither will this",
        "This warning will show",
        "This error will show"
    };
    
    for (int i = 0; i < 4; i++) {
        if (testLevels[i] >= minLevel) {
            log_message(testLevels[i], testMessages[i]);
        }
    }
}

// Configuration flags
typedef enum {
    CONFIG_NONE = 0,
    CONFIG_DEBUG = 1 << 0,
    CONFIG_VERBOSE = 1 << 1,
    CONFIG_AUTOSAVE = 1 << 2,
    CONFIG_DARKMODE = 1 << 3
} ConfigFlag;

typedef struct {
    uint32_t flags;
} Configuration;

void printConfiguration(Configuration *config) {
    printf("Configuration:\n");
    printf("  Debug mode: %s\n", (config->flags & CONFIG_DEBUG) ? "ON" : "OFF");
    printf("  Verbose: %s\n", (config->flags & CONFIG_VERBOSE) ? "ON" : "OFF");
    printf("  Autosave: %s\n", (config->flags & CONFIG_AUTOSAVE) ? "ON" : "OFF");
    printf("  Dark mode: %s\n", (config->flags & CONFIG_DARKMODE) ? "ON" : "OFF");
}

void demonstrateConfiguration() {
    printf("\n=== Configuration Demo ===\n");
    
    Configuration config = { CONFIG_NONE };
    
    // Set some flags
    config.flags |= CONFIG_DEBUG | CONFIG_DARKMODE;
    
    printf("Initial configuration:\n");
    printConfiguration(&config);
    
    // Toggle autosave
    config.flags ^= CONFIG_AUTOSAVE;
    printf("\nAfter toggling autosave:\n");
    printConfiguration(&config);
    
    // Clear all flags
    config.flags = CONFIG_NONE;
    printf("\nAfter clearing all:\n");
    printConfiguration(&config);
}

// Size demonstration
void demonstrateEnumSize() {
    printf("\n=== Enum Size Demo ===\n");
    
    printf("Size of Color enum: %zu bytes\n", sizeof(enum Color));
    printf("Size of ErrorCode: %zu bytes\n", sizeof(ErrorCode));
    printf("Size of Permission: %zu bytes\n", sizeof(Permission));
    printf("Size of individual enum value: %zu bytes\n", sizeof(RED));
    
    // Enum storage
    enum Color colors[10];
    printf("Array of 10 Color enums: %zu bytes\n", sizeof(colors));
}

int main() {
    printf("ENUM COMPREHENSIVE EXAMPLES\n");
    printf("===========================\n");
    
    demonstrateBasicEnum();
    demonstrateErrorHandling();
    demonstrateBitFlags();
    demonstrateStateMachine();
    demonstrateWeekdays();
    demonstrateMenu();
    demonstrateLogging();
    demonstrateConfiguration();
    demonstrateEnumSize();
    
    return 0;
}