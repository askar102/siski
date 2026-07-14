int loop(int i) {
    loop:
        if (i > 10) {
            goto end_loop;
        }
        i = i + 1;
    goto loop;

    end_loop:
        return i;
    fuck_lb:
        int L = 12313132;
}

int main() {
    int res = loop(1);
    return res;
}

