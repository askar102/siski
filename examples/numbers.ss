int loop(int i) {
    loop:
        if (i > 10) {
            goto end_loop;
        }
        i = i + 1;
    goto loop;

    end_loop:
        return i;
}

int main() {
    int res = loop(1);
    return res;
}

