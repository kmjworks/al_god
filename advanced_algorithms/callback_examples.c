#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 1. Basic callback mechanism
typedef void (*callback_t)(int);

void process_array(int *arr, size_t size, callback_t cb) {
    for (size_t i = 0; i < size; i++) {
        cb(arr[i]);
    }
}

void print_callback(int value) {
    printf("%d ", value);
}

void square_callback(int value) {
    printf("%d ", value * value);
}

// 2. Callbacks with context (closure simulation)
typedef struct {
    void (*callback)(void *context, int value);
    void *context;
} CallbackWithContext;

typedef struct {
    int sum;
    int count;
} SumContext;

void sum_callback(void *context, int value) {
    SumContext *ctx = (SumContext *)context;
    ctx->sum += value;
    ctx->count++;
}

void process_with_context(int *arr, size_t size, CallbackWithContext *cb) {
    for (size_t i = 0; i < size; i++) {
        cb->callback(cb->context, arr[i]);
    }
}

// 3. Event system with multiple callbacks
typedef enum {
    EVENT_START,
    EVENT_PROCESS,
    EVENT_END,
    EVENT_ERROR,
    EVENT_COUNT
} EventType;

typedef struct EventHandler {
    void (*handler)(void *data);
    struct EventHandler *next;
} EventHandler;

typedef struct {
    EventHandler *handlers[EVENT_COUNT];
} EventSystem;

EventSystem* create_event_system() {
    EventSystem *sys = (EventSystem *)calloc(1, sizeof(EventSystem));
    return sys;
}

void register_event(EventSystem *sys, EventType type, void (*handler)(void *)) {
    EventHandler *new_handler = (EventHandler *)malloc(sizeof(EventHandler));
    new_handler->handler = handler;
    new_handler->next = sys->handlers[type];
    sys->handlers[type] = new_handler;
}

void trigger_event(EventSystem *sys, EventType type, void *data) {
    EventHandler *handler = sys->handlers[type];
    while (handler) {
        handler->handler(data);
        handler = handler->next;
    }
}

void on_start(void *data) {
    printf("System starting... (data: %s)\n", (char *)data);
}

void on_process(void *data) {
    int *value = (int *)data;
    printf("Processing value: %d\n", *value);
}

void on_end(void *data) {
    printf("System ending...\n");
}

// 4. Function pointer state machine
typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_PAUSED,
    STATE_STOPPED
} State;

typedef struct {
    State current_state;
    void (*state_handlers[4])(void *);
} StateMachine;

void idle_handler(void *sm) {
    StateMachine *machine = (StateMachine *)sm;
    printf("In IDLE state. Starting...\n");
    machine->current_state = STATE_RUNNING;
}

void running_handler(void *sm) {
    StateMachine *machine = (StateMachine *)sm;
    static int counter = 0;
    printf("Running... (iteration %d)\n", ++counter);
    if (counter >= 3) {
        machine->current_state = STATE_PAUSED;
        counter = 0;
    }
}

void paused_handler(void *sm) {
    StateMachine *machine = (StateMachine *)sm;
    printf("Paused. Resuming...\n");
    machine->current_state = STATE_RUNNING;
}

void stopped_handler(void *sm) {
    printf("Stopped.\n");
}

void run_state_machine(StateMachine *sm, int steps) {
    for (int i = 0; i < steps && sm->current_state != STATE_STOPPED; i++) {
        sm->state_handlers[sm->current_state](sm);
    }
}

// 5. Generic comparator callbacks for sorting
typedef int (*comparator_t)(const void *, const void *);

typedef struct {
    char name[50];
    int age;
    double salary;
} Employee;

int compare_by_name(const void *a, const void *b) {
    const Employee *emp1 = (const Employee *)a;
    const Employee *emp2 = (const Employee *)b;
    return strcmp(emp1->name, emp2->name);
}

