// Embedded Systems Programming Examples
// Note: This is a simulation for educational purposes
// Real embedded code would run on actual hardware

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// Simulated hardware registers
static uint32_t GPIO_DATA = 0;
static uint32_t GPIO_DIR = 0;
static uint32_t TIMER_COUNT = 0;
static uint32_t UART_DATA = 0;
static uint32_t UART_STATUS = 0;
static uint32_t ADC_DATA = 0;

// Bit manipulation macros
#define BIT(x) (1UL << (x))
#define SET_BIT(reg, bit) ((reg) |= BIT(bit))
#define CLEAR_BIT(reg, bit) ((reg) &= ~BIT(bit))
#define TOGGLE_BIT(reg, bit) ((reg) ^= BIT(bit))
#define CHECK_BIT(reg, bit) ((reg) & BIT(bit))
#define WRITE_REG(reg, val) ((reg) = (val))
#define READ_REG(reg) (reg)

// GPIO example - LED control
#define LED_PIN 5
#define BUTTON_PIN 2

void gpio_demo() {
    printf("\n=== GPIO Demo - LED and Button ===\n");
    
    // Configure LED pin as output
    SET_BIT(GPIO_DIR, LED_PIN);
    printf("LED pin configured as output\n");
    
    // Configure button pin as input
    CLEAR_BIT(GPIO_DIR, BUTTON_PIN);
    printf("Button pin configured as input\n");
    
    // Simulate button press
    SET_BIT(GPIO_DATA, BUTTON_PIN);
    
    // Read button and control LED
    if (CHECK_BIT(GPIO_DATA, BUTTON_PIN)) {
        SET_BIT(GPIO_DATA, LED_PIN);
        printf("Button pressed - LED ON\n");
    } else {
        CLEAR_BIT(GPIO_DATA, LED_PIN);
        printf("Button released - LED OFF\n");
    }
    
    // Toggle LED
    for (int i = 0; i < 5; i++) {
        TOGGLE_BIT(GPIO_DATA, LED_PIN);
        printf("LED toggled: %s\n", 
               CHECK_BIT(GPIO_DATA, LED_PIN) ? "ON" : "OFF");
    }
}

// Circular buffer for UART
typedef struct {
    uint8_t buffer[64];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} circular_buffer_t;

static circular_buffer_t uart_rx_buffer = {0};
static circular_buffer_t uart_tx_buffer = {0};

void buffer_init(circular_buffer_t* buf) {
    buf->head = 0;
    buf->tail = 0;
    buf->count = 0;
}

bool buffer_put(circular_buffer_t* buf, uint8_t data) {
    if (buf->count >= 64) {
        return false;  // Buffer full
    }
    
    buf->buffer[buf->tail] = data;
    buf->tail = (buf->tail + 1) % 64;
    buf->count++;
    return true;
}

bool buffer_get(circular_buffer_t* buf, uint8_t* data) {
    if (buf->count == 0) {
        return false;  // Buffer empty
    }
    
    *data = buf->buffer[buf->head];
    buf->head = (buf->head + 1) % 64;
    buf->count--;
    return true;
}

// UART simulation
#define UART_STATUS_TX_EMPTY BIT(0)
#define UART_STATUS_RX_READY BIT(1)

void uart_demo() {
    printf("\n=== UART Communication Demo ===\n");
    
    buffer_init(&uart_rx_buffer);
    buffer_init(&uart_tx_buffer);
    
    // Simulate sending data
    const char* message = "Hello Embedded!";
    printf("Sending: %s\n", message);
    
    for (int i = 0; message[i]; i++) {
        // Wait for transmit buffer empty
        UART_STATUS |= UART_STATUS_TX_EMPTY;
        
        if (UART_STATUS & UART_STATUS_TX_EMPTY) {
            UART_DATA = message[i];
            buffer_put(&uart_tx_buffer, message[i]);
            printf("TX: 0x%02X ('%c')\n", message[i], message[i]);
        }
    }
    
    // Simulate receiving data
    printf("\nReceiving data...\n");
    uint8_t rx_data;
    while (buffer_get(&uart_tx_buffer, &rx_data)) {
        buffer_put(&uart_rx_buffer, rx_data);
        UART_STATUS |= UART_STATUS_RX_READY;
        printf("RX: 0x%02X ('%c')\n", rx_data, rx_data);
    }
}

