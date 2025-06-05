#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// CSV Parser with dynamic memory allocation

// Structure to hold a CSV field
typedef struct {
    char *data;
    size_t length;
} CSVField;

// Structure to hold a CSV row
typedef struct {
    CSVField *fields;
    size_t field_count;
    size_t capacity;
} CSVRow;

// Structure to hold entire CSV data
typedef struct {
    CSVRow *rows;
    size_t row_count;
    size_t capacity;
    char delimiter;
    char quote_char;
} CSVData;

// Initialize CSV data structure
CSVData* csv_init(char delimiter, char quote_char) {
    CSVData *csv = (CSVData*)malloc(sizeof(CSVData));
    if (!csv) return NULL;
    
    csv->rows = NULL;
    csv->row_count = 0;
    csv->capacity = 0;
    csv->delimiter = delimiter ? delimiter : ',';
    csv->quote_char = quote_char ? quote_char : '"';
    
    return csv;
}

// Free a single field
void csv_free_field(CSVField *field) {
    if (field && field->data) {
        free(field->data);
        field->data = NULL;
        field->length = 0;
    }
}

// Free a row
void csv_free_row(CSVRow *row) {
    if (row) {
        for (size_t i = 0; i < row->field_count; i++) {
            csv_free_field(&row->fields[i]);
        }
        free(row->fields);
        row->fields = NULL;
        row->field_count = 0;
        row->capacity = 0;
    }
}

// Free entire CSV data
void csv_free(CSVData *csv) {
    if (csv) {
        for (size_t i = 0; i < csv->row_count; i++) {
            csv_free_row(&csv->rows[i]);
        }
        free(csv->rows);
        free(csv);
    }
}

// Add a field to a row
bool csv_add_field(CSVRow *row, const char *data, size_t length) {
    // Resize if needed
    if (row->field_count >= row->capacity) {
        size_t new_capacity = row->capacity == 0 ? 4 : row->capacity * 2;
        CSVField *new_fields = (CSVField*)realloc(row->fields, 
                                                   new_capacity * sizeof(CSVField));
        if (!new_fields) return false;
        
        row->fields = new_fields;
        row->capacity = new_capacity;
    }
    
    // Allocate and copy field data
    row->fields[row->field_count].data = (char*)malloc(length + 1);
    if (!row->fields[row->field_count].data) return false;
    
    memcpy(row->fields[row->field_count].data, data, length);
    row->fields[row->field_count].data[length] = '\0';
    row->fields[row->field_count].length = length;
    row->field_count++;
    
    return true;
}

// Add a row to CSV data
bool csv_add_row(CSVData *csv) {
    if (csv->row_count >= csv->capacity) {
        size_t new_capacity = csv->capacity == 0 ? 16 : csv->capacity * 2;
        CSVRow *new_rows = (CSVRow*)realloc(csv->rows, 
                                            new_capacity * sizeof(CSVRow));
        if (!new_rows) return false;
        
        csv->rows = new_rows;
        csv->capacity = new_capacity;
    }
    
    CSVRow *new_row = &csv->rows[csv->row_count];
    new_row->fields = NULL;
    new_row->field_count = 0;
    new_row->capacity = 0;
    csv->row_count++;
    
    return true;
}

// Parse a CSV line
bool csv_parse_line(CSVData *csv, const char *line) {
    if (!csv_add_row(csv)) return false;
    
    CSVRow *current_row = &csv->rows[csv->row_count - 1];
    const char *field_start = line;
    const char *p = line;
    bool in_quotes = false;
    bool field_has_quotes = false;
    
    while (*p) {
        if (*p == csv->quote_char) {
            if (p == field_start || *(p - 1) == csv->delimiter) {
                in_quotes = true;
                field_has_quotes = true;
                field_start = p + 1;
            } else if (in_quotes) {
                if (*(p + 1) == csv->quote_char) {
                    // Escaped quote
                    p++;
                } else {
                    in_quotes = false;
                }
            }
        } else if (*p == csv->delimiter && !in_quotes) {
            // End of field
            size_t length = p - field_start;
            if (field_has_quotes && length > 0) {
                length--; // Remove closing quote
            }
            
            if (!csv_add_field(current_row, field_start, length)) {
                return false;
            }
            
            field_start = p + 1;
            field_has_quotes = false;
        } else if (*p == '\n' || *p == '\r') {
            break;
        }
        p++;
    }
    
    // Add last field
    size_t length = p - field_start;
    if (field_has_quotes && length > 0) {
        length--; // Remove closing quote
    }
    
    // Skip empty trailing field if line ends with delimiter
    if (length > 0 || *(p - 1) != csv->delimiter) {
        if (!csv_add_field(current_row, field_start, length)) {
            return false;
        }
    }
    
    return true;
}

