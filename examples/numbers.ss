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

U8 test_type() {
    return 10;
}

I32 main() {
    I32 res = 10 + 10;
    U8 lol = 10;
    res = test_type();
    return res;
}