int compare_by_age(const void *a, const void *b) {
    const Employee *emp1 = (const Employee *)a;
    const Employee *emp2 = (const Employee *)b;
    return emp1->age - emp2->age;
}

int compare_by_salary(const void *a, const void *b) {
    const Employee *emp1 = (const Employee *)a;
    const Employee *emp2 = (const Employee *)b;
    double diff = emp1->salary - emp2->salary;
    return (diff > 0) - (diff < 0);
}

// 6. Callback chains (middleware pattern)
typedef int (*middleware_t)(void *data, int (*next)(void *));

int logger_middleware(void *data, int (*next)(void *)) {
    printf("[LOG] Processing data: %s\n", (char *)data);
    int result = next(data);
    printf("[LOG] Result: %d\n", result);
    return result;
}

int uppercase_middleware(void *data, int (*next)(void *)) {
    char *str = (char *)data;
    for (int i = 0; str[i]; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= 32;
        }
    }
    return next(data);
}

int final_handler(void *data) {
    printf("Final data: %s\n", (char *)data);
    return strlen((char *)data);
}

// 7. Async simulation with callbacks
typedef struct {
    void (*on_success)(void *result);
    void (*on_error)(char *error);
    void *user_data;
} AsyncCallback;

void async_operation(int value, AsyncCallback *cb) {
    // Simulate async operation
    if (value < 0) {
        cb->on_error("Negative value not allowed");
    } else {
        int *result = malloc(sizeof(int));
        *result = value * value;
        cb->on_success(result);
        free(result);
    }
}

void on_success_handler(void *result) {
    printf("Success! Result: %d\n", *(int *)result);
}

void on_error_handler(char *error) {
    printf("Error: %s\n", error);
}

// 8. Plugin system with function pointers
typedef struct {
    char name[50];
    void (*init)(void);
    void (*execute)(void *data);
    void (*cleanup)(void);
} Plugin;

void math_plugin_init() {
    printf("Math plugin initialized\n");
}

void math_plugin_execute(void *data) {
    int *value = (int *)data;
    printf("Math plugin: %d squared is %d\n", *value, (*value) * (*value));
}

void math_plugin_cleanup() {
    printf("Math plugin cleaned up\n");
}

// 9. Generic iterator with callbacks
typedef struct {
    void *data;
    size_t element_size;
    size_t count;
    void (*for_each)(void *data, size_t count, size_t elem_size, 
                    void (*cb)(void *elem, size_t index));
} Iterator;

void array_for_each(void *data, size_t count, size_t elem_size, 
                   void (*cb)(void *elem, size_t index)) {
    char *arr = (char *)data;
    for (size_t i = 0; i < count; i++) {
        cb(arr + (i * elem_size), i);
    }
}

void print_int_with_index(void *elem, size_t index) {
    printf("[%zu]: %d\n", index, *(int *)elem);
}

// 10. Complex callback with return value
typedef int (*filter_callback_t)(void *item);
typedef void* (*map_callback_t)(void *item);

int* filter_array(int *arr, size_t size, filter_callback_t filter, size_t *new_size) {
    int *result = malloc(size * sizeof(int));
    *new_size = 0;
    
    for (size_t i = 0; i < size; i++) {
        if (filter(&arr[i])) {
            result[(*new_size)++] = arr[i];
        }
    }
    
    result = realloc(result, (*new_size) * sizeof(int));
    return result;
}

int is_even(void *item) {
    return *(int *)item % 2 == 0;
}

int is_positive(void *item) {
    return *(int *)item > 0;
}

