#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdbool.h>
#include <stddef.h>

// String manipulation functions
char* str_trim(char *str);
char* str_ltrim(char *str);
char* str_rtrim(char *str);
char* str_duplicate(const char *str);
char* str_reverse(char *str);
char* str_to_upper(char *str);
char* str_to_lower(char *str);

// String examination functions
bool str_starts_with(const char *str, const char *prefix);
bool str_ends_with(const char *str, const char *suffix);
bool str_contains(const char *str, const char *substr);
int str_count_char(const char *str, char ch);
int str_count_substr(const char *str, const char *substr);

// String comparison
int str_compare_ignore_case(const char *s1, const char *s2);
bool str_equals_ignore_case(const char *s1, const char *s2);

// String splitting and joining
char** str_split(const char *str, const char *delimiter, int *count);
char* str_join(const char **strings, int count, const char *delimiter);
void str_free_array(char **strings, int count);

// String replacement
char* str_replace(const char *str, const char *old_substr, const char *new_substr);
char* str_replace_all(const char *str, const char *old_substr, const char *new_substr);

// Safe string operations
size_t str_copy_safe(char *dest, const char *src, size_t dest_size);
size_t str_concat_safe(char *dest, const char *src, size_t dest_size);

// Utility macros
#define SAFE_STRLEN(s) ((s) ? strlen(s) : 0)
#define STR_EMPTY(s) ((s) == NULL || (s)[0] == '\0')
#define STR_NOT_EMPTY(s) (!STR_EMPTY(s))

// Inline helper functions
static inline bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

static inline bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static inline bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static inline bool is_alnum(char c) {
    return is_alpha(c) || is_digit(c);
}

#endif // STRING_UTILS_H