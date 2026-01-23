#ifndef TASK9_H
#define TASK9_H

#define TIMEOUT 5

typedef struct {
    int* data;                 // Пакет данных в кольцевом буфере
    int head;                  // Начало очереди
    int tail;                  // Конец очереди
    int size;                  // Размер очереди
    int capacity;              // Предельное число данных в очереди
    size_t total_processed;    // Всего обработано
    size_t total_dropped;      // Потеряно при переполнении
    double avg_wait_time;      // Среднее время в очереди
    int max_queue_size;        // Максимальная длина за всё время
    time_t peak_time;          // Когда была максимальная нагрузка
} Queue;

typedef enum {
    PACKET_ACK,                 // Короткий (1 мс)
    PACKET_DATA,                // Средний (10 мс)
    PACKET_STREAM,              // Длинный (100 мс)
    PACKET_PRIORITY             // Обслуживается вне очереди
} PacketType;

#endif