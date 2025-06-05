# SQL in C Programming Cheat Sheet

## Overview
SQL (Structured Query Language) is used to manage relational databases. In C programming, we interact with SQL databases using APIs like SQLite3, MySQL C API, or PostgreSQL's libpq.

## SQLite3 in C

### Why SQLite3?
- Serverless, zero-configuration
- Self-contained, single file database
- Cross-platform
- Perfect for embedded systems and applications

### Basic SQLite3 Setup
```c
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

// Basic connection
sqlite3 *db;
int rc = sqlite3_open("test.db", &db);

if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
}

// Always close when done
sqlite3_close(db);
```

### Error Handling
```c
void handle_error(sqlite3 *db, const char *msg) {
    fprintf(stderr, "%s: %s\n", msg, sqlite3_errmsg(db));
    sqlite3_close(db);
    exit(1);
}
```

## Creating Tables

### Basic CREATE TABLE
```c
const char *create_table_sql = 
    "CREATE TABLE IF NOT EXISTS users ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "name TEXT NOT NULL,"
    "email TEXT UNIQUE,"
    "age INTEGER,"
    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
    ");";

char *err_msg = NULL;
int rc = sqlite3_exec(db, create_table_sql, 0, 0, &err_msg);

if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
}
```

### Common Data Types
```sql
-- SQLite data types
INTEGER    -- Signed integer
REAL       -- Floating point
TEXT       -- UTF-8 string
BLOB       -- Binary data
NULL       -- NULL value

-- Common column constraints
PRIMARY KEY
AUTOINCREMENT
NOT NULL
UNIQUE
DEFAULT value
CHECK(condition)
FOREIGN KEY
```

## Executing SQL Statements

### 1. Using sqlite3_exec() for Simple Queries
```c
// For queries that don't return data (CREATE, INSERT, UPDATE, DELETE)
int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

const char *sql = "SELECT * FROM users;";
char *err_msg = NULL;

int rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
```

### 2. Using Prepared Statements (Recommended)
```c
// Prepare statement
sqlite3_stmt *stmt;
const char *sql = "INSERT INTO users (name, email, age) VALUES (?, ?, ?);";

rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
if (rc != SQLITE_OK) {
    handle_error(db, "Failed to prepare statement");
}

// Bind parameters (1-indexed)
sqlite3_bind_text(stmt, 1, "John Doe", -1, SQLITE_STATIC);
sqlite3_bind_text(stmt, 2, "john@example.com", -1, SQLITE_STATIC);
sqlite3_bind_int(stmt, 3, 25);

// Execute
rc = sqlite3_step(stmt);
if (rc != SQLITE_DONE) {
    handle_error(db, "Execution failed");
}

// Clean up
sqlite3_finalize(stmt);
```

## CRUD Operations

### CREATE (Insert)
```c
int insert_user(sqlite3 *db, const char *name, const char *email, int age) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO users (name, email, age) VALUES (?, ?, ?);";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }
    
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, age);
    
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        return -1;
    }
    
    return sqlite3_last_insert_rowid(db);
}
```

### READ (Select)
```c
typedef struct {
    int id;
    char name[100];
    char email[100];
    int age;
} User;

int select_all_users(sqlite3 *db, User **users, int *count) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, name, email, age FROM users;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }
    
    // Count results first
    *count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        (*count)++;
    }
    
    // Allocate memory
    *users = malloc(*count * sizeof(User));
    if (*users == NULL) {
        sqlite3_finalize(stmt);
        return -1;
    }
    
    // Reset and fetch data
    sqlite3_reset(stmt);
    int i = 0;
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        (*users)[i].id = sqlite3_column_int(stmt, 0);
        
        const char *name = (const char*)sqlite3_column_text(stmt, 1);
        strncpy((*users)[i].name, name, 99);
        (*users)[i].name[99] = '\0';
        
        const char *email = (const char*)sqlite3_column_text(stmt, 2);
        strncpy((*users)[i].email, email, 99);
        (*users)[i].email[99] = '\0';
        
        (*users)[i].age = sqlite3_column_int(stmt, 3);
        i++;
    }
    
    sqlite3_finalize(stmt);
    return 0;
}
```

### UPDATE
```c
int update_user_email(sqlite3 *db, int user_id, const char *new_email) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE users SET email = ? WHERE id = ?;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }
    
    sqlite3_bind_text(stmt, 1, new_email, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, user_id);
    
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        return -1;
    }
    
    return sqlite3_changes(db);  // Number of rows affected
}
```

### DELETE
```c
int delete_user(sqlite3 *db, int user_id) {
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM users WHERE id = ?;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }
    
    sqlite3_bind_int(stmt, 1, user_id);
    
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        return -1;
    }
    
    return sqlite3_changes(db);
}
```