// Parse CSV file
CSVData* csv_parse_file(const char *filename, char delimiter, char quote_char) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return NULL;
    }
    
    CSVData *csv = csv_init(delimiter, quote_char);
    if (!csv) {
        fclose(file);
        return NULL;
    }
    
    char *line = NULL;
    size_t line_capacity = 0;
    ssize_t line_length;
    
    // Dynamic line reading
    while ((line_length = getline(&line, &line_capacity, file)) != -1) {
        // Skip empty lines
        if (line_length > 0 && line[0] != '\n' && line[0] != '\r') {
            if (!csv_parse_line(csv, line)) {
                fprintf(stderr, "Error parsing line\n");
                free(line);
                fclose(file);
                csv_free(csv);
                return NULL;
            }
        }
    }
    
    free(line);
    fclose(file);
    return csv;
}

// Parse CSV from string
CSVData* csv_parse_string(const char *str, char delimiter, char quote_char) {
    CSVData *csv = csv_init(delimiter, quote_char);
    if (!csv) return NULL;
    
    const char *line_start = str;
    const char *p = str;
    
    while (*p) {
        if (*p == '\n') {
            size_t line_length = p - line_start;
            char *line = (char*)malloc(line_length + 1);
            if (!line) {
                csv_free(csv);
                return NULL;
            }
            
            memcpy(line, line_start, line_length);
            line[line_length] = '\0';
            
            if (line_length > 0) {
                if (!csv_parse_line(csv, line)) {
                    free(line);
                    csv_free(csv);
                    return NULL;
                }
            }
            
            free(line);
            line_start = p + 1;
        }
        p++;
    }
    
    // Parse last line if no trailing newline
    if (line_start < p) {
        if (!csv_parse_line(csv, line_start)) {
            csv_free(csv);
            return NULL;
        }
    }
    
    return csv;
}

// Get field value
const char* csv_get_field(CSVData *csv, size_t row, size_t col) {
    if (!csv || row >= csv->row_count) return NULL;
    if (col >= csv->rows[row].field_count) return NULL;
    return csv->rows[row].fields[col].data;
}

// Print CSV data
void csv_print(CSVData *csv) {
    if (!csv) return;
    
    printf("CSV Data (%zu rows):\n", csv->row_count);
    for (size_t i = 0; i < csv->row_count; i++) {
        printf("Row %zu: ", i);
        for (size_t j = 0; j < csv->rows[i].field_count; j++) {
            printf("[%s]", csv->rows[i].fields[j].data);
            if (j < csv->rows[i].field_count - 1) {
                printf(" ");
            }
        }
        printf("\n");
    }
}

// Write CSV to file
bool csv_write_file(CSVData *csv, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) return false;
    
    for (size_t i = 0; i < csv->row_count; i++) {
        for (size_t j = 0; j < csv->rows[i].field_count; j++) {
            const char *field = csv->rows[i].fields[j].data;
            
            // Check if field needs quoting
            bool needs_quotes = false;
            for (const char *p = field; *p; p++) {
                if (*p == csv->delimiter || *p == csv->quote_char || 
                    *p == '\n' || *p == '\r') {
                    needs_quotes = true;
                    break;
                }
            }
            
            if (needs_quotes) {
                fputc(csv->quote_char, file);
                for (const char *p = field; *p; p++) {
                    if (*p == csv->quote_char) {
                        fputc(csv->quote_char, file); // Escape quote
                    }
                    fputc(*p, file);
                }
                fputc(csv->quote_char, file);
            } else {
                fputs(field, file);
            }
            
            if (j < csv->rows[i].field_count - 1) {
                fputc(csv->delimiter, file);
            }
        }
        fputc('\n', file);
    }
    
    fclose(file);
    return true;
}

