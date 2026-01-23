#include <main.h>

unsigned int busy_time = 0;
unsigned int n = 0;

unsigned int rand_delay(unsigned int r){
    srand(time(NULL));
    return (rand() % r);
}

void compute_average_time(Queue* queue){
    busy_time = queue->size*TIMEOUT;
    queue->avg_wait_time = (queue->avg_wait_time * n + (double)busy_time) / (n + 1);
    n++;
}

void queue_visualize(Queue* queue) {
    if (queue->total_processed % 1000 == 0) 
        printf("Загруженность %i%%\tВ очереди %i элементов\tДо завершения %i мс\n"
        "Среднеее время ожидания %f\tПотеряно элементов %i\n",
        queue->size * 100 / queue->capacity, queue->size, queue->size*TIMEOUT, 
        queue->avg_wait_time, queue->total_dropped);
}

Queue* create_queue(int capacity){
    Queue* tq = (Queue*)malloc(sizeof(Queue));
    tq->data = (int*)calloc(capacity, sizeof(int));
    if (tq->data == NULL){
        ptintf("Error with creating queue\n");
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
    return tq;
}

/* Если хвост догнал голову, то массив данных прекращает меняться, пока голова не отбежит от хвоста хотя бы на один элемент*/
void enqueue(Queue* queue, int value){
    static bool sur  = false;                               // Флаг запрещает очереди пополняться, если равен true
    if (!sur){
        queue->data[queue->tail] = value;
        queue->tail = (queue->tail + 1) % queue->capacity;  // таким образом, если массив полон, он начнет перезаписываться сначала
        queue->size++;                                      // Размер очереди увеличивается только когда добавляется элемент
        if (queue->max_queue_size < queue->size){
            queue->max_queue_size = queue->size;
            time(&queue->peak_time);
        }
        compute_average_time(queue);
        if (queue->tail == queue->head) sur = true;
    }else{
        queue->total_dropped++;                             // из-за переполнения данные не попадут в очередь
        if(queue->tail != queue->head) sur = false;
    }
    sleep(rand_delay(TIMEOUT*2 + 1));
}

/* Если голова догнала хвост, то очередь пуста и ничего не обрабатывается */
void dequeue(Queue* queue){
    static bool sur = false;
    if (!sur){
        queue->data[queue->head]++;
        queue->head = (queue->head + 1) % queue->capacity;
        queue->size--;                                      // Размер очереди уменьшается только когда обрабатывается элемент
        compute_average_time(queue);
        queue_visualize(queue);
        if (queue->head == queue->tail) sur = true; 
        sleep(TIMEOUT);                                     // Возможно, следует это время сделать разным для разных элементов очереди
        queue->total_processed++;
    }else{
        if (queue->head != queue->tail) sur = false;        // Если данные вновь поступили, то ставим в очередь
    }
}

void task9(){
    Queue* requests = create_queue(200);
    pthread_t thread1, thread2;
}