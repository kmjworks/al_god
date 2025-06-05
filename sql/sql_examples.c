#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <time.h>

// User structure
typedef struct {
    int id;
    char username[50];
    char email[100];
    int age;
    char created_at[20];
} User;

// Product structure for e-commerce example
typedef struct {
    int id;
    char name[100];
    double price;
    int stock;
} Product;

// Order structure
typedef struct {
    int id;
    int user_id;
    int product_id;
    int quantity;
    double total;
    char order_date[20];
} Order;

// Error handling helper
void handle_error(sqlite3 *db, const char *msg) {
    fprintf(stderr, "%s: %s\n", msg, sqlite3_errmsg(db));
}

// Database initialization
int init_database(sqlite3 *db) {
    char *err_msg = NULL;
    
    // Create users table
    const char *create_users_sql = 
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE NOT NULL,"
        "email TEXT UNIQUE NOT NULL,"
        "age INTEGER,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";
    
    if (sqlite3_exec(db, create_users_sql, NULL, NULL, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Failed to create users table: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    
    // Create products table
    const char *create_products_sql = 
        "CREATE TABLE IF NOT EXISTS products ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "price REAL NOT NULL,"
        "stock INTEGER DEFAULT 0"
        ");";
    
    if (sqlite3_exec(db, create_products_sql, NULL, NULL, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Failed to create products table: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    
    // Create orders table with foreign keys
    const char *create_orders_sql = 
        "CREATE TABLE IF NOT EXISTS orders ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "user_id INTEGER NOT NULL,"
        "product_id INTEGER NOT NULL,"
        "quantity INTEGER NOT NULL,"
        "total REAL NOT NULL,"
        "order_date DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "FOREIGN KEY (user_id) REFERENCES users(id),"
        "FOREIGN KEY (product_id) REFERENCES products(id)"
        ");";
    
    if (sqlite3_exec(db, create_orders_sql, NULL, NULL, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Failed to create orders table: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    
    // Create indexes
    const char *indexes[] = {
        "CREATE INDEX IF NOT EXISTS idx_users_email ON users(email);",
        "CREATE INDEX IF NOT EXISTS idx_orders_user ON orders(user_id);",
        "CREATE INDEX IF NOT EXISTS idx_orders_product ON orders(product_id);"
    };
    
    for (int i = 0; i < 3; i++) {
        if (sqlite3_exec(db, indexes[i], NULL, NULL, &err_msg) != SQLITE_OK) {
            fprintf(stderr, "Failed to create index: %s\n", err_msg);
            sqlite3_free(err_msg);
        }
    }
    
    printf("Database initialized successfully\n");
    return 0;
}

// Basic CRUD operations demo
void crud_operations_demo(sqlite3 *db) {
    printf("\n=== CRUD Operations Demo ===\n");
    
    // INSERT - Add users
    sqlite3_stmt *stmt;
    const char *insert_sql = "INSERT INTO users (username, email, age) VALUES (?, ?, ?);";
    
    const char *users[][3] = {
        {"john_doe", "john@example.com", "25"},
        {"jane_smith", "jane@example.com", "30"},
        {"bob_johnson", "bob@example.com", "35"}
    };
    
    printf("\n1. INSERT - Adding users:\n");
    for (int i = 0; i < 3; i++) {
        if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, users[i][0], -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, users[i][1], -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 3, atoi(users[i][2]));
            
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                printf("   Added user: %s (ID: %lld)\n", users[i][0], sqlite3_last_insert_rowid(db));
            }
            sqlite3_finalize(stmt);
        }
    }
    
    // SELECT - Read all users
    printf("\n2. SELECT - Reading all users:\n");
    const char *select_sql = "SELECT id, username, email, age, created_at FROM users;";
    
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const char *username = (const char*)sqlite3_column_text(stmt, 1);
            const char *email = (const char*)sqlite3_column_text(stmt, 2);
            int age = sqlite3_column_int(stmt, 3);
            const char *created = (const char*)sqlite3_column_text(stmt, 4);
            
            printf("   ID: %d, Username: %s, Email: %s, Age: %d, Created: %s\n",
                   id, username, email, age, created);
        }
        sqlite3_finalize(stmt);
    }
    
    // UPDATE - Modify user
    printf("\n3. UPDATE - Updating john_doe's age:\n");
    const char *update_sql = "UPDATE users SET age = ? WHERE username = ?;";
    
    if (sqlite3_prepare_v2(db, update_sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, 26);
        sqlite3_bind_text(stmt, 2, "john_doe", -1, SQLITE_STATIC);
        
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            printf("   Updated %d row(s)\n", sqlite3_changes(db));
        }
        sqlite3_finalize(stmt);
    }
    
    // DELETE - Remove user
    printf("\n4. DELETE - Removing bob_johnson:\n");
    const char *delete_sql = "DELETE FROM users WHERE username = ?;";
    
    if (sqlite3_prepare_v2(db, delete_sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, "bob_johnson", -1, SQLITE_STATIC);
        
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            printf("   Deleted %d row(s)\n", sqlite3_changes(db));
        }
        sqlite3_finalize(stmt);
    }
}

