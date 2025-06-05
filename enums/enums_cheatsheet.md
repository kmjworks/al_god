# Enums Cheat Sheet - C Programming

## What are Enums?
Enumerations (enums) are user-defined data types that consist of named integer constants. They make code more readable and maintainable by giving meaningful names to numeric values.

## Basic Declaration and Usage

### Simple Enum
```c
enum Color {
    RED,    // 0
    GREEN,  // 1
    BLUE    // 2
};

enum Color myColor = GREEN;
```

### Enum with Custom Values
```c
enum Status {
    SUCCESS = 0,
    ERROR = -1,
    PENDING = 1,
    CANCELLED = 2
};

enum HTTPCode {
    OK = 200,
    NOT_FOUND = 404,
    SERVER_ERROR = 500
};
```

### Enum with Incremental Values
```c
enum Priority {
    LOW = 1,     // 1
    MEDIUM,      // 2 (automatically)
    HIGH,        // 3
    CRITICAL = 10 // 10
};
```

## Typedef with Enums
```c
// Method 1: Separate typedef
enum DayOfWeek {
    MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY
};
typedef enum DayOfWeek Day;

// Method 2: Combined typedef
typedef enum {
    JANUARY = 1, FEBRUARY, MARCH, APRIL, MAY, JUNE,
    JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER
} Month;

// Usage
Day today = MONDAY;
Month currentMonth = JANUARY;
```

## Common Patterns

### 1. State Machine with Enums
```c
typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_PAUSED,
    STATE_STOPPED,
    STATE_ERROR
} State;

typedef struct {
    State currentState;
    int data;
} StateMachine;

void processStateMachine(StateMachine *sm) {
    switch (sm->currentState) {
        case STATE_IDLE:
            printf("System is idle\n");
            sm->currentState = STATE_RUNNING;
            break;
        case STATE_RUNNING:
            printf("System is running\n");
            break;
        case STATE_PAUSED:
            printf("System is paused\n");
            break;
        case STATE_STOPPED:
            printf("System stopped\n");
            break;
        case STATE_ERROR:
            printf("Error occurred!\n");
            break;
    }
}
```

### 2. Flags and Bit Manipulation
```c
typedef enum {
    FLAG_NONE = 0,
    FLAG_READ = 1 << 0,    // 0001 = 1
    FLAG_WRITE = 1 << 1,   // 0010 = 2
    FLAG_EXECUTE = 1 << 2, // 0100 = 4
    FLAG_DELETE = 1 << 3   // 1000 = 8
} FilePermission;

// Usage
int permissions = FLAG_READ | FLAG_WRITE;  // 0011 = 3

// Check permission
if (permissions & FLAG_READ) {
    printf("Has read permission\n");
}

// Add permission
permissions |= FLAG_EXECUTE;

// Remove permission
permissions &= ~FLAG_WRITE;
```

### 3. Menu System
```c
typedef enum {
    MENU_FILE,
    MENU_EDIT,
    MENU_VIEW,
    MENU_HELP,
    MENU_EXIT
} MenuItem;

void displayMenu() {
    printf("1. File\n");
    printf("2. Edit\n");
    printf("3. View\n");
    printf("4. Help\n");
    printf("5. Exit\n");
}

void handleMenuChoice(MenuItem choice) {
    switch (choice) {
        case MENU_FILE:
            printf("Opening file menu...\n");
            break;
        case MENU_EDIT:
            printf("Opening edit menu...\n");
            break;
        case MENU_VIEW:
            printf("Opening view menu...\n");
            break;
        case MENU_HELP:
            printf("Displaying help...\n");
            break;
        case MENU_EXIT:
            printf("Exiting...\n");
            break;
    }
}
```

### 4. Error Codes
```c
typedef enum {
    ERR_NONE = 0,
    ERR_FILE_NOT_FOUND = 100,
    ERR_PERMISSION_DENIED = 101,
    ERR_OUT_OF_MEMORY = 200,
    ERR_INVALID_PARAMETER = 300,
    ERR_NETWORK_TIMEOUT = 400
} ErrorCode;

const char* getErrorMessage(ErrorCode err) {
    switch (err) {
        case ERR_NONE:
            return "No error";
        case ERR_FILE_NOT_FOUND:
            return "File not found";
        case ERR_PERMISSION_DENIED:
            return "Permission denied";
        case ERR_OUT_OF_MEMORY:
            return "Out of memory";
        case ERR_INVALID_PARAMETER:
            return "Invalid parameter";
        case ERR_NETWORK_TIMEOUT:
            return "Network timeout";
        default:
            return "Unknown error";
    }
}
```

## Advanced Examples