## Transactions

### Basic Transaction Pattern
```c
int perform_transaction(sqlite3 *db) {
    char *err_msg = NULL;
    
    // Begin transaction
    if (sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Failed to begin transaction: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    
    // Perform multiple operations
    if (insert_user(db, "User1", "user1@email.com", 20) < 0) {
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        return -1;
    }
    
    if (insert_user(db, "User2", "user2@email.com", 25) < 0) {
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        return -1;
    }
    
    // Commit transaction
    if (sqlite3_exec(db, "COMMIT;", NULL, NULL, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Failed to commit: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        return -1;
    }
    
    return 0;
}
```

## Advanced Queries

### JOIN Operations
```c
const char *join_sql = 
    "SELECT u.name, o.order_date, o.total "
    "FROM users u "
    "INNER JOIN orders o ON u.id = o.user_id "
    "WHERE o.total > ?;";

sqlite3_stmt *stmt;
sqlite3_prepare_v2(db, join_sql, -1, &stmt, NULL);
sqlite3_bind_double(stmt, 1, 100.0);

while (sqlite3_step(stmt) == SQLITE_ROW) {
    const char *name = (const char*)sqlite3_column_text(stmt, 0);
    const char *date = (const char*)sqlite3_column_text(stmt, 1);
    double total = sqlite3_column_double(stmt, 2);
    
    printf("%s - %s - $%.2f\n", name, date, total);
}

sqlite3_finalize(stmt);
```

### Aggregate Functions
```c
// Count users
int count_users(sqlite3 *db) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM users;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }
    
    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    return count;
}

// Other aggregate examples
const char *aggregates[] = {
    "SELECT AVG(age) FROM users;",
    "SELECT MIN(age), MAX(age) FROM users;",
    "SELECT SUM(total) FROM orders WHERE user_id = ?;",
    "SELECT COUNT(DISTINCT email) FROM users;"
};
```

## Security Best Practices

### 1. Always Use Prepared Statements
```c
// BAD - SQL Injection vulnerable
char sql[256];
sprintf(sql, "SELECT * FROM users WHERE name = '%s';", user_input);
sqlite3_exec(db, sql, callback, 0, &err_msg);  // DANGEROUS!

// GOOD - Safe from SQL injection
const char *sql = "SELECT * FROM users WHERE name = ?;";
sqlite3_stmt *stmt;
sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
sqlite3_bind_text(stmt, 1, user_input, -1, SQLITE_STATIC);
```

### 2. Input Validation
```c
int validate_email(const char *email) {
    // Simple email validation
    const char *at = strchr(email, '@');
    if (!at || at == email) return 0;
    
    const char *dot = strchr(at, '.');
    if (!dot || dot == at + 1) return 0;
    
    return 1;
}

int safe_insert_user(sqlite3 *db, const char *name, const char *email, int age) {
    // Validate inputs
    if (strlen(name) > 50 || strlen(email) > 100) {
        return -1;
    }
    
    if (!validate_email(email)) {
        return -1;
    }
    
    if (age < 0 || age > 150) {
        return -1;
    }
    
    return insert_user(db, name, email, age);
}
```

## Database Schema Management

### Creating Indexes
```c
const char *create_index_sql[] = {
    "CREATE INDEX idx_users_email ON users(email);",
    "CREATE INDEX idx_users_age ON users(age);",
    "CREATE UNIQUE INDEX idx_users_username ON users(username);"
};

for (int i = 0; i < 3; i++) {
    sqlite3_exec(db, create_index_sql[i], NULL, NULL, &err_msg);
}
```

### Foreign Key Constraints
```c
// Enable foreign keys (disabled by default in SQLite)
sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL);

const char *create_orders_table = 
    "CREATE TABLE orders ("
    "id INTEGER PRIMARY KEY,"
    "user_id INTEGER NOT NULL,"
    "total REAL NOT NULL,"
    "FOREIGN KEY (user_id) REFERENCES users(id)"
    "   ON DELETE CASCADE"
    "   ON UPDATE CASCADE"
    ");";
```

## Memory Management

### Proper Cleanup Pattern
```c
typedef struct {
    sqlite3 *db;
    sqlite3_stmt *stmt;
} DBContext;

void cleanup_db(DBContext *ctx) {
    if (ctx->stmt) {
        sqlite3_finalize(ctx->stmt);
        ctx->stmt = NULL;
    }
    
    if (ctx->db) {
        sqlite3_close(ctx->db);
        ctx->db = NULL;
    }
}

// Usage with goto for cleanup
int database_operation() {
    DBContext ctx = {0};
    int result = -1;
    
    if (sqlite3_open("test.db", &ctx.db) != SQLITE_OK) {
        goto cleanup;
    }
    
    if (sqlite3_prepare_v2(ctx.db, sql, -1, &ctx.stmt, NULL) != SQLITE_OK) {
        goto cleanup;
    }
    
    // Operations...
    result = 0;
    
cleanup:
    cleanup_db(&ctx);
    return result;
}
```

