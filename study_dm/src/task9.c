#include <main.h>

unsigned int busy_time = 0;
unsigned int n = 0;

unsigned int rand_delay(unsigned int r){
    srand(time(NULL));
    return (rand() % r);
}

void msleep(int milliseconds) {
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;           // Секунды
    ts.tv_nsec = (milliseconds % 1000) * 1000000L;  // Наносекунды
    nanosleep(&ts, NULL);
}

void compute_average_time(Queue* queue){
    busy_time = queue->size*TIMEOUT;
    queue->avg_wait_time = (queue->avg_wait_time * n + (double)busy_time) / (n + 1);
    n++;
}

void queue_visualize(Queue* queue) {
    if (queue->total_processed % 300 == 0) 
        printf("Загруженность %i%%\tВ очереди %i элементов\tДо завершения %i мс\n"
        "Среднеее время ожидания %f\tПотеряно элементов %lu\n",
        queue->size * 100 / queue->capacity, queue->size, queue->size*TIMEOUT, 
        queue->avg_wait_time, queue->total_dropped);
}

Queue* create_queue(int capacity){
    Queue* tq = (Queue*)malloc(sizeof(Queue));
    tq->data = (int*)calloc(capacity, sizeof(int));
    if (tq->data == NULL){
        printf("Error with creating queue\n");
        return NULL;
    }
    tq->capacity = capacity;
    tq->size = 0;
    tq->head = 0;
    tq->tail = 0;
    tq->avg_wait_time = 0;
    tq->max_queue_size = 0;
    tq->peak_time = 0;
    tq->total_dropped = 0;
    tq->total_processed = 0;
    tq->is_full = false;
    tq->boot = true;
    return tq;
}

/* Если хвост догнал голову, то массив данных прекращает меняться, пока голова не отбежит от хвоста хотя бы на один элемент*/
void* enqueue(void* arg){
    Queue* queue = (Queue*)arg;
    while(queue->boot){
        if (!queue->is_full){
            pthread_mutex_lock(&queue->lock);
            queue->data[queue->tail] = 1;
            queue->tail = (queue->tail + 1) % queue->capacity;  // таким образом, если массив полон, он начнет перезаписываться сначала
            queue->size++;                                      // Размер очереди увеличивается только когда добавляется элемент
            if (queue->max_queue_size < queue->size){
                queue->max_queue_size = queue->size;
                time(&queue->peak_time);
            }
            pthread_mutex_unlock(&queue->lock);
            compute_average_time(queue);
            if (queue->tail == queue->head) queue->is_full = true;
        }else{
            queue->total_dropped++;                             // из-за переполнения данные не попадут в очередь
            if(queue->tail != queue->head) queue->is_full = false;
        }
        msleep(rand_delay(TIMEOUT*2 + 1));
    }
    return NULL;
}

/* Если голова догнала хвост, то очередь пуста и ничего не обрабатывается */
void* dequeue(void* arg){
    Queue* queue = (Queue*)arg;
    while(queue->boot){
        if (!queue->is_empty){
            pthread_mutex_lock(&queue->lock);
            queue->data[queue->head]++;
            queue->head = (queue->head + 1) % queue->capacity;
            queue->size--;                                      // Размер очереди уменьшается только когда обрабатывается элемент
            pthread_mutex_unlock(&queue->lock);
            compute_average_time(queue);
            queue_visualize(queue);
            if (queue->head == queue->tail) queue->is_empty = true; 
            msleep(TIMEOUT);                                     // Возможно, следует это время сделать разным для разных элементов очереди
            queue->total_processed++;
            if(queue->total_processed >= 100000) queue->boot = false;
        }else{
            if (queue->head != queue->tail) queue->is_empty = false;        // Если данные вновь поступили, то ставим в очередь
        }
    }
    return NULL;
}

void task9(){
    Queue* requests = create_queue(200);
    pthread_mutex_init(&requests->lock, NULL);
    pthread_t thread1;
    pthread_t thread2;
    pthread_create(&thread1, NULL, enqueue, (void*)requests);
    pthread_create(&thread2, NULL, dequeue, (void*)requests);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    for (int i = 0; i < 10000000; i++);
    pthread_mutex_destroy(&requests->lock);
}