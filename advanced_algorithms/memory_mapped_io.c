#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>

// 1. Basic memory-mapped file reading
void demonstrate_mmap_read() {
    printf("=== Memory-Mapped File Reading ===\n");
    
    // Create a test file
    const char *filename = "test_mmap.txt";
    FILE *fp = fopen(filename, "w");
    fprintf(fp, "Hello, memory-mapped world!\nThis is line 2.\nAnd this is line 3.");
    fclose(fp);
    
    // Open file for memory mapping
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return;
    }
    
    // Get file size
    struct stat sb;
    if (fstat(fd, &sb) < 0) {
        perror("fstat");
        close(fd);
        return;
    }
    
    // Memory map the file
    char *mapped = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return;
    }
    
    // Read data directly from memory
    printf("File contents via mmap:\n");
    write(STDOUT_FILENO, mapped, sb.st_size);
    printf("\n\n");
    
    // Count characters without traditional I/O
    int char_count = 0;
    for (off_t i = 0; i < sb.st_size; i++) {
        if (mapped[i] != ' ' && mapped[i] != '\n') {
            char_count++;
        }
    }
    printf("Non-space character count: %d\n", char_count);
    
    // Cleanup
    munmap(mapped, sb.st_size);
    close(fd);
    unlink(filename);
}

// 2. Memory-mapped file writing
void demonstrate_mmap_write() {
    printf("\n=== Memory-Mapped File Writing ===\n");
    
    const char *filename = "test_mmap_write.bin";
    size_t file_size = 1024; // 1KB file
    
    // Create and resize file
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        perror("open");
        return;
    }
    
    // Extend file to desired size
    if (ftruncate(fd, file_size) < 0) {
        perror("ftruncate");
        close(fd);
        return;
    }
    
    // Memory map for writing
    char *mapped = mmap(NULL, file_size, PROT_READ | PROT_WRITE, 
                        MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return;
    }
    
    // Write data directly to memory
    strcpy(mapped, "Header: Binary Data File\n");
    
    // Write binary data
    int *int_data = (int *)(mapped + 32);
    for (int i = 0; i < 10; i++) {
        int_data[i] = i * i;
    }
    
    // Write structured data
    typedef struct {
        int id;
        float value;
        char name[20];
    } Record;
    
    Record *records = (Record *)(mapped + 128);
    for (int i = 0; i < 5; i++) {
        records[i].id = 100 + i;
        records[i].value = 3.14f * i;
        snprintf(records[i].name, 20, "Record_%d", i);
    }
    
    // Force write to disk
    if (msync(mapped, file_size, MS_SYNC) < 0) {
        perror("msync");
    }
    
    printf("Written data to memory-mapped file\n");
    
    // Read back to verify
    printf("Header: %s", mapped);
    printf("Integer squares: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", int_data[i]);
    }
    printf("\n");
    
    printf("Records:\n");
    for (int i = 0; i < 5; i++) {
        printf("  ID: %d, Value: %.2f, Name: %s\n",
               records[i].id, records[i].value, records[i].name);
    }
    
    // Cleanup
    munmap(mapped, file_size);
    close(fd);
    unlink(filename);
}

// 3. Shared memory between processes
void demonstrate_shared_memory() {
    printf("\n=== Shared Memory Between Processes ===\n");
    
    const char *shm_name = "/test_shared_mem";
    size_t shm_size = 4096;
    
    // Create shared memory
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        perror("shm_open");
        return;
    }
    
    // Set size
    if (ftruncate(shm_fd, shm_size) < 0) {
        perror("ftruncate");
        close(shm_fd);
        shm_unlink(shm_name);
        return;
    }
    
    // Map shared memory
    void *shm_ptr = mmap(NULL, shm_size, PROT_READ | PROT_WRITE,
                         MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        shm_unlink(shm_name);
        return;
    }
    
    // Use shared memory as a circular buffer
    typedef struct {
        int write_index;
        int read_index;
        int buffer[100];
    } CircularBuffer;
    
    CircularBuffer *cb = (CircularBuffer *)shm_ptr;
    cb->write_index = 0;
    cb->read_index = 0;
    
    // Simulate producer
    printf("Producer writing to shared memory:\n");
    for (int i = 0; i < 10; i++) {
        cb->buffer[cb->write_index] = i * 10;
        printf("Wrote: %d at index %d\n", i * 10, cb->write_index);
        cb->write_index = (cb->write_index + 1) % 100;
    }
    
    // Simulate consumer
    printf("\nConsumer reading from shared memory:\n");
    while (cb->read_index != cb->write_index) {
        printf("Read: %d from index %d\n", 
               cb->buffer[cb->read_index], cb->read_index);
        cb->read_index = (cb->read_index + 1) % 100;
    }
    
    // Cleanup
    munmap(shm_ptr, shm_size);
    close(shm_fd);
    shm_unlink(shm_name);
}

