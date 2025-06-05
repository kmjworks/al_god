# Embedded Systems Comprehensive Cheat Sheet - C Programming

## What are Embedded Systems?
Embedded systems are specialized computing systems that perform dedicated functions within larger mechanical or electrical systems. They are characterized by:
- Limited resources (memory, processing power)
- Real-time constraints
- Direct hardware interaction
- Specific, dedicated functionality
- Low power consumption requirements

## Key Characteristics

### 1. Resource Constraints
- **Limited RAM**: Often KB instead of GB
- **Limited ROM/Flash**: Program storage measured in KB-MB
- **Processing Power**: MHz instead of GHz
- **No OS or RTOS**: Often bare-metal programming

### 2. Real-time Requirements
- **Hard Real-time**: Missing deadline causes system failure
- **Soft Real-time**: Missing deadline degrades performance
- **Deterministic Behavior**: Predictable execution time

### 3. Hardware Interface
- **Direct Register Access**: Manipulating hardware through memory-mapped I/O
- **Interrupts**: Hardware events that need immediate attention
- **Peripherals**: UART, SPI, I2C, ADC, PWM, etc.

## Memory-Mapped I/O and Register Access

### Basic Register Operations
```c
// Define register addresses
#define GPIO_BASE_ADDR  0x40020000
#define GPIO_DATA_REG   (*(volatile uint32_t*)(GPIO_BASE_ADDR + 0x00))
#define GPIO_DIR_REG    (*(volatile uint32_t*)(GPIO_BASE_ADDR + 0x04))
#define GPIO_CTRL_REG   (*(volatile uint32_t*)(GPIO_BASE_ADDR + 0x08))

// Set pin as output
GPIO_DIR_REG |= (1 << 5);  // Set bit 5

// Write to pin
GPIO_DATA_REG |= (1 << 5);   // Set pin high
GPIO_DATA_REG &= ~(1 << 5);  // Set pin low

// Toggle pin
GPIO_DATA_REG ^= (1 << 5);

// Read pin state
if (GPIO_DATA_REG & (1 << 5)) {
    // Pin is high
}
```

### Structure-based Register Access
```c
// Define peripheral structure
typedef struct {
    volatile uint32_t DATA;
    volatile uint32_t DIR;
    volatile uint32_t CTRL;
    volatile uint32_t STATUS;
} GPIO_TypeDef;

// Create pointer to peripheral
#define GPIO_A ((GPIO_TypeDef*)0x40020000)
#define GPIO_B ((GPIO_TypeDef*)0x40020400)

// Use structure members
GPIO_A->DIR |= (1 << 5);
GPIO_A->DATA |= (1 << 5);

// More readable with bit definitions
#define PIN_5 (1 << 5)
GPIO_A->DIR |= PIN_5;
GPIO_A->DATA |= PIN_5;
```

## Bit Manipulation

### Common Bit Operations
```c
// Bit masks and operations
#define BIT(x) (1 << (x))
#define SET_BIT(reg, bit) ((reg) |= BIT(bit))
#define CLEAR_BIT(reg, bit) ((reg) &= ~BIT(bit))
#define TOGGLE_BIT(reg, bit) ((reg) ^= BIT(bit))
#define CHECK_BIT(reg, bit) ((reg) & BIT(bit))

// Multi-bit field operations
#define MASK(bits) ((1 << (bits)) - 1)
#define SET_FIELD(reg, mask, shift, value) \
    ((reg) = ((reg) & ~((mask) << (shift))) | (((value) & (mask)) << (shift)))

// Example: Set 3-bit field at position 4 to value 5
// Bits: [7][6][5][4][3][2][1][0]
//            xxx (field at position 4-6)
uint8_t reg = 0b11111111;
SET_FIELD(reg, 0x07, 4, 5);  // reg = 0b11010111
```

### Bit Field Structures
```c
// Hardware register with bit fields
typedef union {
    uint32_t raw;
    struct {
        uint32_t enable : 1;      // Bit 0
        uint32_t mode : 2;        // Bits 1-2
        uint32_t speed : 3;       // Bits 3-5
        uint32_t reserved : 2;    // Bits 6-7
        uint32_t data : 8;        // Bits 8-15
        uint32_t status : 16;     // Bits 16-31
    } bits;
} ControlRegister;

// Usage
volatile ControlRegister* ctrl = (ControlRegister*)0x40000000;
ctrl->bits.enable = 1;
ctrl->bits.mode = 2;
ctrl->bits.speed = 5;
```

## Volatile Keyword

