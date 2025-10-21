#include "main.h"

#define task4

int main(void){
    #ifdef task1

    #define SIZE_EXAMPLE 10000
    int min, max;
    printf("The task1 is begin completed!\n");
    
    int example[SIZE_EXAMPLE];
    
    printf("Calling fill_array...\n");
    fill_array(example, (ui)(sizeof(example)/sizeof(example[0])));
    printf("fill_array completed\n");
    
    FILE *file = fopen("task1.txt", "w");

    printf("Array contents: ");
    for(int i = 0; i < 100; i++) printf("%i ", example[i]);
    printf("\n");
    
    for (int i = 0; i < SIZE_EXAMPLE; i++) {
        fprintf(file, "%i ", example[i]);
        if (i % 20 == 0 && i > 0) fprintf(file, "\n");
    }
    fclose(file);

    printf("Calling search_min_max...\n");
    search_min_max(example, &min, &max, (ui)(sizeof(example)/sizeof(example[0])));
    printf("search_min_max completed\n");
    printf("min = %i\tmax = %i\n", min, max);
    
    printf("Calling sorted_enum...\n");
    sorted_enum(example, (ui)(sizeof(example)/sizeof(example[0])));
    printf("sorted_enum completed\n");
    
    printf("Sorted array: ");
    for(int i = 0; i < 100; i++) printf("%i ", example[i]);
    printf("\n");
    
    printf("Calling average...\n");
    int out_aver = average(example, (ui)(sizeof(example)/sizeof(example[0])));
    printf("average completed\n");
    printf("Average = %i\n", out_aver);
    //solonina();
    
    #endif

    #ifdef task2

    char induce;
    printf("If you want to add student, click \'a\'\n"
        "If you want to read all students, click \'b\'\n"
        "If you want to find the better student, click \'c\'\n"
        "If you want to close the programm, click \'d\'\n");
    char inf = 1;
    while(inf){
        //scanf("%c", &induce);
        induce = (char)getchar();
        while(getchar() != '\n'){}
        if(induce == EOF) break;
        switch(induce){
            case 'a':
                data_entry();
                break;
            case 'b':
                data_output();
                break;
            case 'c':
                search_better();
                break;
            case 'd':
                inf = 0;
                break;
            default:
                printf("Invalid input '%c'. Please enter a, b, c, or d.\n", induce);
                break;
        }
    }

    #endif

    #ifdef task3

    int prim_size = 1;
    int *primary_array = create_array(prim_size);
    //int *primary_array = malloc(prim_size*sizeof(int));
    if (primary_array == NULL) return 0;

    char induce;
    printf("If you want to add row, click \'a\'\n"
        "If you want to read all numbers, click \'b\'\n"
        "If you want to remove the element of array, click \'c\'\n"
        "If you want to close the programm, click \'d\'\n");
    char inf = 1;
    int index;
    while(inf){
        induce = (char)getchar();
        while(getchar() != '\n'){}
        if(induce == EOF) break;
        switch(induce){
            case 'a':
                data_entry_row(&primary_array, &prim_size);
                break;
            case 'b':
                prt_arr(primary_array, prim_size);
                break;
            case 'c':
                scanf("%i", &index);
                while(getchar() != '\n');   //очистка буфера
                if (index >= 0 && index < prim_size)
                    remove_element(&primary_array, &prim_size, index);
                prt_arr(primary_array, prim_size);
                break;
            case 'd':
                inf = 0;
                break;
            default:
                printf("Invalid input '%c'. Please enter a, b, c, or d.\n", induce);
                break;
        }
    }
    free(primary_array);

    #endif

    #ifdef task4



    #endif

    return 0;
}