// Timer and delay functions
static volatile uint32_t systick_count = 0;

void systick_handler(void) {
    systick_count++;
}

uint32_t get_tick(void) {
    return systick_count;
}

void delay_ms(uint32_t ms) {
    uint32_t start = get_tick();
    while ((get_tick() - start) < ms) {
        // Simulate waiting
    }
}

void timer_demo() {
    printf("\n=== Timer Demo ===\n");
    
    // Simulate timer counting
    printf("Starting timer...\n");
    for (int i = 0; i < 10; i++) {
        systick_handler();  // Simulate timer interrupt
        printf("Tick: %u\n", get_tick());
    }
    
    // Measure execution time
    uint32_t start = get_tick();
    
    // Simulate some work
    volatile uint32_t sum = 0;
    for (int i = 0; i < 1000; i++) {
        sum += i;
    }
    
    uint32_t elapsed = get_tick() - start;
    printf("Execution time: %u ticks\n", elapsed);
}

// State machine example - Traffic light controller
typedef enum {
    STATE_RED,
    STATE_YELLOW,
    STATE_GREEN
} traffic_state_t;

typedef struct {
    traffic_state_t state;
    uint32_t duration_ms;
    const char* name;
} traffic_light_t;

static traffic_light_t traffic_light = {
    .state = STATE_RED,
    .duration_ms = 0,
    .name = "RED"
};

void traffic_light_update(void) {
    static uint32_t last_change = 0;
    uint32_t current_time = get_tick();
    
    if (current_time - last_change >= traffic_light.duration_ms) {
        last_change = current_time;
        
        switch (traffic_light.state) {
            case STATE_RED:
                traffic_light.state = STATE_GREEN;
                traffic_light.duration_ms = 5000;  // 5 seconds
                traffic_light.name = "GREEN";
                break;
                
            case STATE_GREEN:
                traffic_light.state = STATE_YELLOW;
                traffic_light.duration_ms = 2000;  // 2 seconds
                traffic_light.name = "YELLOW";
                break;
                
            case STATE_YELLOW:
                traffic_light.state = STATE_RED;
                traffic_light.duration_ms = 5000;  // 5 seconds
                traffic_light.name = "RED";
                break;
        }
        
        printf("Traffic light changed to: %s\n", traffic_light.name);
    }
}

void state_machine_demo() {
    printf("\n=== State Machine Demo - Traffic Light ===\n");
    
    // Simulate traffic light for several cycles
    for (int i = 0; i < 20; i++) {
        traffic_light_update();
        systick_count += 1000;  // Simulate 1 second passing
    }
}

// ADC (Analog to Digital Converter) simulation
#define ADC_MAX_VALUE 4095  // 12-bit ADC

uint16_t adc_read(uint8_t channel) {
    // Simulate reading analog value
    // In real hardware, this would start conversion and wait
    static uint16_t simulated_values[] = {1024, 2048, 3072, 512};
    return simulated_values[channel % 4];
}

float adc_to_voltage(uint16_t adc_value, float vref) {
    return (adc_value * vref) / ADC_MAX_VALUE;
}

void adc_demo() {
    printf("\n=== ADC Demo - Reading Sensors ===\n");
    
    float vref = 3.3;  // 3.3V reference
    
    // Read multiple channels
    for (int ch = 0; ch < 4; ch++) {
        uint16_t raw = adc_read(ch);
        float voltage = adc_to_voltage(raw, vref);
        
        printf("Channel %d: Raw=%4d, Voltage=%.3fV", ch, raw, voltage);
        
        // Interpret as temperature sensor (example)
        if (ch == 0) {
            float temp = (voltage - 0.5) * 100;  // 10mV/°C, 0.5V at 0°C
            printf(" (Temperature: %.1f°C)", temp);
        }
        
        printf("\n");
    }
}