// 4. Memory-mapped array operations
void demonstrate_mmap_array() {
    printf("\n=== Memory-Mapped Array Operations ===\n");
    
    const char *filename = "large_array.dat";
    size_t array_size = 1000000; // 1 million integers
    size_t file_size = array_size * sizeof(int);
    
    // Create file
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        perror("open");
        return;
    }
    
    // Extend file
    if (ftruncate(fd, file_size) < 0) {
        perror("ftruncate");
        close(fd);
        return;
    }
    
    // Map as integer array
    int *array = mmap(NULL, file_size, PROT_READ | PROT_WRITE,
                      MAP_SHARED, fd, 0);
    if (array == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return;
    }
    
    // Initialize array
    printf("Initializing large array...\n");
    for (size_t i = 0; i < array_size; i++) {
        array[i] = i % 1000;
    }
    
    // Perform operations directly on mapped memory
    long long sum = 0;
    for (size_t i = 0; i < array_size; i++) {
        sum += array[i];
    }
    printf("Sum of array elements: %lld\n", sum);
    
    // Find max element
    int max = array[0];
    size_t max_index = 0;
    for (size_t i = 1; i < array_size; i++) {
        if (array[i] > max) {
            max = array[i];
            max_index = i;
        }
    }
    printf("Max element: %d at index %zu\n", max, max_index);
    
    // Modify a portion
    printf("Modifying middle section...\n");
    for (size_t i = array_size/2 - 100; i < array_size/2 + 100; i++) {
        array[i] = -array[i];
    }
    
    // Cleanup
    munmap(array, file_size);
    close(fd);
    unlink(filename);
}

// 5. Copy-on-write demonstration
void demonstrate_cow() {
    printf("\n=== Copy-on-Write Memory Mapping ===\n");
    
    const char *filename = "cow_test.dat";
    size_t size = 4096;
    
    // Create file with initial data
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        perror("open");
        return;
    }
    
    char initial_data[4096];
    memset(initial_data, 'A', sizeof(initial_data));
    write(fd, initial_data, size);
    
    // Create private mapping (copy-on-write)
    char *private_map = mmap(NULL, size, PROT_READ | PROT_WRITE,
                            MAP_PRIVATE, fd, 0);
    if (private_map == MAP_FAILED) {
        perror("mmap private");
        close(fd);
        return;
    }
    
    // Create shared mapping
    char *shared_map = mmap(NULL, size, PROT_READ | PROT_WRITE,
                           MAP_SHARED, fd, 0);
    if (shared_map == MAP_FAILED) {
        perror("mmap shared");
        munmap(private_map, size);
        close(fd);
        return;
    }
    
    printf("Initial state:\n");
    printf("Private map[0]: %c\n", private_map[0]);
    printf("Shared map[0]: %c\n", shared_map[0]);
    
    // Modify private mapping (triggers COW)
    private_map[0] = 'B';
    printf("\nAfter modifying private map:\n");
    printf("Private map[0]: %c\n", private_map[0]);
    printf("Shared map[0]: %c\n", shared_map[0]);
    
    // Modify shared mapping
    shared_map[1] = 'C';
    printf("\nAfter modifying shared map:\n");
    printf("Private map[1]: %c (unchanged)\n", private_map[1]);
    printf("Shared map[1]: %c (changed)\n", shared_map[1]);
    
    // Cleanup
    munmap(private_map, size);
    munmap(shared_map, size);
    close(fd);
    unlink(filename);
}

// 6. Advanced: Ring buffer using mmap
typedef struct {
    size_t size;
    size_t write_pos;
    size_t read_pos;
    char data[];
} RingBuffer;

RingBuffer* create_ring_buffer(size_t size) {
    // Allocate memory for the ring buffer
    size_t total_size = sizeof(RingBuffer) + size;
    
    // Create anonymous mapping
    RingBuffer *rb = mmap(NULL, total_size, PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (rb == MAP_FAILED) {
        return NULL;
    }
    
    rb->size = size;
    rb->write_pos = 0;
    rb->read_pos = 0;
    
    return rb;
}

void ring_buffer_write(RingBuffer *rb, const char *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        rb->data[rb->write_pos] = data[i];
        rb->write_pos = (rb->write_pos + 1) % rb->size;
    }
}

size_t ring_buffer_read(RingBuffer *rb, char *buffer, size_t max_len) {
    size_t count = 0;
    while (count < max_len && rb->read_pos != rb->write_pos) {
        buffer[count++] = rb->data[rb->read_pos];
        rb->read_pos = (rb->read_pos + 1) % rb->size;
    }
    return count;
}

void demonstrate_ring_buffer() {
    printf("\n=== Memory-Mapped Ring Buffer ===\n");
    
    RingBuffer *rb = create_ring_buffer(256);
    if (!rb) {
        perror("create_ring_buffer");
        return;
    }
    
    // Write data
    const char *msg1 = "Hello, ";
    const char *msg2 = "Ring Buffer!";
    ring_buffer_write(rb, msg1, strlen(msg1));
    ring_buffer_write(rb, msg2, strlen(msg2));
    
    // Read data
    char buffer[100];
    size_t read_len = ring_buffer_read(rb, buffer, sizeof(buffer) - 1);
    buffer[read_len] = '\0';
    
    printf("Read from ring buffer: %s\n", buffer);
    
    // Cleanup
    munmap(rb, sizeof(RingBuffer) + rb->size);
}

int main() {
    printf("=== Memory-Mapped File I/O Examples ===\n\n");
    
    demonstrate_mmap_read();
    demonstrate_mmap_write();
    demonstrate_shared_memory();
    demonstrate_mmap_array();
    demonstrate_cow();
    demonstrate_ring_buffer();
    
    return 0;
}