// Transaction demo
void transaction_demo(sqlite3 *db) {
    printf("\n=== Transaction Demo ===\n");
    
    // Add products for transaction demo
    const char *products[][3] = {
        {"Laptop", "999.99", "10"},
        {"Mouse", "29.99", "50"},
        {"Keyboard", "79.99", "30"}
    };
    
    printf("Adding products...\n");
    sqlite3_stmt *stmt;
    const char *insert_product = "INSERT INTO products (name, price, stock) VALUES (?, ?, ?);";
    
    for (int i = 0; i < 3; i++) {
        if (sqlite3_prepare_v2(db, insert_product, -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, products[i][0], -1, SQLITE_STATIC);
            sqlite3_bind_double(stmt, 2, atof(products[i][1]));
            sqlite3_bind_int(stmt, 3, atoi(products[i][2]));
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
    
    // Transaction example - Place an order
    printf("\nPlacing order with transaction:\n");
    
    // Begin transaction
    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    
    int user_id = 1;
    int product_id = 1;
    int quantity = 2;
    
    // Check stock
    const char *check_stock = "SELECT stock, price FROM products WHERE id = ?;";
    if (sqlite3_prepare_v2(db, check_stock, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, product_id);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int stock = sqlite3_column_int(stmt, 0);
            double price = sqlite3_column_double(stmt, 1);
            
            if (stock >= quantity) {
                sqlite3_finalize(stmt);
                
                // Update stock
                const char *update_stock = "UPDATE products SET stock = stock - ? WHERE id = ?;";
                if (sqlite3_prepare_v2(db, update_stock, -1, &stmt, NULL) == SQLITE_OK) {
                    sqlite3_bind_int(stmt, 1, quantity);
                    sqlite3_bind_int(stmt, 2, product_id);
                    sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }
                
                // Create order
                const char *create_order = "INSERT INTO orders (user_id, product_id, quantity, total) VALUES (?, ?, ?, ?);";
                if (sqlite3_prepare_v2(db, create_order, -1, &stmt, NULL) == SQLITE_OK) {
                    sqlite3_bind_int(stmt, 1, user_id);
                    sqlite3_bind_int(stmt, 2, product_id);
                    sqlite3_bind_int(stmt, 3, quantity);
                    sqlite3_bind_double(stmt, 4, price * quantity);
                    sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }
                
                // Commit transaction
                sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
                printf("   Order placed successfully!\n");
            } else {
                sqlite3_finalize(stmt);
                // Rollback
                sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
                printf("   Insufficient stock! Transaction rolled back.\n");
            }
        } else {
            sqlite3_finalize(stmt);
            sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        }
    }
}

// Advanced queries demo
void advanced_queries_demo(sqlite3 *db) {
    printf("\n=== Advanced Queries Demo ===\n");
    
    sqlite3_stmt *stmt;
    
    // JOIN query - Get user orders
    printf("\n1. JOIN - User orders:\n");
    const char *join_sql = 
        "SELECT u.username, p.name, o.quantity, o.total, o.order_date "
        "FROM orders o "
        "JOIN users u ON o.user_id = u.id "
        "JOIN products p ON o.product_id = p.id;";
    
    if (sqlite3_prepare_v2(db, join_sql, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("   %s ordered %d x %s for $%.2f on %s\n",
                   sqlite3_column_text(stmt, 0),
                   sqlite3_column_int(stmt, 2),
                   sqlite3_column_text(stmt, 1),
                   sqlite3_column_double(stmt, 3),
                   sqlite3_column_text(stmt, 4));
        }
        sqlite3_finalize(stmt);
    }
    
    // Aggregate functions
    printf("\n2. Aggregate Functions:\n");
    
    // Count users
    const char *count_sql = "SELECT COUNT(*) FROM users;";
    if (sqlite3_prepare_v2(db, count_sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("   Total users: %d\n", sqlite3_column_int(stmt, 0));
        }
        sqlite3_finalize(stmt);
    }
    
    // Average age
    const char *avg_sql = "SELECT AVG(age) FROM users;";
    if (sqlite3_prepare_v2(db, avg_sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("   Average age: %.2f\n", sqlite3_column_double(stmt, 0));
        }
        sqlite3_finalize(stmt);
    }
    
    // GROUP BY - Orders per user
    printf("\n3. GROUP BY - Orders per user:\n");
    const char *group_sql = 
        "SELECT u.username, COUNT(o.id) as order_count, SUM(o.total) as total_spent "
        "FROM users u "
        "LEFT JOIN orders o ON u.id = o.user_id "
        "GROUP BY u.id;";
    
    if (sqlite3_prepare_v2(db, group_sql, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("   %s: %d orders, $%.2f total\n",
                   sqlite3_column_text(stmt, 0),
                   sqlite3_column_int(stmt, 1),
                   sqlite3_column_double(stmt, 2));
        }
        sqlite3_finalize(stmt);
    }
    
    // Subquery - Users with orders
    printf("\n4. Subquery - Users who have placed orders:\n");
    const char *subquery_sql = 
        "SELECT username FROM users "
        "WHERE id IN (SELECT DISTINCT user_id FROM orders);";
    
    if (sqlite3_prepare_v2(db, subquery_sql, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("   %s\n", sqlite3_column_text(stmt, 0));
        }
        sqlite3_finalize(stmt);
    }
}

// Prepared statements and security demo
void security_demo(sqlite3 *db) {
    printf("\n=== Security Demo ===\n");
    
    // Simulated user input (could be malicious)
    const char *user_input = "admin'; DROP TABLE users; --";
    
    // BAD: String concatenation (DON'T DO THIS!)
    printf("\n1. UNSAFE - String concatenation:\n");
    char unsafe_sql[256];
    sprintf(unsafe_sql, "SELECT * FROM users WHERE username = '%s';", user_input);
    printf("   Generated SQL: %s\n", unsafe_sql);
    printf("   This would be vulnerable to SQL injection!\n");
    
    // GOOD: Prepared statement
    printf("\n2. SAFE - Prepared statement:\n");
    sqlite3_stmt *stmt;
    const char *safe_sql = "SELECT * FROM users WHERE username = ?;";
    
    if (sqlite3_prepare_v2(db, safe_sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, user_input, -1, SQLITE_STATIC);
        printf("   SQL injection attempt safely handled\n");
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("   Found user (unlikely with that input!)\n");
        } else {
            printf("   No user found (expected)\n");
        }
        sqlite3_finalize(stmt);
    }
}