### 1. Enum with Strings
```c
typedef enum {
    MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY
} Weekday;

const char* weekdayStrings[] = {
    "Monday", "Tuesday", "Wednesday", "Thursday", 
    "Friday", "Saturday", "Sunday"
};

const char* getWeekdayName(Weekday day) {
    if (day >= MONDAY && day <= SUNDAY) {
        return weekdayStrings[day];
    }
    return "Invalid day";
}
```

### 2. Configuration Options
```c
typedef enum {
    CONFIG_DEBUG_MODE    = 0x01,
    CONFIG_VERBOSE_LOG   = 0x02,
    CONFIG_AUTO_SAVE     = 0x04,
    CONFIG_DARK_THEME    = 0x08,
    CONFIG_NOTIFICATIONS = 0x10
} ConfigOption;

typedef struct {
    unsigned int flags;
} Configuration;

void setConfig(Configuration *config, ConfigOption option) {
    config->flags |= option;
}

void unsetConfig(Configuration *config, ConfigOption option) {
    config->flags &= ~option;
}

int isConfigSet(Configuration *config, ConfigOption option) {
    return (config->flags & option) != 0;
}
```

### 3. Command Parser
```c
typedef enum {
    CMD_HELP,
    CMD_LIST,
    CMD_ADD,
    CMD_DELETE,
    CMD_UPDATE,
    CMD_QUIT,
    CMD_UNKNOWN
} Command;

Command parseCommand(const char *str) {
    if (strcmp(str, "help") == 0) return CMD_HELP;
    if (strcmp(str, "list") == 0) return CMD_LIST;
    if (strcmp(str, "add") == 0) return CMD_ADD;
    if (strcmp(str, "delete") == 0) return CMD_DELETE;
    if (strcmp(str, "update") == 0) return CMD_UPDATE;
    if (strcmp(str, "quit") == 0) return CMD_QUIT;
    return CMD_UNKNOWN;
}
```

## Size and Memory
```c
#include <stdio.h>

enum SmallEnum { A, B, C };
enum LargeEnum { X = 1000000 };

int main() {
    printf("Size of enum: %zu bytes\n", sizeof(enum SmallEnum));
    printf("Size of enum variable: %zu bytes\n", sizeof(A));
    
    // Enums are typically int-sized (4 bytes on most systems)
    return 0;
}
```

## Best Practices

### 1. Naming Conventions
```c
// Good: Use descriptive prefixes
typedef enum {
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE
} Color;

typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR
} LogLevel;
```

### 2. Always Handle All Cases
```c
void processColor(Color color) {
    switch (color) {
        case COLOR_RED:
            // handle red
            break;
        case COLOR_GREEN:
            // handle green
            break;
        case COLOR_BLUE:
            // handle blue
            break;
        default:
            // Always include default for safety
            printf("Unknown color\n");
            break;
    }
}
```

### 3. Use Enums for Related Constants
```c
// Instead of:
#define MONDAY 0
#define TUESDAY 1
// ... etc

// Use:
typedef enum {
    MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY
} Weekday;
```

## Common Pitfalls

### 1. Enum Values are Integers
```c
enum Status { OK, ERROR };
enum Status s = OK;

// This is valid but not recommended
int x = s;  // x = 0
s = 1;      // s = ERROR

// Better to be explicit
if (s == OK) { /* ... */ }
```

### 2. Scope Issues
```c
// Enum values are in the same namespace
enum First { A, B, C };
enum Second { A, D, E };  // Error! A already defined
```

### 3. Size Assumptions
```c
// Don't assume enum size
enum MyEnum { VALUE = 0x7FFFFFFF };  // May require more than default int

// If size matters, use fixed-width types
typedef uint8_t SmallEnum;
#define SMALL_VALUE_A ((SmallEnum)0)
#define SMALL_VALUE_B ((SmallEnum)1)
```

## Quick Reference
| Feature | Syntax | Example |
|---------|---------|---------|
| Basic enum | `enum Name { values };` | `enum Color { RED, GREEN };` |
| With typedef | `typedef enum { } Name;` | `typedef enum { } Status;` |
| Custom values | `NAME = value` | `SUCCESS = 0` |
| Bit flags | `1 << n` | `FLAG_READ = 1 << 0` |
| Check flag | `var & FLAG` | `if (perm & FLAG_READ)` |
| Set flag | `var \|= FLAG` | `perm \|= FLAG_WRITE` |
| Clear flag | `var &= ~FLAG` | `perm &= ~FLAG_WRITE` |

## Exam Tips
1. Enums start at 0 by default and increment by 1
2. Enum values are constants and cannot be changed
3. Enums are typically int-sized
4. Use enums instead of #define for related constants
5. Always handle all enum cases in switch statements
6. Enum values share the same namespace - avoid duplicates
7. Bit flags with enums are powerful for configuration options