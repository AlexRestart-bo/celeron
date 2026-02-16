#include "main.h"

Matrix* create_matrix(void){
    Matrix* mtx = (Matrix*)malloc(sizeof(Matrix));
    if (mtx == NULL) return NULL;
    mtx->rows = 1;
    mtx->nums = (int**)malloc(mtx->rows*sizeof(int*));
    if (mtx->nums == NULL) {
        free(mtx);
        return NULL;
    }
    mtx->cols = 1;
    mtx->nums[0] = (int*)malloc(mtx->cols*sizeof(int));
    if (mtx->nums[0] == NULL) {
        free(mtx->nums);
        free(mtx);
        return NULL;
    }
    mtx->min_size_line = 0;
    return mtx;
}

void free_array(int** mtx, int rows){
    for (int i = 0; i < rows; i++){
        if (mtx[i] != NULL) free(mtx[i]);
    }
    free(mtx);
}
/**
 * Функция списана с convert_series(), однако приспособлена под матрицы.
*/
void add_line_to_matrix(Matrix* mtx, const char* data_char, const int size_data, const char symbol){
    static bool init_params = false;    // в первое исполнение функции матрица уже исеет одну строку, поэтому не нужно выполнять realloc()
    if (init_params){   // чтобы mtx->rows всегда показывал верное количество строк в массиве, он увеличивается с начале
                        // функции, если только это не первое ее исполнение
        int** tm = (int**)realloc(mtx->nums, (++mtx->rows)*sizeof(int*));
        if (tm == NULL) return;
        mtx->nums = tm;
    }
    if (mtx->cols == 0)     // если последовательность пустая, то необходимо выделить память хотя бы для одного элемента
        mtx->nums[mtx->rows - 1] = (int*)malloc(++mtx->cols*sizeof(int));
    if (mtx->nums[mtx->rows - 1] == NULL) {
        printf("Happen error with selecting memory in function \"add_line_to_matrix\"\n");
        return;
    }

    int i = 0;
    int how_long = 0;
    
    int weight_num = 1;
    char* strnum = malloc(weight_num*sizeof(char));
    if (strnum == NULL) return;
    /** Счетчик counter используется для записи символов в строку strnum, которая перезаписывается
     * каждый раз после встречи symbol в строке data_char.
    */
    int counter = 0;
    while(i < size_data){
        // В конце строки обычно не ставят пробел, поетому добавлена проверка на нуль-терминатор,
        // чтобы не отбросить последнее число.
        if (data_char[i] == symbol || data_char[i] == '\0') {
            // добавляем память, только если нехватает 
            if(how_long >= mtx->cols) {
                int* tmp_nums = (int*)realloc(mtx->nums[mtx->rows - 1], (++mtx->cols)*sizeof(int)); 
                if (tmp_nums == NULL) {
                    free(strnum);
                    return;
                }
                mtx->nums[mtx->rows - 1] = tmp_nums;
            }
            mtx->nums[mtx->rows - 1][how_long++] = string_to_int(strnum, counter);
            counter = 0;
        }else{
            if (counter < weight_num -1){
                char* tmp = realloc(strnum, weight_num++*sizeof(char)); 
                if(tmp == NULL) {
                    free(strnum);
                    return;
                }
                strnum = tmp;
            }
            strnum[counter++] = data_char[i];
        }
        i++;
    }
    if (mtx->min_size_line <= 0)        // min_size_line содержит информацию о ширине матрицы, все остальные числа отбрасываются
        mtx->min_size_line = mtx->cols;
    else if (mtx->min_size_line > mtx->cols)
        mtx->min_size_line = mtx->cols;
    mtx->cols = 0;                      // показатель того что нужно сделать malloc()
    init_params = true;
    free(strnum);
}

/* matrix должна иметь rows строк и cols рядов*/
int** transpose_matrix(int** matrix, int rows, int cols){
    int** new_matrix = (int**)malloc(cols*sizeof(int*));
    for (int i = 0; i < cols; i++){
        new_matrix[i] = (int*)malloc(rows*sizeof(int));
        if (new_matrix[i] == NULL){
            for (int j = i - 1; j >= 0; j--)
                free(new_matrix[j]);
            free(new_matrix);
            printf("Ошибка выделения памяти под новую матрицу\n");
            return NULL;
        }
    }
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++)
            new_matrix[j][i] = matrix[i][j];
    }
    return new_matrix;
}

// Узнаем лимит стека
void check_stack_limit(int depth) {
    char dummy;
    printf("Глубина: %d, Адрес переменной: %p\n", depth, &dummy);
    
    if (depth >= 0) {
        check_stack_limit(depth + 1);
    }
}

void print_matrix(int** matrix, int rows, int cols){
    printf("\nMatrix:");
    for (int i = 0; i < rows; i++){
        printf("\n");
        for (int j = 0; j < cols; j++)
            printf("%i\t", matrix[i][j]);
    }
}

void free_matrix(Matrix* matrix){
    for (int i = 0; i < matrix->rows; i++){
        if (matrix->nums[i] != NULL) free(matrix->nums[i]);
    }
    free(matrix->nums);
    free(matrix);
}

void task12(void){
    int init_inbuf = 1;
    int* mtx_sizes = malloc(init_inbuf*sizeof(int));
    if(mtx_sizes == NULL) {
        msg_error_sellecting_mem("mtx_sizes");
        return;
    }

    int basic_new = 1;
    char* new_input = malloc(basic_new*sizeof(char));
    if(new_input == NULL){
        msg_error_sellecting_mem("new_input");
        free(mtx_sizes);
        return;
    }
    // ввод размеров массива пользователем
    printf("Enter sizes of your matrix (every size must be a natural number)\n");
    read_line(&new_input, &basic_new);
    convert_series(&mtx_sizes, &init_inbuf, new_input, basic_new, ' ');

    if(init_inbuf < 2){
        printf("Errror, your data (sizes of matrix) is incorrect!\n");
        free(new_input);
        free(mtx_sizes);
        return;
    }
    else if(mtx_sizes[0] < 1 || mtx_sizes[1] < 1){
        printf("Errror, your data (sizes of matrix) is incorrect!\n");
        free(new_input);
        free(mtx_sizes);
        return;
    }

    const int rows = mtx_sizes[0];
    const int cols = mtx_sizes[1];

    free(mtx_sizes);

    Matrix* mtx = create_matrix();
    printf("Enter your matrix line by line\n");
    for (int i = 0; i < rows; i++){
        read_line(&new_input, &basic_new);
        add_line_to_matrix(mtx, new_input, basic_new, ' ');
    }
    free(new_input);

    int** trmtx = transpose_matrix(mtx->nums, rows, cols);
    print_matrix(mtx->nums, rows, cols);
    print_matrix(trmtx, cols, rows);

    free_array(trmtx, cols);
    free_matrix(mtx);
}