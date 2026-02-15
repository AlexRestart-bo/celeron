/*
******************************************************************************
**  Файл         : STM32L476.ld
**  Описание     : Линкер-скрипт для STM32L476xx (1024KB Flash, 128KB RAM)
**  Для компилятора: GCC ARM Embedded
******************************************************************************
*/

/* Входная точка - обработчик сброса */
ENTRY(Reset_Handler)

/* Определение областей памяти */
MEMORY
{
    /* Области флеш-памяти */
    FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 1024K  /* Вся флеш-память */
    
    /* Области ОЗУ */
    RAM    (xrw)    : ORIGIN = 0x20000000, LENGTH = 96K    /* SRAM1 */
    RAM2   (xrw)    : ORIGIN = 0x10000000, LENGTH = 32K    /* SRAM2 (отдельная) */
}

/* Размеры кучи (heap) и стека (stack) */
_Min_Heap_Size  = 0x200;   /* 512 байт - минимальный размер кучи */
_Min_Stack_Size = 0x400;   /* 1 КБ - минимальный размер стека */

/* Определение секций */
SECTIONS
{
    /* 1. Секция векторов прерываний - обязательно в начало FLASH */
    .isr_vector :
    {
        . = ALIGN(8);
        KEEP(*(.isr_vector))      /* Таблица векторов */
        . = ALIGN(8);
    } >FLASH

    /* 2. Код программы */
    .text :
    {
        . = ALIGN(8);
        *(.text)                   /* .text секции (код) */
        *(.text*)                  /* .text* секции (код) */
        *(.glue_7)                 /* Обёртки для ARM->Thumb */
        *(.glue_7t)                /* Обёртки для Thumb->ARM */
        *(.eh_frame)
        
        KEEP (*(.init))
        KEEP (*(.fini))
        
        . = ALIGN(8);
        _etext = .;                /* Конец кода */
    } >FLASH

    /* 3. Константные данные (хранятся во FLASH) */
    .rodata :
    {
        . = ALIGN(8);
        *(.rodata)                 /* .rodata секции */
        *(.rodata*)                /* .rodata* секции */
        . = ALIGN(8);
    } >FLASH

    /* 4. Данные для обработки исключений ARM */
    .ARM.extab :
    {
        . = ALIGN(8);
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        . = ALIGN(8);
    } >FLASH
    
    .ARM :
    {
        . = ALIGN(8);
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
        . = ALIGN(8);
    } >FLASH

    /* 5. Таблицы инициализации */
    .preinit_array :
    {
        . = ALIGN(8);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
        . = ALIGN(8);
    } >FLASH
    
    .init_array :
    {
        . = ALIGN(8);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
        . = ALIGN(8);
    } >FLASH
    
    .fini_array :
    {
        . = ALIGN(8);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
        . = ALIGN(8);
    } >FLASH

    /* Адрес начала данных для копирования */
    _sidata = LOADADDR(.data);

    /* 6. Инициализированные данные (переменные) - располагаются в RAM */
    .data :
    {
        . = ALIGN(8);
        _sdata = .;                  /* Начало данных в RAM */
        *(.data)                     /* .data секции */
        *(.data*)                     /* .data* секции */
        
        . = ALIGN(8);
        _edata = .;                   /* Конец данных в RAM */
    } >RAM AT> FLASH                  /* Данные хранятся во FLASH, но выполняются в RAM */

    /* 7. Неинициализированные данные (BSS) */
    .bss :
    {
        . = ALIGN(8);
        _sbss = .;                    /* Начало BSS */
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
        
        . = ALIGN(8);
        _ebss = .;                    /* Конец BSS */
        __bss_end__ = _ebss;
    } >RAM

    /* 8. Куча и стек - проверка, что они помещаются в RAM */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + _Min_Heap_Size;       /* Добавляем размер кучи */
        . = . + _Min_Stack_Size;      /* Добавляем размер стека */
        . = ALIGN(8);
    } >RAM

    /* Вершина стека - последний адрес RAM */
    __StackTop = ORIGIN(RAM) + LENGTH(RAM);
    _estack = __StackTop;              /* Для совместимости со стандартными startup-файлами */
    
    /* Дополнительные символы для работы с SRAM2 */
    _sram2_start = ORIGIN(RAM2);
    _sram2_end = ORIGIN(RAM2) + LENGTH(RAM2);

    /* Удаление отладочной информации из стандартных библиотек */
    /DISCARD/ :
    {
        libc.a (*)
        libm.a (*)
        libgcc.a (*)
    }

    /* Секция атрибутов ARM */
    .ARM.attributes 0 : { *(.ARM.attributes) }
}