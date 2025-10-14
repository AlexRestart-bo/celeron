#include <stdio.h>
#include <stdlib.h>

void change_struct_1(ts1 *mer){
    mer->nano = realloc(mer->nano, 5*sizeof(mer->nano));
    mer->nano = "Komb";
}

void return_struct(ts1 *mer){
    printf("suzeof(struct->field1) = %zu\n", sizeof(mer->nano));
}