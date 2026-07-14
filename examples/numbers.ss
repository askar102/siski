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

I32 main() {
    U8 lol = 10;
    I32 res = 10 + loop(lol);
    return res;
}