// Pagination demo
void pagination_demo(sqlite3 *db) {
    printf("\n=== Pagination Demo ===\n");
    
    // Add more users for pagination
    sqlite3_stmt *stmt;
    const char *insert_sql = "INSERT OR IGNORE INTO users (username, email, age) VALUES (?, ?, ?);";
    
    for (int i = 1; i <= 20; i++) {
        char username[50], email[100];
        sprintf(username, "user%d", i);
        sprintf(email, "user%d@example.com", i);
        
        if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, email, -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 3, 20 + i);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
    
    // Paginated query
    int page = 2;
    int per_page = 5;
    int offset = (page - 1) * per_page;
    
    printf("\nShowing page %d (items per page: %d):\n", page, per_page);
    
    const char *page_sql = "SELECT username, email FROM users ORDER BY id LIMIT ? OFFSET ?;";
    if (sqlite3_prepare_v2(db, page_sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, per_page);
        sqlite3_bind_int(stmt, 2, offset);
        
        int row = 1;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("   %d. %s - %s\n", 
                   offset + row,
                   sqlite3_column_text(stmt, 0),
                   sqlite3_column_text(stmt, 1));
            row++;
        }
        sqlite3_finalize(stmt);
    }
}

// Full-text search demo
void search_demo(sqlite3 *db) {
    printf("\n=== Search Demo ===\n");
    
    const char *search_term = "john";
    char pattern[100];
    sprintf(pattern, "%%%s%%", search_term);
    
    printf("Searching for '%s':\n", search_term);
    
    sqlite3_stmt *stmt;
    const char *search_sql = 
        "SELECT username, email FROM users "
        "WHERE username LIKE ? OR email LIKE ? "
        "ORDER BY username;";
    
    if (sqlite3_prepare_v2(db, search_sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, pattern, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, pattern, -1, SQLITE_STATIC);
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("   Found: %s - %s\n",
                   sqlite3_column_text(stmt, 0),
                   sqlite3_column_text(stmt, 1));
        }
        sqlite3_finalize(stmt);
    }
}