// Interrupt priority and handling
typedef struct {
    uint8_t priority;
    const char* name;
    void (*handler)(void);
    volatile bool pending;
} interrupt_t;

void high_priority_handler(void) {
    printf("  [HIGH PRIORITY ISR] Critical interrupt handled\n");
}

void medium_priority_handler(void) {
    printf("  [MEDIUM PRIORITY ISR] Normal interrupt handled\n");
}

void low_priority_handler(void) {
    printf("  [LOW PRIORITY ISR] Background interrupt handled\n");
}

static interrupt_t interrupts[] = {
    {0, "SysTick", high_priority_handler, false},
    {1, "UART", medium_priority_handler, false},
    {2, "Timer", low_priority_handler, false}
};

void interrupt_demo() {
    printf("\n=== Interrupt Priority Demo ===\n");
    
    // Simulate multiple interrupts occurring
    interrupts[2].pending = true;  // Low priority
    interrupts[0].pending = true;  // High priority
    interrupts[1].pending = true;  // Medium priority
    
    printf("Interrupts pending: Timer, SysTick, UART\n");
    printf("Processing in priority order:\n");
    
    // Process interrupts by priority
    for (int priority = 0; priority < 3; priority++) {
        for (int i = 0; i < 3; i++) {
            if (interrupts[i].priority == priority && interrupts[i].pending) {
                interrupts[i].handler();
                interrupts[i].pending = false;
            }
        }
    }
}

// Memory pool allocator
#define POOL_BLOCK_SIZE 32
#define POOL_NUM_BLOCKS 10

typedef struct block {
    struct block* next;
    uint8_t data[POOL_BLOCK_SIZE - sizeof(struct block*)];
} block_t;

static uint8_t memory_pool_storage[POOL_BLOCK_SIZE * POOL_NUM_BLOCKS];
static block_t* free_list = NULL;

void pool_init(void) {
    free_list = (block_t*)memory_pool_storage;
    
    // Link all blocks
    for (int i = 0; i < POOL_NUM_BLOCKS - 1; i++) {
        block_t* current = (block_t*)&memory_pool_storage[i * POOL_BLOCK_SIZE];
        block_t* next = (block_t*)&memory_pool_storage[(i + 1) * POOL_BLOCK_SIZE];
        current->next = next;
    }
    
    // Last block
    block_t* last = (block_t*)&memory_pool_storage[(POOL_NUM_BLOCKS - 1) * POOL_BLOCK_SIZE];
    last->next = NULL;
}

void* pool_alloc(void) {
    if (free_list == NULL) {
        return NULL;
    }
    
    block_t* block = free_list;
    free_list = free_list->next;
    return block->data;
}

void pool_free(void* ptr) {
    if (ptr == NULL) return;
    
    // Get block header
    block_t* block = (block_t*)((uint8_t*)ptr - offsetof(block_t, data));
    
    // Add to free list
    block->next = free_list;
    free_list = block;
}

void memory_pool_demo() {
    printf("\n=== Memory Pool Demo ===\n");
    
    pool_init();
    printf("Memory pool initialized: %d blocks of %d bytes\n", 
           POOL_NUM_BLOCKS, POOL_BLOCK_SIZE);
    
    // Allocate some blocks
    void* blocks[5];
    for (int i = 0; i < 5; i++) {
        blocks[i] = pool_alloc();
        if (blocks[i]) {
            printf("Allocated block %d at %p\n", i, blocks[i]);
            sprintf((char*)blocks[i], "Block %d", i);
        }
    }
    
    // Free some blocks
    pool_free(blocks[1]);
    pool_free(blocks[3]);
    printf("Freed blocks 1 and 3\n");
    
    // Allocate again
    void* new_block = pool_alloc();
    printf("New allocation at %p (should reuse freed block)\n", new_block);
    
    // Clean up
    for (int i = 0; i < 5; i++) {
        if (i != 1 && i != 3) {  // Already freed
            pool_free(blocks[i]);
        }
    }
}

