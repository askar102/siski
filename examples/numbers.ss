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
    return ret;
}

I8 test_const_return() {
    return -1;
}

I32 main() {
    I32 res = 10 + 10;
    U8 lol = 10;
    res = test_type(res, lol);
    test_void();
    return res;
}

