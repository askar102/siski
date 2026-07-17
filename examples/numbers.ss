I32 loop(I32 i) {
    loop:
        if (i > 10) {
            goto end_loop;
        }
        i = i + 1;
    goto loop;

    end_loop:
        return i;
    fuck_lb:
        I32 L = 12313132;
}

I32 test_type(I32 a, U8 b) {
    return 10;
}

U16 test_return() {
    U16 ret = 10;
    return ret;
}

U0 test_void() {
    I32 ret = 10;
    return;
}

/* I32 test_const_return() {
    I8 ret = 10;
    return ret;
} */

// пишем через __, так как идея добавления кастомных аттрибутов
%__MNE_POHUI_NA_TYPES // soft version
%__MNE_POHUI_NA_TYPES__SOFT // soft version явно: попытается преобразовать встроенные типы по типу int
%__MNE_POHUI_NA_TYPES__HARD // полность забеьет
I32 foo(I16 t) {
    I32 var2 = t;
    return t;
}

I32 main() {
    U8 var1 = 1;
    I32 var2 = var1;

    
    return 10;
}