// Watchdog timer simulation
static uint32_t watchdog_counter = 0;
static uint32_t watchdog_timeout = 1000;
static bool watchdog_enabled = false;

void watchdog_init(uint32_t timeout) {
    watchdog_timeout = timeout;
    watchdog_counter = 0;
    watchdog_enabled = true;
    printf("Watchdog initialized with %u ms timeout\n", timeout);
}

void watchdog_feed(void) {
    watchdog_counter = 0;
    printf("Watchdog fed - counter reset\n");
}

void watchdog_tick(void) {
    if (!watchdog_enabled) return;
    
    watchdog_counter++;
    if (watchdog_counter >= watchdog_timeout) {
        printf("*** WATCHDOG TIMEOUT - SYSTEM RESET ***\n");
        watchdog_counter = 0;
        // In real system, would trigger reset
    }
}

void watchdog_demo() {
    printf("\n=== Watchdog Timer Demo ===\n");
    
    watchdog_init(5);  // 5 tick timeout
    
    // Normal operation - feeding watchdog
    for (int i = 0; i < 10; i++) {
        printf("Tick %d: ", i);
        
        if (i % 3 == 0) {
            watchdog_feed();
        }
        
        watchdog_tick();
        
        if (i == 7) {
            printf("Simulating hang...\n");
            // Stop feeding watchdog
            for (int j = 0; j < 6; j++) {
                watchdog_tick();
            }
        }
    }
}

// Low power mode simulation
typedef enum {
    POWER_MODE_ACTIVE,
    POWER_MODE_IDLE,
    POWER_MODE_SLEEP,
    POWER_MODE_DEEP_SLEEP
} power_mode_t;

static power_mode_t current_power_mode = POWER_MODE_ACTIVE;

void enter_power_mode(power_mode_t mode) {
    const char* mode_names[] = {
        "ACTIVE", "IDLE", "SLEEP", "DEEP_SLEEP"
    };
    
    current_power_mode = mode;
    printf("Entering %s mode\n", mode_names[mode]);
    
    switch (mode) {
        case POWER_MODE_IDLE:
            printf("  CPU idle, peripherals active\n");
            break;
        case POWER_MODE_SLEEP:
            printf("  CPU stopped, selected peripherals active\n");
            break;
        case POWER_MODE_DEEP_SLEEP:
            printf("  Most systems powered down, wake on interrupt\n");
            break;
        default:
            break;
    }
}

void power_management_demo() {
    printf("\n=== Power Management Demo ===\n");
    
    enter_power_mode(POWER_MODE_ACTIVE);
    printf("Performing work...\n");
    
    enter_power_mode(POWER_MODE_IDLE);
    printf("Waiting for interrupt...\n");
    
    enter_power_mode(POWER_MODE_SLEEP);
    printf("Deeper sleep, wake on UART...\n");
    
    enter_power_mode(POWER_MODE_DEEP_SLEEP);
    printf("Minimum power, wake on button...\n");
    
    enter_power_mode(POWER_MODE_ACTIVE);
    printf("Woke up!\n");
}

int main() {
    printf("EMBEDDED SYSTEMS PROGRAMMING EXAMPLES\n");
    printf("=====================================\n");
    
    gpio_demo();
    uart_demo();
    timer_demo();
    state_machine_demo();
    adc_demo();
    interrupt_demo();
    memory_pool_demo();
    watchdog_demo();
    power_management_demo();
    
    printf("\nAll demos completed!\n");
    
    return 0;
}