// Performance optimization demo
void performance_demo(sqlite3 *db) {
    printf("\n=== Performance Demo ===\n");
    
    // Enable query timing
    sqlite3_exec(db, "PRAGMA timer = ON;", NULL, NULL, NULL);
    
    // Demonstrate the importance of indexes
    printf("\n1. Query performance with index:\n");
    
    sqlite3_stmt *stmt;
    clock_t start, end;
    
    // Query with index (email has index)
    start = clock();
    const char *indexed_query = "SELECT * FROM users WHERE email = 'user10@example.com';";
    if (sqlite3_prepare_v2(db, indexed_query, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    end = clock();
    printf("   Query with index took: %.6f seconds\n", 
           ((double)(end - start)) / CLOCKS_PER_SEC);
    
    // Batch insert with transaction
    printf("\n2. Batch insert performance:\n");
    
    // Without transaction
    start = clock();
    for (int i = 0; i < 100; i++) {
        char sql[200];
        sprintf(sql, "INSERT INTO users (username, email, age) VALUES ('test%d', 'test%d@example.com', %d);", 
                i, i, 20 + i);
        sqlite3_exec(db, sql, NULL, NULL, NULL);
    }
    end = clock();
    printf("   100 inserts without transaction: %.6f seconds\n", 
           ((double)(end - start)) / CLOCKS_PER_SEC);
    
    // With transaction
    start = clock();
    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    for (int i = 100; i < 200; i++) {
        char sql[200];
        sprintf(sql, "INSERT INTO users (username, email, age) VALUES ('test%d', 'test%d@example.com', %d);", 
                i, i, 20 + i);
        sqlite3_exec(db, sql, NULL, NULL, NULL);
    }
    sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    end = clock();
    printf("   100 inserts with transaction: %.6f seconds (much faster!)\n", 
           ((double)(end - start)) / CLOCKS_PER_SEC);
}

// Main function
int main() {
    printf("SQL IN C PROGRAMMING EXAMPLES\n");
    printf("=============================\n");
    
    sqlite3 *db;
    
    // Open database
    int rc = sqlite3_open("example.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    
    // Enable foreign keys
    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL);
    
    // Initialize database
    if (init_database(db) != 0) {
        sqlite3_close(db);
        return 1;
    }
    
    // Run demonstrations
    crud_operations_demo(db);
    transaction_demo(db);
    advanced_queries_demo(db);
    security_demo(db);
    pagination_demo(db);
    search_demo(db);
    performance_demo(db);
    
    // Cleanup
    sqlite3_close(db);
    
    printf("\nAll demonstrations completed!\n");
    printf("\nNote: To compile this program, use:\n");
    printf("gcc sql_examples.c -lsqlite3 -o sql_examples\n");
    
    return 0;
}