### Why Volatile is Critical
```c
// Without volatile - compiler may optimize away
uint32_t* status_reg = (uint32_t*)0x40000004;
while (*status_reg & 0x01) {
    // Compiler might read once and cache
}

// With volatile - forces fresh read each time
volatile uint32_t* status_reg = (volatile uint32_t*)0x40000004;
while (*status_reg & 0x01) {
    // Reads from hardware each iteration
}
```

### Volatile Usage Examples
```c
// Hardware registers
volatile uint32_t* TIMER_COUNT = (volatile uint32_t*)0x40001000;

// Shared variables modified by ISR
volatile uint8_t flag = 0;
volatile uint32_t tick_count = 0;

// ISR (Interrupt Service Routine)
void TIMER_IRQ_Handler(void) {
    tick_count++;
    flag = 1;
}

// Main code
int main(void) {
    while (1) {
        if (flag) {
            flag = 0;
            // Process event
        }
    }
}
```

## Interrupts and ISRs

### Basic Interrupt Handling
```c
// Interrupt vector table (ARM Cortex-M example)
typedef void (*interrupt_handler_t)(void);

// Vector table structure
typedef struct {
    uint32_t* initial_sp;
    interrupt_handler_t reset_handler;
    interrupt_handler_t nmi_handler;
    interrupt_handler_t hardfault_handler;
    // ... more handlers
    interrupt_handler_t timer0_handler;
    interrupt_handler_t uart0_handler;
} vector_table_t;

// ISR implementation
void __attribute__((interrupt)) TIMER0_IRQHandler(void) {
    // Clear interrupt flag
    TIMER0->STATUS = TIMER_INT_CLEAR;
    
    // Handle interrupt
    system_tick++;
    
    // Keep ISR short!
}

// Enable/disable interrupts
#define ENABLE_INTERRUPTS()  __asm__("cpsie i")
#define DISABLE_INTERRUPTS() __asm__("cpsid i")

// Critical section
void critical_function(void) {
    uint32_t primask = __get_PRIMASK();
    DISABLE_INTERRUPTS();
    
    // Critical code here
    shared_variable++;
    
    __set_PRIMASK(primask);  // Restore interrupt state
}
```

### Interrupt Priorities and Nesting
```c
// Interrupt controller registers (NVIC for ARM)
#define NVIC_BASE       0xE000E100
#define NVIC_ISER(n)    (*(volatile uint32_t*)(NVIC_BASE + 0x00 + 4*(n)))
#define NVIC_ICER(n)    (*(volatile uint32_t*)(NVIC_BASE + 0x80 + 4*(n)))
#define NVIC_IPR(n)     (*(volatile uint8_t*)(NVIC_BASE + 0x300 + (n)))

// Configure interrupt
void configure_interrupt(IRQn_Type irq, uint8_t priority) {
    // Set priority (0 = highest)
    NVIC_IPR(irq) = priority << 4;
    
    // Enable interrupt
    NVIC_ISER(irq / 32) = 1 << (irq % 32);
}
```

## Common Peripherals

### 1. GPIO (General Purpose I/O)
```c
typedef struct {
    volatile uint32_t MODER;    // Mode register
    volatile uint32_t OTYPER;   // Output type
    volatile uint32_t OSPEEDR;  // Output speed
    volatile uint32_t PUPDR;    // Pull-up/down
    volatile uint32_t IDR;      // Input data
    volatile uint32_t ODR;      // Output data
    volatile uint32_t BSRR;     // Bit set/reset
} GPIO_TypeDef;

// Initialize GPIO pin
void gpio_init_output(GPIO_TypeDef* port, uint8_t pin) {
    // Set as output (2 bits per pin)
    port->MODER &= ~(3 << (pin * 2));
    port->MODER |= (1 << (pin * 2));
    
    // Push-pull output
    port->OTYPER &= ~(1 << pin);
    
    // High speed
    port->OSPEEDR |= (3 << (pin * 2));
}

// Efficient pin control
#define GPIO_SET_PIN(port, pin)   ((port)->BSRR = (1 << (pin)))
#define GPIO_RESET_PIN(port, pin) ((port)->BSRR = (1 << ((pin) + 16)))
```