// Advanced CSV parser with header support
typedef struct {
    CSVData *data;
    char **headers;
    size_t header_count;
} CSVTable;

CSVTable* csv_parse_with_headers(const char *filename, char delimiter, char quote_char) {
    CSVData *data = csv_parse_file(filename, delimiter, quote_char);
    if (!data || data->row_count == 0) return NULL;
    
    CSVTable *table = (CSVTable*)malloc(sizeof(CSVTable));
    if (!table) {
        csv_free(data);
        return NULL;
    }
    
    // First row as headers
    table->header_count = data->rows[0].field_count;
    table->headers = (char**)malloc(table->header_count * sizeof(char*));
    if (!table->headers) {
        free(table);
        csv_free(data);
        return NULL;
    }
    
    // Copy headers
    for (size_t i = 0; i < table->header_count; i++) {
        table->headers[i] = strdup(data->rows[0].fields[i].data);
    }
    
    // Remove header row from data
    csv_free_row(&data->rows[0]);
    memmove(&data->rows[0], &data->rows[1], 
            (data->row_count - 1) * sizeof(CSVRow));
    data->row_count--;
    
    table->data = data;
    return table;
}

// Get field by header name
const char* csv_get_field_by_name(CSVTable *table, size_t row, const char *header) {
    if (!table || !header) return NULL;
    
    // Find header index
    size_t col = (size_t)-1;
    for (size_t i = 0; i < table->header_count; i++) {
        if (strcmp(table->headers[i], header) == 0) {
            col = i;
            break;
        }
    }
    
    if (col == (size_t)-1) return NULL;
    return csv_get_field(table->data, row, col);
}

// Free CSV table
void csv_table_free(CSVTable *table) {
    if (table) {
        for (size_t i = 0; i < table->header_count; i++) {
            free(table->headers[i]);
        }
        free(table->headers);
        csv_free(table->data);
        free(table);
    }
}

// Demo function
void csv_parser_demo() {
    printf("=== CSV Parser Demo ===\n\n");
    
    // Create sample CSV data
    const char *csv_string = 
        "Name,Age,City,Salary\n"
        "John Doe,30,\"New York\",50000\n"
        "Jane Smith,25,\"Los Angeles\",60000\n"
        "Bob Johnson,35,Chicago,55000\n"
        "\"Alice, M. Brown\",28,\"San Francisco\",65000\n"
        "Charlie Davis,32,\"Boston, MA\",58000\n";
    
    printf("Parsing CSV string:\n%s\n", csv_string);
    
    // Parse CSV
    CSVData *csv = csv_parse_string(csv_string, ',', '"');
    if (csv) {
        csv_print(csv);
        
        // Access specific fields
        printf("\nAccessing specific fields:\n");
        printf("Row 1, Col 0: %s\n", csv_get_field(csv, 1, 0));
        printf("Row 2, Col 2: %s\n", csv_get_field(csv, 2, 2));
        
        // Write to file
        csv_write_file(csv, "output.csv");
        printf("\nCSV written to output.csv\n");
        
        csv_free(csv);
    }
    
    // Test with headers
    printf("\n=== CSV with Headers Demo ===\n");
    
    // Create a test file
    FILE *test_file = fopen("test_with_headers.csv", "w");
    if (test_file) {
        fputs(csv_string, test_file);
        fclose(test_file);
        
        CSVTable *table = csv_parse_with_headers("test_with_headers.csv", ',', '"');
        if (table) {
            printf("Headers: ");
            for (size_t i = 0; i < table->header_count; i++) {
                printf("%s ", table->headers[i]);
            }
            printf("\n\n");
            
            // Access by header name
            for (size_t i = 0; i < table->data->row_count; i++) {
                printf("Name: %s, Age: %s, Salary: %s\n",
                       csv_get_field_by_name(table, i, "Name"),
                       csv_get_field_by_name(table, i, "Age"),
                       csv_get_field_by_name(table, i, "Salary"));
            }
            
            csv_table_free(table);
        }
    }
}

int main() {
    csv_parser_demo();
    return 0;
}