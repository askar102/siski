int foo(int a = 10, int b) {
    if (a > 10) {
        return a + b;
    }
    else {
        return 0;
    }
}

int main() {
    int r = 0;
    bar(1, 2, -3);
    goto skip;
    r = 999;
    skip:
    return r;
}