### 2. UART (Serial Communication)
```c
typedef struct {
    volatile uint32_t DR;       // Data register
    volatile uint32_t SR;       // Status register
    volatile uint32_t CR1;      // Control register 1
    volatile uint32_t CR2;      // Control register 2
    volatile uint32_t BRR;      // Baud rate register
} UART_TypeDef;

// UART initialization
void uart_init(UART_TypeDef* uart, uint32_t baudrate) {
    // Calculate baud rate divisor
    uint32_t clock = 16000000;  // 16 MHz
    uint32_t divisor = clock / baudrate;
    
    uart->BRR = divisor;
    uart->CR1 = UART_CR1_TE | UART_CR1_RE | UART_CR1_UE;
}

// Send character
void uart_putc(UART_TypeDef* uart, char c) {
    while (!(uart->SR & UART_SR_TXE));  // Wait for empty
    uart->DR = c;
}

// Receive character
char uart_getc(UART_TypeDef* uart) {
    while (!(uart->SR & UART_SR_RXNE)); // Wait for data
    return uart->DR;
}
```

### 3. Timer/Counter
```c
typedef struct {
    volatile uint32_t CR1;      // Control register 1
    volatile uint32_t CR2;      // Control register 2
    volatile uint32_t SR;       // Status register
    volatile uint32_t CNT;      // Counter
    volatile uint32_t PSC;      // Prescaler
    volatile uint32_t ARR;      // Auto-reload
} TIM_TypeDef;

// Configure timer for 1ms tick
void timer_init_1ms(TIM_TypeDef* tim) {
    tim->PSC = 16000 - 1;   // Prescaler (16MHz / 16000 = 1kHz)
    tim->ARR = 1000 - 1;    // Count to 1000 (1 second)
    tim->CR1 = TIM_CR1_CEN; // Enable counter
}

// Delay function using timer
void delay_ms(uint32_t ms) {
    uint32_t start = TIM2->CNT;
    while ((TIM2->CNT - start) < ms);
}
```

## Memory Management in Embedded Systems

### Static Memory Allocation
```c
// Prefer static allocation in embedded systems
#define MAX_BUFFERS 10
#define BUFFER_SIZE 256

// Static buffers
static uint8_t buffers[MAX_BUFFERS][BUFFER_SIZE];
static uint8_t buffer_used[MAX_BUFFERS] = {0};

// Simple allocator
void* buffer_alloc(void) {
    for (int i = 0; i < MAX_BUFFERS; i++) {
        if (!buffer_used[i]) {
            buffer_used[i] = 1;
            return buffers[i];
        }
    }
    return NULL;
}

void buffer_free(void* ptr) {
    for (int i = 0; i < MAX_BUFFERS; i++) {
        if (ptr == buffers[i]) {
            buffer_used[i] = 0;
            break;
        }
    }
}
```

### Memory Pool
```c
// Fixed-size block allocator
typedef struct block {
    struct block* next;
} block_t;

typedef struct {
    block_t* free_list;
    uint8_t* memory;
    size_t block_size;
    size_t num_blocks;
} memory_pool_t;

void pool_init(memory_pool_t* pool, void* memory, 
               size_t block_size, size_t num_blocks) {
    pool->memory = (uint8_t*)memory;
    pool->block_size = block_size;
    pool->num_blocks = num_blocks;
    pool->free_list = (block_t*)memory;
    
    // Link all blocks
    for (size_t i = 0; i < num_blocks - 1; i++) {
        block_t* current = (block_t*)(pool->memory + i * block_size);
        block_t* next = (block_t*)(pool->memory + (i + 1) * block_size);
        current->next = next;
    }
    
    // Last block
    block_t* last = (block_t*)(pool->memory + (num_blocks - 1) * block_size);
    last->next = NULL;
}

void* pool_alloc(memory_pool_t* pool) {
    if (pool->free_list == NULL) {
        return NULL;
    }
    
    block_t* block = pool->free_list;
    pool->free_list = block->next;
    return block;
}

void pool_free(memory_pool_t* pool, void* ptr) {
    block_t* block = (block_t*)ptr;
    block->next = pool->free_list;
    pool->free_list = block;
}
```

## Power Management

### Low Power Modes
```c
// Sleep modes (ARM Cortex-M)
void enter_sleep_mode(void) {
    // Wait for interrupt
    __WFI();
}

void enter_deep_sleep(void) {
    // Set deep sleep bit
    SCB->SCR |= SCB_SCR_SLEEPDEEP;
    
    // Enter sleep
    __WFI();
    
    // Clear deep sleep bit
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP;
}

// Peripheral clock gating
void enable_peripheral_clock(uint32_t peripheral) {
    RCC->APB1ENR |= peripheral;
}

void disable_peripheral_clock(uint32_t peripheral) {
    RCC->APB1ENR &= ~peripheral;
}
```

## Real-Time Constraints