## Complete Example: User Management System
```c
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    sqlite3 *db;
    sqlite3_stmt *insert_stmt;
    sqlite3_stmt *select_stmt;
    sqlite3_stmt *update_stmt;
    sqlite3_stmt *delete_stmt;
} UserDB;

int init_user_db(UserDB *udb, const char *db_path) {
    // Open database
    if (sqlite3_open(db_path, &udb->db) != SQLITE_OK) {
        return -1;
    }
    
    // Create table
    const char *create_sql = 
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE NOT NULL,"
        "email TEXT UNIQUE NOT NULL,"
        "password_hash TEXT NOT NULL,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";
    
    char *err_msg;
    if (sqlite3_exec(udb->db, create_sql, NULL, NULL, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(udb->db);
        return -1;
    }
    
    // Prepare statements
    const char *insert_sql = "INSERT INTO users (username, email, password_hash) VALUES (?, ?, ?);";
    sqlite3_prepare_v2(udb->db, insert_sql, -1, &udb->insert_stmt, NULL);
    
    const char *select_sql = "SELECT id, username, email FROM users WHERE username = ?;";
    sqlite3_prepare_v2(udb->db, select_sql, -1, &udb->select_stmt, NULL);
    
    return 0;
}

void close_user_db(UserDB *udb) {
    if (udb->insert_stmt) sqlite3_finalize(udb->insert_stmt);
    if (udb->select_stmt) sqlite3_finalize(udb->select_stmt);
    if (udb->update_stmt) sqlite3_finalize(udb->update_stmt);
    if (udb->delete_stmt) sqlite3_finalize(udb->delete_stmt);
    if (udb->db) sqlite3_close(udb->db);
}
```

## Common SQL Patterns

### Pagination
```c
int get_users_page(sqlite3 *db, int page, int per_page, User **users) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM users LIMIT ? OFFSET ?;";
    
    int offset = (page - 1) * per_page;
    
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, per_page);
    sqlite3_bind_int(stmt, 2, offset);
    
    // Fetch results...
}
```

### Search with LIKE
```c
int search_users(sqlite3 *db, const char *search_term) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM users WHERE name LIKE ? OR email LIKE ?;";
    
    char pattern[256];
    snprintf(pattern, sizeof(pattern), "%%%s%%", search_term);
    
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, pattern, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pattern, -1, SQLITE_STATIC);
    
    // Process results...
}
```

## Performance Tips

1. **Use indexes** for frequently queried columns
2. **Batch operations** in transactions
3. **Prepare statements** once, reuse many times
4. **Use appropriate data types**
5. **VACUUM** periodically to reclaim space
6. **Use EXPLAIN QUERY PLAN** to analyze queries

## Error Codes
```c
// Common SQLite return codes
SQLITE_OK          // Successful result
SQLITE_ERROR       // Generic error
SQLITE_INTERNAL    // Internal logic error
SQLITE_PERM        // Access permission denied
SQLITE_ABORT       // Callback routine requested abort
SQLITE_BUSY        // Database file is locked
SQLITE_LOCKED      // Table in database is locked
SQLITE_NOMEM       // malloc() failed
SQLITE_READONLY    // Attempt to write readonly database
SQLITE_INTERRUPT   // Operation terminated by interrupt
SQLITE_IOERR       // Some kind of disk I/O error
SQLITE_CONSTRAINT  // Abort due to constraint violation
SQLITE_MISMATCH    // Data type mismatch
SQLITE_MISUSE      // Library used incorrectly
```

## Quick Reference

| Operation | Function | Purpose |
|-----------|----------|---------|
| Open DB | `sqlite3_open()` | Open database connection |
| Close DB | `sqlite3_close()` | Close database connection |
| Execute | `sqlite3_exec()` | Execute SQL without results |
| Prepare | `sqlite3_prepare_v2()` | Compile SQL statement |
| Bind | `sqlite3_bind_*()` | Bind parameters to statement |
| Step | `sqlite3_step()` | Execute prepared statement |
| Column | `sqlite3_column_*()` | Retrieve column values |
| Finalize | `sqlite3_finalize()` | Destroy prepared statement |

## Exam Tips
1. Always check return codes from SQLite functions
2. Use prepared statements to prevent SQL injection
3. Remember to finalize statements and close databases
4. Bind parameters are 1-indexed, not 0-indexed
5. Use transactions for multiple related operations
6. Handle NULL values appropriately
7. Free error messages with `sqlite3_free()`
8. Enable foreign keys explicitly in SQLite