int main() {
    printf("=== Function Pointer and Callback Examples ===\n\n");
    
    // 1. Basic callbacks
    printf("1. Basic callbacks:\n");
    int arr[] = {1, 2, 3, 4, 5};
    printf("Original: ");
    process_array(arr, 5, print_callback);
    printf("\nSquared: ");
    process_array(arr, 5, square_callback);
    printf("\n\n");
    
    // 2. Callbacks with context
    printf("2. Callbacks with context:\n");
    SumContext ctx = {0, 0};
    CallbackWithContext cb_ctx = {sum_callback, &ctx};
    process_with_context(arr, 5, &cb_ctx);
    printf("Sum: %d, Count: %d, Average: %.2f\n\n", 
           ctx.sum, ctx.count, (double)ctx.sum / ctx.count);
    
    // 3. Event system
    printf("3. Event system:\n");
    EventSystem *events = create_event_system();
    register_event(events, EVENT_START, on_start);
    register_event(events, EVENT_PROCESS, on_process);
    register_event(events, EVENT_END, on_end);
    
    trigger_event(events, EVENT_START, "System v1.0");
    int value = 42;
    trigger_event(events, EVENT_PROCESS, &value);
    trigger_event(events, EVENT_END, NULL);
    printf("\n");
    
    // 4. State machine
    printf("4. State machine:\n");
    StateMachine sm = {
        STATE_IDLE,
        {idle_handler, running_handler, paused_handler, stopped_handler}
    };
    run_state_machine(&sm, 10);
    printf("\n");
    
    // 5. Sorting with comparators
    printf("5. Sorting with different comparators:\n");
    Employee employees[] = {
        {"Charlie", 30, 50000},
        {"Alice", 25, 60000},
        {"Bob", 35, 55000}
    };
    
    qsort(employees, 3, sizeof(Employee), compare_by_name);
    printf("Sorted by name:\n");
    for (int i = 0; i < 3; i++) {
        printf("  %s, %d, %.0f\n", employees[i].name, 
               employees[i].age, employees[i].salary);
    }
    
    qsort(employees, 3, sizeof(Employee), compare_by_age);
    printf("Sorted by age:\n");
    for (int i = 0; i < 3; i++) {
        printf("  %s, %d, %.0f\n", employees[i].name, 
               employees[i].age, employees[i].salary);
    }
    printf("\n");
    
    // 6. Middleware chain
    printf("6. Middleware chain:\n");
    char data[] = "hello world";
    int result = logger_middleware(data, 
                     (int (*)(void *))uppercase_middleware);
    printf("\n");
    
    // 7. Async callbacks
    printf("7. Async operation callbacks:\n");
    AsyncCallback async_cb = {on_success_handler, on_error_handler, NULL};
    async_operation(5, &async_cb);
    async_operation(-3, &async_cb);
    printf("\n");
    
    // 8. Plugin system
    printf("8. Plugin system:\n");
    Plugin math_plugin = {
        "Math Plugin",
        math_plugin_init,
        math_plugin_execute,
        math_plugin_cleanup
    };
    
    math_plugin.init();
    int plugin_data = 7;
    math_plugin.execute(&plugin_data);
    math_plugin.cleanup();
    printf("\n");
    
    // 9. Generic iterator
    printf("9. Generic iterator:\n");
    Iterator iter = {
        arr,
        sizeof(int),
        5,
        array_for_each
    };
    iter.for_each(iter.data, iter.count, iter.element_size, print_int_with_index);
    printf("\n");
    
    // 10. Filter with callbacks
    printf("10. Filter with callbacks:\n");
    int mixed[] = {-2, 3, -5, 8, 0, 12, -7, 4};
    size_t filtered_size;
    
    int *evens = filter_array(mixed, 8, is_even, &filtered_size);
    printf("Even numbers: ");
    for (size_t i = 0; i < filtered_size; i++) {
        printf("%d ", evens[i]);
    }
    printf("\n");
    
    int *positives = filter_array(mixed, 8, is_positive, &filtered_size);
    printf("Positive numbers: ");
    for (size_t i = 0; i < filtered_size; i++) {
        printf("%d ", positives[i]);
    }
    printf("\n");
    
    free(evens);
    free(positives);
    free(events);
    
    return 0;
}