### Measuring Execution Time
```c
// Cycle counter (ARM Cortex-M)
#define DWT_CTRL    (*(volatile uint32_t*)0xE0001000)
#define DWT_CYCCNT  (*(volatile uint32_t*)0xE0001004)

void enable_cycle_counter(void) {
    DWT_CTRL |= 1;
}

uint32_t get_cycles(void) {
    return DWT_CYCCNT;
}

// Measure function execution time
void measure_function(void) {
    uint32_t start = get_cycles();
    
    // Function to measure
    critical_function();
    
    uint32_t cycles = get_cycles() - start;
    printf("Execution time: %u cycles\n", cycles);
}
```

### Watchdog Timer
```c
typedef struct {
    volatile uint32_t KR;   // Key register
    volatile uint32_t PR;   // Prescaler
    volatile uint32_t RLR;  // Reload
    volatile uint32_t SR;   // Status
} IWDG_TypeDef;

#define IWDG ((IWDG_TypeDef*)0x40003000)

// Initialize watchdog
void watchdog_init(uint32_t timeout_ms) {
    IWDG->KR = 0x5555;  // Enable access
    IWDG->PR = 4;       // Prescaler /64
    IWDG->RLR = timeout_ms * 40000 / 64000;  // Calculate reload
    IWDG->KR = 0xCCCC;  // Start watchdog
}

// Feed watchdog
void watchdog_feed(void) {
    IWDG->KR = 0xAAAA;
}
```

## State Machines

### Simple State Machine
```c
typedef enum {
    STATE_IDLE,
    STATE_INIT,
    STATE_RUNNING,
    STATE_ERROR
} state_t;

typedef struct {
    state_t current_state;
    uint32_t error_code;
    void (*state_handlers[4])(void);
} state_machine_t;

void handle_idle(void);
void handle_init(void);
void handle_running(void);
void handle_error(void);

state_machine_t machine = {
    .current_state = STATE_IDLE,
    .error_code = 0,
    .state_handlers = {
        handle_idle,
        handle_init,
        handle_running,
        handle_error
    }
};

void state_machine_run(void) {
    if (machine.current_state < 4) {
        machine.state_handlers[machine.current_state]();
    }
}
```

## Communication Protocols

### SPI Master
```c
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SR;
    volatile uint32_t DR;
} SPI_TypeDef;

uint8_t spi_transfer(SPI_TypeDef* spi, uint8_t data) {
    while (!(spi->SR & SPI_SR_TXE));  // Wait TX empty
    spi->DR = data;
    while (!(spi->SR & SPI_SR_RXNE)); // Wait RX ready
    return spi->DR;
}
```

### I2C Master
```c
void i2c_start(I2C_TypeDef* i2c, uint8_t addr, uint8_t direction) {
    i2c->CR1 |= I2C_CR1_START;
    while (!(i2c->SR1 & I2C_SR1_SB));
    
    i2c->DR = (addr << 1) | direction;
    while (!(i2c->SR1 & I2C_SR1_ADDR));
    
    // Clear ADDR flag
    (void)i2c->SR1;
    (void)i2c->SR2;
}
```

## Best Practices

1. **Use const for ROM data**: `const uint8_t table[] = {1,2,3};`
2. **Minimize stack usage**: Avoid large local variables
3. **Avoid dynamic allocation**: Use static or stack allocation
4. **Keep ISRs short**: Do minimal work, use flags
5. **Use volatile correctly**: For hardware registers and ISR-shared data
6. **Profile power consumption**: Use sleep modes when idle
7. **Handle all error cases**: Embedded systems must be robust
8. **Use fixed-point math**: Avoid floating point when possible

## Quick Reference

| Concept | Usage | Example |
|---------|-------|---------|
| Register Access | `volatile` pointer | `*(volatile uint32_t*)0x4000` |
| Bit Set | OR with mask | `reg \|= (1 << bit)` |
| Bit Clear | AND with inverted mask | `reg &= ~(1 << bit)` |
| Bit Toggle | XOR with mask | `reg ^= (1 << bit)` |
| Bit Check | AND with mask | `if (reg & (1 << bit))` |
| Critical Section | Disable interrupts | `__disable_irq()` |
| Memory Barrier | Prevent reordering | `__DMB()` |

## Exam Tips
1. Always use `volatile` for hardware registers
2. Keep ISRs short and fast
3. Understand bit manipulation thoroughly
4. Know memory-mapped I/O concepts
5. Be familiar with common peripherals (GPIO, UART, Timer)
6. Understand interrupt priorities and nesting
7. Know power-saving techniques
8. Static allocation is preferred over dynamic in embedded systems